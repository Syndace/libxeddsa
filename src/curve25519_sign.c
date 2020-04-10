#include <xeddsa.h>

#include <stdbool.h>

#include <fe.h>
#include <ge.h>
#include <sc.h>

#include <sodium.h>

// See comments in curve25519_sign for how this number was chosen.
#define STACKZERO_SIZE 1024

static const uint8_t HASH_PADDING[32] = {
    0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static const uint8_t ZERO[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t MINUS_ONE[32] = {
    0xec, 0xd3, 0xf5, 0x5c, 0x1a, 0x63, 0x12, 0x58,
    0xd6, 0x9c, 0xf7, 0xa2, 0xde, 0xf9, 0xde, 0x14,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
};

static void sc_clamp(uint8_t* scalar_out, const uint8_t* scalar_in) {
    // Mandated by RFC 7748: https://tools.ietf.org/html/rfc7748#page-8
    // Referred to as "clamping": https://moderncrypto.org/mail-archive/curves/2017/000858.html
    for (uint8_t i = 0; i < 32; i++)
        scalar_out[i] = scalar_in[i];

    scalar_out[ 0] = scalar_in[ 0] & 248;
    scalar_out[31] = scalar_in[31] & 127 | 64;
}

static void sc_neg(uint8_t* scalar_out, const uint8_t* scalar_in) {
    // ref10 only provides sc_muladd to calculate (ab+c) mod l.
    // This can be used to negate a scalar by choosing a = -1 mod l, b = scalar, c = 0.
    // The "difficult" part is calculating -1 mod l, where l = 2^252 + 27742317777372353535851937790883648493.
    // Python to the rescue!
    // Running (-1 % l).to_bytes(32, "little") yields the byte sequence stored in MINUS_ONE.
    sc_muladd(scalar_out, MINUS_ONE, scalar_in, ZERO);

    // sc_muladd leaves secret data on the stack. See comments in curve25519_sign for details.
    sodium_stackzero(STACKZERO_SIZE);
}

static void sc_cmov(uint8_t* scalar_inout, const uint8_t* scalar_in, bool move) {
    // The goal is a bitmask with either all ones or all zeros, based on the boolean value of move and in
    // constant time without conditional branches.
    // The code first negates `move` twice logically to get either a one or a zero.
    // Then it subtracts one from `move`, which either results in 0x00 (if `move` is true) or in 0xff due to
    // an 8 bit underflow (if move is false).
    uint8_t mask = ((uint8_t) !!move) - 1;

    // First, XOR both possible results
    for (uint8_t i = 0; i < 32; i++)
        scalar_inout[i] ^= scalar_in[i];

    // Second, and the condition mask to the mixture, which results in either of:
    // - out = (out ^ in) & 0x00 = 0x00,     if `move` is true
    // - out = (out ^ in) & 0xff = out ^ in, if `move` is false
    for (uint8_t i = 0; i < 32; i++)
        scalar_inout[i] &= mask;

    // Third, XOR out with in again, which results in either of:
    // - out ^ in = (0       ) ^ in = in,  if `move` is true
    // - out ^ in = (out ^ in) ^ in = out, if `move` is false
    for (uint8_t i = 0; i < 32; i++)
        scalar_inout[i] ^= scalar_in[i];

    // Overwrite even this single bit of information
    sodium_memzero(&mask, 1);
    sodium_memzero(&move, sizeof(bool));
}

/**
 * Sign a message using a Curve25519 private key.
 *
 * @param sig (out): An Ed25519-compatible signature that validates `msg` with the Ed25519 public key
 *     corresponding to this Curve25519 private key, as calculated by `curve25519_pub_to_ed25519_pub`. The
 *     signature is 64 bytes long and follows the byte format defined in RFC 8032 (on page 8).
 * @param curve25519_priv (in): The Curve25519 private key to sign `msg` with. The little-endian encoding of
 *     the u coordinate as per RFC 7748 (on page 4).
 * @param msg (in): The message to sign.
 * @param msg_size (in): Size of the message to sign.
 * @param nonce (in): 64 bytes of secure random data.
 */
void curve25519_sign(uint8_t* sig,
                     const uint8_t* curve25519_priv,
                     const uint8_t* msg,
                     const uint32_t msg_size,
                     const uint8_t* nonce) {
    /*
     * PREPARATION OF THE KEY MATERIAL
     *
     * This algorithm to sign data using a Curve25519 key pair has to tackle two issues. The first issue is
     * that the conversion of a Curve25519 public key to an Ed25519 public key is not unique when only having
     * access to the u coordinate of the Curve25519 public key, which is the case with the serialization
     * format commonly used. In fact the conversion is unique by the sign of the Ed25519 public key x
     * coordinate. This signing algorithm "solves" the problem by modifying the private key so that the sign
     * of the resulting Ed25519 public key is always zero.
     */

    ge_p3 ed25519_pub_ge;
    uint8_t ed25519_pub[32];

    uint8_t curve25519_priv_clamped[32], curve25519_priv_clamped_neg[32];

    // Clamp the private key before using it
    sc_clamp(curve25519_priv_clamped, curve25519_priv);

    // Calculate the Ed25519 public key from the Curve25519 private key
    ge_scalarmult_base(&ed25519_pub_ge, curve25519_priv_clamped);
    ge_p3_tobytes(ed25519_pub, &ed25519_pub_ge);

    // Create a negated copy of the clamped private key
    sc_neg(curve25519_priv_clamped_neg, curve25519_priv_clamped);

    // If the sign bit of the calculated Ed25519 public key is zero, the private key doesn't have to be
    // touched. If the sign bit is one, the private key has to be inverted prior to using it. The challenge is
    // to do so without conditional branching. This code dodges conditional branching by always calculating
    // the negated private key and using XOR-based conditional moving of the negated private key into the
    // untouched private key.
    sc_cmov(curve25519_priv_clamped, curve25519_priv_clamped_neg, ed25519_pub[31] & 0x80);

    // The negated private key is not required any more, overwrite it with zeros
    sodium_memzero(curve25519_priv_clamped_neg, 32);

    // Set the sign bit to zero after adjusting the private key accordingly
    ed25519_pub[31] &= 0x7f;

    /*
     * SIGNING
     *
     * The second problem this algorithm has to tackle is that Ed25519 signature algorithms don't use the
     * private scalar directly but rather use a seed to derive other data from. To create signatures
     * compatible with Ed25519, a modified version of the signing algorithm is required that does not depend
     * on a seed.
     */

    crypto_hash_sha512_state state;

    uint8_t r[64];
    uint8_t h[64];
    uint8_t s[32];

    ge_p3 R_ge;
    uint8_t R[32];

    // r = hash_1(a || M || Z) (mod q)
    // where:
    //     hash_i(X) = sha512(2 ^ 256 - 1 - i || X)
    // Sadly, the ref implementation of SHA-512 only comes with a single-part API and not with a
    // multi-part/streaming API. This makes it very inconvenient to use here. Instead, the streaming API of
    // libsodium's SHA-512 implementation is used. libsodium also comes with useful helpers to wipe secret
    // data off the stack without being scared of the compiler optimizing it away.
    crypto_hash_sha512_init(&state);
    crypto_hash_sha512_update(&state, HASH_PADDING, 32);
    crypto_hash_sha512_update(&state, curve25519_priv_clamped, 32);
    crypto_hash_sha512_update(&state, msg, msg_size);
    crypto_hash_sha512_update(&state, nonce, 64);
    crypto_hash_sha512_final(&state, r);
    sc_reduce(r);

    // R = rB
    ge_scalarmult_base(&R_ge, r);
    ge_p3_tobytes(R, &R_ge);

    // h = hash(R || A || M) (mod q)
    // where:
    //     hash(X) = sha512(X)
    crypto_hash_sha512_init(&state);
    crypto_hash_sha512_update(&state, R, 32);
    crypto_hash_sha512_update(&state, ed25519_pub, 32);
    crypto_hash_sha512_update(&state, msg, msg_size);
    crypto_hash_sha512_final(&state, h);
    sc_reduce(h);

    // The hash state is not required any more, overwrite it with zeros
    sodium_memzero(&state, sizeof(crypto_hash_sha512_state));

    // s = r + ha (mod q)
    sc_muladd(s, h, curve25519_priv_clamped, r);

    // The clamped private key is not required any more, overwrite it with zeros
    sodium_memzero(curve25519_priv_clamped, 32);

    // r is not required any more, overwrite it with zeros
    sodium_memzero(r, 64);

    // ge_scalarmult_base, sc_reduce, sc_muladd all store secret data on the stack without overwriting it
    // before returning. The sodium_stackzero function solves that by allocating a configurable amount of
    // space on the stack and overwriting it in the hope to catch all leftovers of stack variables allocated
    // by previously called functions. ge_scalarmult_base allocates around 470 bytes of stack, sc_reduce
    // around 330 and sc_muladd around 660 bytes which makes it the biggest stack consumer. Together with
    // things like parameters and the return addresses lying on the stack, at least 700 bytes have to be
    // cleared. To be sure (and to have a power of two!), STACKZERO_SIZE is set to 1024.
    sodium_stackzero(STACKZERO_SIZE);

    // return R || s
    for (uint8_t i = 0; i < 32; i++)
        *sig++ = R[i];
    for (uint8_t i = 0; i < 32; i++)
        *sig++ = s[i];
}

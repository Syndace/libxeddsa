#include <ge.h>
#include <sc.h>
#include <sodium.h>

#include <xeddsa.h>
#include "utils.c"

// Needed by `ed25519_priv_sign`.
static const uint8_t HASH_PADDING[32] = {
    0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

/**
 * Sign a message using a Curve25519/Ed25519 private key.
 *
 * @param sig (out): An Ed25519-compatible signature of `msg`.
 * @param priv (in): The Curve25519/Ed25519 private key to sign with.
 * @param msg (in): The message to sign.
 * @param msg_size (in): Size of the message to sign, in bytes.
 * @param nonce (in): 64 bytes of secure random data.
 */
void ed25519_priv_sign(uint8_t* sig,
                       const uint8_t* priv,
                       const uint8_t* msg,
                       const uint32_t msg_size,
                       const uint8_t* nonce) {
    ge_p3 ed25519_pub_ge;
    uint8_t A[32];

    crypto_hash_sha512_state state;

    uint8_t r[64];
    uint8_t h[64];
    uint8_t s[32];

    ge_p3 R_ge;
    uint8_t R[32];

    // Calculate the Ed25519 public key from the Ed25519 private key
    ge_scalarmult_base(&ed25519_pub_ge, priv);
    ge_p3_tobytes(A, &ed25519_pub_ge);

    // r = hash_1(a || M || Z) (mod q)
    // where:
    //     hash_i(X) = sha512(2 ^ 256 - 1 - i || X)
    // Sadly, the ref implementation of SHA-512 only comes with a single-part API and not with a
    // multi-part/streaming API. This makes it very inconvenient to use here. Instead, the streaming API of
    // libsodium's SHA-512 implementation is used. libsodium also comes with useful helpers to wipe private
    // data off the stack without being scared of the compiler optimizing it away. Note that the function
    // signatures of the sha512 API are declared to return an integer, however the implementations always
    // return the hardcoded value of 0 from all code paths.
    crypto_hash_sha512_init(&state);
    crypto_hash_sha512_update(&state, HASH_PADDING, 32);
    crypto_hash_sha512_update(&state, priv, 32);
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
    crypto_hash_sha512_update(&state, A, 32);
    crypto_hash_sha512_update(&state, msg, msg_size);
    crypto_hash_sha512_final(&state, h);
    sc_reduce(h);

    // s = r + ha (mod q)
    sc_muladd(s, h, priv, r);

    // return R || s
    for (uint8_t i = 0; i < 32; i++)
        *sig++ = R[i];
    for (uint8_t i = 0; i < 32; i++)
        *sig++ = s[i];

    sodium_memzero(&state, sizeof(crypto_hash_sha512_state));
    sodium_memzero(r, sizeof(r));
    sodium_stackzero(STACKZERO_SIZE);
}

/**
 * Sign a message using a Curve25519/Ed25519 seed.
 *
 * @param sig (out): An Ed25519-compatible signature of `msg`.
 * @param seed (in): The Curve25519/Ed25519 seed to sign with.
 * @param msg (in): The message to sign.
 * @param msg_size (in): Size of the message to sign, in bytes.
 */
void ed25519_seed_sign(uint8_t* sig, const uint8_t* seed, const uint8_t* msg, const uint32_t msg_size) {
    // libsodium needs seed and public key in 64 adjacent bytes
    uint8_t seed_and_pub[64];

    for (uint8_t i = 0; i < 32; i++)
        seed_and_pub[i] = seed[i];

    seed_to_ed25519_pub(&seed_and_pub[32], seed);

    // Returns the constant value of 0, thus the return value can be ignored.
    crypto_sign_ed25519_detached(sig, NULL, msg, msg_size, seed_and_pub);

    sodium_memzero(seed_and_pub, sizeof(seed_and_pub));
}

/**
 * Verify an Ed25519 signature.
 *
 * @param sig (in): An Ed25519-compatible signature of `msg`.
 * @param ed25519_pub (in): The Ed25519 public key to verify the signature with.
 * @param msg (in): The message.
 * @param msg_size (in): Size of the message, in bytes.
 * @return 0 in case of success, -1 if the verification fails.
 */
int ed25519_verify(const uint8_t* sig,
                   const uint8_t* ed25519_pub,
                   const uint8_t* msg,
                   const uint32_t msg_size) {
    return crypto_sign_verify_detached(sig, msg, msg_size, ed25519_pub);
}

#include <ge.h>
#include <sodium.h>

#include <xeddsa.h>
#include "utils.c"

/**
 * Negate a Curve25519/Ed25519 private key if necessary, such that the corresponding Ed25519 public key has
 * the sign bit set (or not set) as requested.
 *
 * @param priv_out (out): The adjusted Curve25519/Ed25519 private key.
 * @param priv_in (in): The original Curve25519/Ed25519 private key.
 * @param set_sign_bit: Whether the goal is for the sign bit to be set on the Ed25519 public key corresponding
 *     to the adjusted Curve25519/Ed25519 private key.
 */
void priv_force_sign(uint8_t* priv_out, const uint8_t* priv_in, const bool set_sign_bit) {
    ge_p3 ed25519_pub_ge;
    uint8_t ed25519_pub[32];

    uint8_t priv_neg[32];

    // Clamp the private key before using it
    sc_clamp(priv_out, priv_in);

    // Calculate the Ed25519 public key from the private key
    ge_scalarmult_base(&ed25519_pub_ge, priv_out);
    ge_p3_tobytes(ed25519_pub, &ed25519_pub_ge);

    // Create a negated copy of the clamped private key
    sc_neg(priv_neg, priv_out);

    // If the sign bit of the calculated Ed25519 public key is zero, the private key doesn't have to be
    // touched. If the sign bit is one, the private key has to be inverted prior to using it. The challenge is
    // to do so without conditional branching. This code dodges conditional branching by always calculating
    // the negated private key and using XOR-based conditional moving of the negated private key into the
    // untouched private key.
    sc_cmov(priv_out, priv_neg, ((ed25519_pub[31] & 0x80) >> 7) == !set_sign_bit);

    sodium_memzero(priv_neg, sizeof(priv_neg));
    sodium_stackzero(STACKZERO_SIZE);
}

/**
 * Derive the Curve25519/Ed25519 private key from a Curve25519/Ed25519 seed. Re-export of libsodiums/ref10s
 * `crypto_sign_ed25519_sk_to_curve25519` function for convenience.
 *
 * @param priv (out): The Curve25519/Ed25519 private key derived from the seed.
 * @param seed (in): The Curve25519/Ed25519 seed.
 */
void seed_to_priv(uint8_t* priv, const uint8_t* seed) {
    // Returns the constant value of 0, thus the return value can be ignored.
    crypto_sign_ed25519_sk_to_curve25519(priv, seed);
}

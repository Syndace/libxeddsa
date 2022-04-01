#include <ge.h>
#include <sodium.h>

#include <xeddsa.h>
#include "utils.c"

/**
 * Derive the Curve25519 public key from a Curve25519/Ed25519 private key.
 *
 * @param curve25519_pub (out): The Curve25519 public key.
 * @param priv (in): The Curve25519/Ed25519 private key.
 */
void priv_to_curve25519_pub(uint8_t* curve25519_pub, const uint8_t* priv) {
    uint8_t priv_clamped[32];

    // Clamp the private key before using it
    sc_clamp(priv_clamped, priv);

    // Calculate the Curve25519 public key from the private key. Note that while the function is declared to
    // return an integer, it always returns the constant value of 0, and can thus be treated as a void.
    crypto_scalarmult_curve25519_base(curve25519_pub, priv_clamped);

    sodium_memzero(priv_clamped, sizeof(priv_clamped));
    sodium_stackzero(STACKZERO_SIZE);
}

/**
 * Derive the Ed25519 public key from a Curve25519/Ed25519 private key.
 *
 * @param ed25519_pub (out): The Ed25519 public key.
 * @param priv (in): The Curve25519/Ed25519 private key.
 */
void priv_to_ed25519_pub(uint8_t* ed25519_pub, const uint8_t* priv) {
    ge_p3 ed25519_pub_ge;

    uint8_t priv_clamped[32];

    // Clamp the private key before using it
    sc_clamp(priv_clamped, priv);

    // Calculate the Ed25519 public key from the private key
    ge_scalarmult_base(&ed25519_pub_ge, priv_clamped);
    ge_p3_tobytes(ed25519_pub, &ed25519_pub_ge);

    sodium_memzero(priv_clamped, sizeof(priv_clamped));
    sodium_stackzero(STACKZERO_SIZE);
}

/**
 * Derive the Ed25519 public key from a Curve25519/Ed25519 seed.
 *
 * @param ed25519_pub (out): The Ed25519 public key.
 * @param seed (in): The Curve25519/Ed25519 seed.
 */
void seed_to_ed25519_pub(uint8_t* ed25519_pub, const uint8_t* seed) {
    uint8_t priv[32];

    seed_to_priv(priv, seed);
    priv_to_ed25519_pub(ed25519_pub, priv);

    sodium_memzero(priv, sizeof(priv));
}

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <sodium.h>
#include <xeddsa.h>

#include <test.h>

#define NUM_KEY_PAIRS 8192

int test_pub_derivation() {
    uint8_t priv[32], curve_pub_original[32], curve_pub_derived[32];
    uint8_t seed[32], ed_pub_original[32], ed_pub_derived[32];

    bool failed = false;

    // Test on a set of different key pairs
    for (uint32_t i = 0; i < NUM_KEY_PAIRS; i++) {
        // Generate a Curve25519 key pair
        crypto_box_keypair(curve_pub_original, priv);

        // Derive the Curve25519 public key from the private key
        priv_to_curve25519_pub(curve_pub_derived, priv);

        // Check whether the derived Curve25519 public key matches the original
        if (memcmp(curve_pub_original, curve_pub_derived, 32) != 0) {
            fprintf(stderr, "Derived Curve25519 public key does not match the original.\n");
            failed = true;
        }

        // Generate an Ed25519 key pair (seed + pub)
        crypto_sign_keypair(ed_pub_original, seed);

        // Derive the Ed25519 public key from the seed (also tests private key from seed derivation)
        seed_to_ed25519_pub(ed_pub_derived, seed);

        // Check whether the derived Ed25519 public key matches the original
        if (memcmp(ed_pub_original, ed_pub_derived, 32) != 0) {
            fprintf(stderr, "Derived Ed25519 public key does not match the original.\n");
            failed = true;
        }
    }

    return failed ? -1 : 0;
}

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <sodium.h>
#include <xeddsa.h>

#include <test.h>

#define NUM_KEYS 8192

int test_conversion_uniqueness() {
    uint8_t curve_priv[32], curve_pub_original[32], curve_pub_converted[32];
    uint8_t ed_seed[32], ed_pub_original[32], ed_pub_converted[32];

    bool failed = false;

    // Test on a set of different keys
    for (uint32_t i = 0; i < NUM_KEYS; i++) {
        // Generate a Curve25519 key pair (the private key is not used)
        randombytes_buf(curve_priv, 32);
        priv_to_curve25519_pub(curve_pub_original, curve_priv);

        // Convert the Curve25519 public key to an Ed25519 public key
        curve25519_pub_to_ed25519_pub(ed_pub_converted, curve_pub_original, 0);

        // Convert the Ed25519 public key back into a Curve25519 public key
        if (ed25519_pub_to_curve25519_pub(curve_pub_converted, ed_pub_converted) != 0) {
            fprintf(stderr, "Public key conversion from Ed25519 to Curve25519 failed.\n");
            failed = true;
        } else {
            // Check whether the converted Curve25519 public key is equal to the original
            if (memcmp(curve_pub_original, curve_pub_converted, 32) != 0) {
                fprintf(stderr, "Converted Curve25519 public key does not match the original.\n");
                failed = true;
            }
        }

        // Generate an Ed25519 key pair (the private key is not used)
        randombytes_buf(ed_seed, 32);
        seed_to_ed25519_pub(ed_pub_original, ed_seed);

        // Convert the Ed25519 public key to a Curve25519 public key
        if (ed25519_pub_to_curve25519_pub(curve_pub_converted, ed_pub_original) != 0) {
            fprintf(stderr, "Public key conversion from Ed25519 to Curve25519 failed.\n");
            failed = true;
        } else {
            // Convert the Curve25519 public key back into an Ed25519 public key. Set the sign accordingly.
            curve25519_pub_to_ed25519_pub(ed_pub_converted, curve_pub_converted, ed_pub_original[31] & 0x80);

            // Check whether the converted Ed25519 public key is equal to the original
            if (memcmp(ed_pub_original, ed_pub_converted, 32) != 0) {
                fprintf(stderr, "Converted Ed25519 public key does not match the original.\n");
                failed = true;
            }
        }
    }

    return failed ? -1 : 0;
}

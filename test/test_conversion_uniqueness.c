#include <test.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <xeddsa.h>

#include <sodium.h>

int test_conversion_uniqueness() {
    uint8_t curve_priv[32], curve_pub[32];
    uint8_t ed_pub[32], curve_pub_converted[32];

    bool failed = false;

    // Test on a set of different keys
    for (uint16_t i = 0; i < 4096; i++) {
        // Generate a Curve25519 key pair (the private key is not used)
        crypto_box_keypair(curve_pub, curve_priv);

        // Convert the Curve25519 public key to an Ed25519 public key (the function to test)
        curve25519_pub_to_ed25519_pub(ed_pub, curve_pub);

        // Convert the Ed25519 public key back into a Curve25519 public key and check whether the converted
        // Curve25519 public key is equal to the original
        if (crypto_sign_ed25519_pk_to_curve25519(curve_pub_converted, ed_pub) != 0 ||
            memcmp(curve_pub, curve_pub_converted, 32)                        != 0) {
            fprintf(stderr, "Converted public keys to not match.\n");
            failed = true;
        }
    }

    return failed ? -1 : 0;
}

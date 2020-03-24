#include <test.h>

#include <stdio.h>
#include <stdbool.h>

#include <xeddsa.h>

#include <sodium.h>

int test_libsodium_compatibility() {
    uint8_t curve_priv[32], curve_pub[32];
    uint8_t ed_pub[32];
    uint8_t sig[64];
    uint8_t nonce[64];

    uint8_t* msg = NULL;
    uint16_t msg_size;

    bool failed = false;

    // Test on a set of different messages
    for (uint8_t i = 0; i < 64; i++) {
        // Randomize the size of the message (0 to 65535 bytes)
        randombytes_buf(&msg_size, sizeof(uint16_t));

        // Allocate the buffer for the message
        if ((msg = calloc(msg_size, sizeof(uint8_t))) == NULL) {
            fprintf(stderr, "Can't allocate enough memory.\n");
            return -2;
        }

        // Fill the message with random bytes
        randombytes_buf(msg, msg_size);

        // Test each message on a set of different key pairs
        for (uint8_t j = 0; j < 64; j++) {
            // Generate a random nonce
            randombytes_buf(nonce, 64);

            // Generate a Curve25519 key pair
            crypto_box_keypair(curve_pub, curve_priv);

            // Convert the Curve25519 public key to an Ed25519 public key (required for the verification)
            curve25519_pub_to_ed25519_pub(ed_pub, curve_pub);

            // Sign the message using the Curve25519 private key
            curve25519_sign(sig, curve_priv, msg, msg_size, nonce);

            // Verify the message using the converted public key. This uses the verification function of
            // libsodium to confirm that signatures created by XEdDSA are valid.
            if (crypto_sign_verify_detached(sig, msg, msg_size, ed_pub) != 0) {
                fprintf(stderr, "Signature created by XEdDSA does not validate against libsodium.\n");
                failed = true;
            }
        }

        // Release the memory allocated for the message
        free(msg);
        msg = NULL;
    }

    return failed ? -1 : 0;
}

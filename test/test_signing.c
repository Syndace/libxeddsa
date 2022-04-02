#include <stdbool.h>
#include <stdio.h>

#include <sodium.h>
#include <xeddsa.h>

#include <test.h>

#define NUM_MESSAGES 64
#define NUM_KEY_PAIRS 64

int test_signing() {
    uint8_t curve_priv[32], curve_pub[32], priv[32];
    uint8_t ed_pub[32], ed_seed[32];
    uint8_t sig[64];
    uint8_t nonce[64];

    uint8_t* msg = NULL;
    uint16_t msg_size;

    bool failed = false;

    // Test on a set of different messages
    for (uint32_t i = 0; i < NUM_MESSAGES; i++) {
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
        for (uint32_t j = 0; j < NUM_KEY_PAIRS; j++) {
            // Generate a random nonce
            randombytes_buf(nonce, 64);

            // Generate a Curve25519 key pair
            randombytes_buf(curve_priv, 32);
            priv_to_curve25519_pub(curve_pub, curve_priv);

            // Adjust the private key such that the sign of the derived Ed25519 public key is zero
            priv_force_sign(priv, curve_priv, 0);

            // Sign the message using the Curve25519 private key
            ed25519_priv_sign(sig, priv, msg, msg_size, nonce);

            // Convert the Curve25519 public key to an Ed25519 public key
            curve25519_pub_to_ed25519_pub(ed_pub, curve_pub, 0);

            // Verify the message using the converted public key.
            if (ed25519_verify(sig, ed_pub, msg, msg_size) != 0) {
                fprintf(stderr, "Signature built by private key signing is not valid.\n");
                failed = true;
            }

            // Convert the Curve25519 public key to an Ed25519 public key, intentionally choosing the wrong
            // sign
            curve25519_pub_to_ed25519_pub(ed_pub, curve_pub, 1);

            // Verify the message using the converted public key.
            if (ed25519_verify(sig, ed_pub, msg, msg_size) == 0) {
                fprintf(stderr, "Signature that should be invalid was validated.\n");
                failed = true;
            }

            // Adjust the private key such that the sign of the derived Ed25519 public key is one
            priv_force_sign(priv, curve_priv, 1);

            // Sign the message using the Curve25519 private key
            ed25519_priv_sign(sig, priv, msg, msg_size, nonce);

            // Convert the Curve25519 public key to an Ed25519 public key
            curve25519_pub_to_ed25519_pub(ed_pub, curve_pub, 1);

            // Verify the message using the converted public key.
            if (ed25519_verify(sig, ed_pub, msg, msg_size) != 0) {
                fprintf(stderr, "Signature built by private key signing is not valid.\n");
                failed = true;
            }

            // Convert the Curve25519 public key to an Ed25519 public key, intentionally choosing the wrong
            // sign
            curve25519_pub_to_ed25519_pub(ed_pub, curve_pub, 0);

            // Verify the message using the converted public key.
            if (ed25519_verify(sig, ed_pub, msg, msg_size) == 0) {
                fprintf(stderr, "Signature that should be invalid was validated.\n");
                failed = true;
            }

            // Generate an Ed25519 key pair
            randombytes_buf(ed_seed, 32);
            seed_to_ed25519_pub(ed_pub, ed_seed);

            // Sign the message normally
            ed25519_seed_sign(sig, ed_seed, msg, msg_size);

            // Verify the message normally
            if (ed25519_verify(sig, ed_pub, msg, msg_size) != 0) {
                fprintf(stderr, "Signature built by seed signing is not valid.\n");
                failed = true;
            }
        }

        // Release the memory allocated for the message
        free(msg);
        msg = NULL;
    }

    return failed ? -1 : 0;
}

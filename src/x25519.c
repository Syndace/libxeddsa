#include <sodium.h>

#include <xeddsa.h>

/**
 * Perform Diffie-Hellman key agreement on Curve25519, also known as X25519. Re-export of libsodiums/ref10s
 * `crypto_scalarmult_curve25519` function for convenience.
 *
 * @param shared_secret (out): The shared secret.
 * @param priv (in): The private key partaking in the key agreement.
 * @param curve25519_pub (in): The public key partaking in the key agreement.
 * @return 0 in case of success, -1 if the public key was rejected due to suboptimal security propierties or
 *     if the shared secret consists of only zeros.
 */
int x25519(uint8_t* shared_secret, const uint8_t* priv, const uint8_t* curve25519_pub) {
    return crypto_scalarmult_curve25519(shared_secret, priv, curve25519_pub);
}

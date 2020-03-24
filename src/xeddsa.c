#include <xeddsa.h>

#include <sodium.h>

/**
 * Initializes the XEdDSA library and its dependencies. Has to be called at least once before using
 * `curve25519_sign` or `curve25519_pub_to_ed25519_pub`. Can be called multiple times, even from different
 * threads.
 *
 * @return 0 if the library was initialized, 1 if the library was already initialized and -1 in case of an
 *     error.
 */
int xeddsa_init() {
    return sodium_init();
}

#include <xeddsa.h>

#include <sodium.h>

/**
 * Initializes the library and its dependencies. Has to be called at least once before using and of the other
 * functions. Can be called multiple times, even from different threads.
 *
 * @return 0 if the library was initialized, 1 if the library was already initialized and -1 in case of an
 *     error.
 */
int xeddsa_init() {
    return sodium_init();
}

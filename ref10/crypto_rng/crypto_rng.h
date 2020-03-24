#ifndef crypto_rng_H
#define crypto_rng_H

#include <crypto_rng_salsa20.h>

#define crypto_rng crypto_rng_salsa20
#define crypto_rng_KEYBYTES crypto_rng_salsa20_KEYBYTES
#define crypto_rng_OUTPUTBYTES crypto_rng_salsa20_OUTPUTBYTES
#define crypto_rng_PRIMITIVE "salsa20"
#define crypto_rng_IMPLEMENTATION crypto_rng_salsa20_IMPLEMENTATION
#define crypto_rng_VERSION crypto_rng_salsa20_VERSION

#endif

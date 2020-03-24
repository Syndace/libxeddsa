#include <cross_platform.h>

#ifndef crypto_rng_salsa20_H
#define crypto_rng_salsa20_H

#define crypto_rng_salsa20_ref_KEYBYTES 32
#define crypto_rng_salsa20_ref_OUTPUTBYTES 736
 
#ifdef __cplusplus
extern "C" {
#endif
extern int INTERFACE crypto_rng_salsa20_ref(unsigned char *,unsigned char *,const unsigned char *);
#ifdef __cplusplus
}
#endif

#define crypto_rng_salsa20 crypto_rng_salsa20_ref
#define crypto_rng_salsa20_KEYBYTES crypto_rng_salsa20_ref_KEYBYTES
#define crypto_rng_salsa20_OUTPUTBYTES crypto_rng_salsa20_ref_OUTPUTBYTES
#define crypto_rng_salsa20_IMPLEMENTATION "crypto_rng/salsa20/ref"
#ifndef crypto_rng_salsa20_ref_VERSION
#define crypto_rng_salsa20_ref_VERSION "-"
#endif
#define crypto_rng_salsa20_VERSION crypto_rng_salsa20_ref_VERSION

#endif

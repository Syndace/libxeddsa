#include <cross_platform.h>

#ifndef crypto_scalarmult_curve25519_H
#define crypto_scalarmult_curve25519_H

#define crypto_scalarmult_curve25519_ref10_BYTES 32
#define crypto_scalarmult_curve25519_ref10_SCALARBYTES 32
 
#ifdef __cplusplus
extern "C" {
#endif
extern int INTERFACE crypto_scalarmult_curve25519_ref10(unsigned char *,const unsigned char *,const unsigned char *);
extern int INTERFACE crypto_scalarmult_curve25519_ref10_base(unsigned char *,const unsigned char *);
#ifdef __cplusplus
}
#endif

#define crypto_scalarmult_curve25519 crypto_scalarmult_curve25519_ref10
#define crypto_scalarmult_curve25519_base crypto_scalarmult_curve25519_ref10_base
#define crypto_scalarmult_curve25519_BYTES crypto_scalarmult_curve25519_ref10_BYTES
#define crypto_scalarmult_curve25519_SCALARBYTES crypto_scalarmult_curve25519_ref10_SCALARBYTES
#define crypto_scalarmult_curve25519_IMPLEMENTATION "crypto_scalarmult/curve25519/ref10"
#ifndef crypto_scalarmult_curve25519_ref10_VERSION
#define crypto_scalarmult_curve25519_ref10_VERSION "-"
#endif
#define crypto_scalarmult_curve25519_VERSION crypto_scalarmult_curve25519_ref10_VERSION

#endif

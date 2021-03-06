#include <cross_platform.h>

#ifndef crypto_sign_ed25519_H
#define crypto_sign_ed25519_H

#define crypto_sign_ed25519_ref10_SECRETKEYBYTES 64
#define crypto_sign_ed25519_ref10_PUBLICKEYBYTES 32
#define crypto_sign_ed25519_ref10_BYTES 64
#define crypto_sign_ed25519_ref10_DETERMINISTIC 1
 
#ifdef __cplusplus
extern "C" {
#endif
extern int INTERFACE crypto_sign_ed25519_ref10(unsigned char *,unsigned long long *,const unsigned char *,unsigned long long,const unsigned char *);
extern int INTERFACE crypto_sign_ed25519_ref10_open(unsigned char *,unsigned long long *,const unsigned char *,unsigned long long,const unsigned char *);
extern int INTERFACE crypto_sign_ed25519_ref10_keypair(unsigned char *,unsigned char *);
#ifdef __cplusplus
}
#endif

#define crypto_sign_ed25519 crypto_sign_ed25519_ref10
#define crypto_sign_ed25519_open crypto_sign_ed25519_ref10_open
#define crypto_sign_ed25519_keypair crypto_sign_ed25519_ref10_keypair
#define crypto_sign_ed25519_BYTES crypto_sign_ed25519_ref10_BYTES
#define crypto_sign_ed25519_SECRETKEYBYTES crypto_sign_ed25519_ref10_SECRETKEYBYTES
#define crypto_sign_ed25519_PUBLICKEYBYTES crypto_sign_ed25519_ref10_PUBLICKEYBYTES
#define crypto_sign_ed25519_DETERMINISTIC crypto_sign_ed25519_ref10_DETERMINISTIC
#define crypto_sign_ed25519_IMPLEMENTATION "crypto_sign/ed25519/ref10"
#ifndef crypto_sign_ed25519_ref10_VERSION
#define crypto_sign_ed25519_ref10_VERSION "-"
#endif
#define crypto_sign_ed25519_VERSION crypto_sign_ed25519_ref10_VERSION

#endif

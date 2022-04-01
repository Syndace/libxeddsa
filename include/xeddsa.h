#ifndef _XEDDSA_H_
#define _XEDDSA_H_

#include <stdbool.h>
#include <stdint.h>

#include <cross_platform.h>

#ifdef __cplusplus
extern "C" {
#endif
extern void INTERFACE ed25519_priv_sign(uint8_t*, const uint8_t*, const uint8_t*, const uint32_t, const uint8_t*);
extern void INTERFACE ed25519_seed_sign(uint8_t*, const uint8_t*, const uint8_t*, const uint32_t);
extern int  INTERFACE ed25519_verify(const uint8_t*, const uint8_t*, const uint8_t*, const uint32_t);

extern void INTERFACE curve25519_pub_to_ed25519_pub(uint8_t*, const uint8_t*, const bool);
extern int  INTERFACE ed25519_pub_to_curve25519_pub(uint8_t*, const uint8_t*);

extern void INTERFACE priv_to_curve25519_pub(uint8_t*, const uint8_t*);
extern void INTERFACE priv_to_ed25519_pub(uint8_t*, const uint8_t*);
extern void INTERFACE seed_to_ed25519_pub(uint8_t*, const uint8_t*);

extern void INTERFACE priv_force_sign(uint8_t*, const uint8_t*, const bool);
extern void INTERFACE seed_to_priv(uint8_t*, const uint8_t*);

extern int  INTERFACE x25519(uint8_t*, const uint8_t*, const uint8_t*);

extern int  INTERFACE xeddsa_init();

extern const unsigned XEDDSA_VERSION_MAJOR;
extern const unsigned XEDDSA_VERSION_MINOR;
extern const unsigned XEDDSA_VERSION_REVISION;
#ifdef __cplusplus
}
#endif

#endif /* _XEDDSA_H_ */

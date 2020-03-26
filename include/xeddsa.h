/** @file */

#ifndef _XEDDSA_H_
#define _XEDDSA_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
extern int xeddsa_init();
extern void curve25519_sign(uint8_t*, const uint8_t*, const uint8_t*, const uint32_t, const uint8_t*);
extern void curve25519_pub_to_ed25519_pub(uint8_t*, const uint8_t*);
#ifdef __cplusplus
}
#endif

#endif /* _XEDDSA_H_ */

#include <cross_platform.h>

#ifndef SC_H
#define SC_H

/*
The set of scalars is \Z/l
where l = 2^252 + 27742317777372353535851937790883648493.
*/

#define sc_reduce crypto_sign_ed25519_ref10_sc_reduce
#define sc_muladd crypto_sign_ed25519_ref10_sc_muladd

#ifdef __cplusplus
extern "C" {
#endif
extern void INTERFACE sc_reduce(unsigned char *);
extern void INTERFACE sc_muladd(unsigned char *,const unsigned char *,const unsigned char *,const unsigned char *);
#ifdef __cplusplus
}
#endif

#endif

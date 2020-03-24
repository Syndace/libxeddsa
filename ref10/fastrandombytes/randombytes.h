#include <cross_platform.h>

#ifndef randombytes_h
#define randombytes_h

#ifdef __cplusplus
extern "C" {
#endif

extern void INTERFACE randombytes(unsigned char *,unsigned long long);
extern unsigned long long INTERFACE randombytes_calls;
extern unsigned long long INTERFACE randombytes_bytes;

#ifdef __cplusplus
}
#endif

#endif

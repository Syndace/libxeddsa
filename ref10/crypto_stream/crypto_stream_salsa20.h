#include <cross_platform.h>

#ifndef crypto_stream_salsa20_H
#define crypto_stream_salsa20_H

#define crypto_stream_salsa20_ref_KEYBYTES 32
#define crypto_stream_salsa20_ref_NONCEBYTES 8
 
#ifdef __cplusplus
extern "C" {
#endif
extern int INTERFACE crypto_stream_salsa20_ref(unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
extern int INTERFACE crypto_stream_salsa20_ref_xor(unsigned char *,const unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
#ifdef __cplusplus
}
#endif

#define crypto_stream_salsa20 crypto_stream_salsa20_ref
#define crypto_stream_salsa20_xor crypto_stream_salsa20_ref_xor
#define crypto_stream_salsa20_KEYBYTES crypto_stream_salsa20_ref_KEYBYTES
#define crypto_stream_salsa20_NONCEBYTES crypto_stream_salsa20_ref_NONCEBYTES
#define crypto_stream_salsa20_IMPLEMENTATION "crypto_stream/salsa20/ref"
#ifndef crypto_stream_salsa20_ref_VERSION
#define crypto_stream_salsa20_ref_VERSION "-"
#endif
#define crypto_stream_salsa20_VERSION crypto_stream_salsa20_ref_VERSION

#endif

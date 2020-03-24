#include "kernelrandombytes.h"
#include <Windows.h>

#define RtlGenRandom SystemFunction036

#ifdef __cplusplus
extern "C" {
#endif
BOOLEAN NTAPI RtlGenRandom(PVOID RandomBuffer, ULONG RandomBufferLength);
#ifdef __cplusplus
}
#endif

void kernelrandombytes(unsigned char *x,unsigned long long xlen)
{
  int i;

  while (xlen > 0) {
    if (xlen < 256) i = xlen; else i = 256;
    RtlGenRandom(x, i);
    x += i;
    xlen -= i;
  }
}

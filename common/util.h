#ifndef __UTIL_H
#define __UTIL_H

#include <stdint.h>

static int verify(int n, const volatile int* test, const int* verify)
{
  int i;
  // Unrolled for faster verification
  for (i = 0; i < n/2*2; i+=2)
  {
    int t0 = test[i], t1 = test[i+1];
    int v0 = verify[i], v1 = verify[i+1];
    if (t0 != v0) return i+1;
    if (t1 != v1) return i+2;
  }
  if (n % 2 != 0 && test[n-1] != verify[n-1])
    return n;
  return 0;
}

static void __attribute__((noinline)) barrier(int ncores)
{
  static volatile int sense;
  static volatile int count;
  static __thread int threadsense;  // non shared variable in TLS

  __sync_synchronize();   // fence

  threadsense = !threadsense;
  if (__sync_fetch_and_add(&count, 1) == ncores-1)
  {
    count = 0;
    sense = threadsense;
  }
  else while(sense != threadsense)
    ;

  __sync_synchronize();
}

// this needs to be enabled to calculate IPC and CPI
// #ifdef __riscv
// #include "encoding.h"
// #endif

// #define stringify_1(s) #s
// #define stringify(s) stringify_1(s)
// #define stats(code, iter) do { \
//     unsigned long _c = -read_csr(mcycle), _i = -read_csr(minstret); \
//     code; \
//     _c += read_csr(mcycle), _i += read_csr(minstret); \
//     if (cid == 0) \
//       printf("\n%s: %ld cycles, %ld.%ld cycles/iter, %ld.%ld CPI\n", \
//              stringify(code), _c, _c/iter, 10*_c/iter%10, _c/_i, 10*_c/_i%10); \
//   } while(0)

#endif //__UTIL_H

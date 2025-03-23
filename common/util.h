//**************************************************************************
// util.h
//--------------------------------------------------------------------------
// Hiruna Vishwamith
// UOM
// 10/03/2025
//
//
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// gards and includes

#ifndef __UTIL_H
#define __UTIL_H

#include <stdint.h>

static int verify(int n, const volatile int *test, const int *verify)
{
  int i;
  // Unrolled for faster verification
  for (i = 0; i < n / 2 * 2; i += 2)
  {
    int t0 = test[i], t1 = test[i + 1];
    int v0 = verify[i], v1 = verify[i + 1];
    if (t0 != v0)
      return i + 1;
    if (t1 != v1)
      return i + 2;
  }
  if (n % 2 != 0 && test[n - 1] != verify[n - 1])
    return n;
  return 0;
}

// // Atomic fetch-and-add using RISC-V atomic instruction
// static inline int atomic_fetch_add(volatile int *ptr, int val)
// {
//   int old;
//   asm volatile(
//       "amoadd.w.aqrl %0, %2, (%1)" // Atomic add with acquire-release semantics
//       : "=r"(old)
//       : "r"(ptr), "r"(val)
//       : "memory");
//   return old;
// }

// static void __attribute__((noinline)) barrier(int ncores)
// {
//   static volatile int sense;
//   static volatile int count;
//   static __thread int threadsense; // non shared variable in TLS

//   __sync_synchronize(); // fence

//   threadsense = !threadsense;

//   int old_count = atomic_fetch_add(&count, 1); // Atomically increment count
//   if (old_count == ncores - 1)
//   {
//     // Last thread to arrive
//     count = 0;           // Reset counter for next barrier
//     sense = threadsense; // Update global sense to this thread's sense
//   }
//   else
//   {
//     // Other threads wait for global sense to match their local sense
//     while (sense != threadsense)
//     {
//       // Busy wait (could optimize with a yield if available)
//     }
//   }

//   __sync_synchronize();
// }

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

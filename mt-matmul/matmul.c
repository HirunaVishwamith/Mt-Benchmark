#include "dataset.h"


void __attribute__((noinline)) matmul(const int coreid, const int ncores, const int lda,  const data_t A[], const data_t B[], data_t C[] )
{
   int i, j, k;
   int block = lda / ncores;
   int start = block * coreid;
 
   for ( j = start; j < (start+block); j++ ) 
   {
      for ( k = 0; k < lda; k++ )  
      {
         for ( i = 0; i < lda; i++ ) 
         {
            C[i + j*lda] += A[j*lda + k] * B[k*lda + i];
         }
      }
   }
}

// #include <stddef.h>

// #pragma GCC optimize ("unroll-loops")

// void matmul(const size_t coreid, const size_t ncores, const size_t lda,  const data_t A[], const data_t B[], data_t C[])
// {
//   size_t i, j, k;
//   size_t block = lda / ncores;
//   size_t start = block * coreid;
 
//   for (i = 0; i < lda; i++) {
//     for (j = start; j < (start+block); j++) {
//       data_t sum = 0;
//       for (k = 0; k < lda; k++)
//         sum += A[j*lda + k] * B[k*lda + i];
//       C[i + j*lda] = sum;
//     }
//   }
// }



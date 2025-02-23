//**************************************************************************
// Multi-threaded Matrix Multiply benchmark
//--------------------------------------------------------------------------
// Hiruna Vishwamith
// UOM
//
//
// This benchmark multiplies two 2-D arrays together and writes the results to
// a third vector. The input data (and reference data) should be generated
// using the matmul_gendata.pl perl script and dumped to a file named
// dataset.h. 


// print out arrays, etc.
//#define DEBUG

//--------------------------------------------------------------------------
// Includes 

// #include <string.h>
// #include <stdlib.h>
// #include <stdio.h>




//--------------------------------------------------------------------------
// Input/Reference Data

#include "dataset.h"
 

//--------------------------------------------------------------------------
// Basic Utilities and Multi-thread Support

#include "util.h"
#include "syscall.h"

   
//--------------------------------------------------------------------------
// matmul function
 
 extern void __attribute__((noinline)) matmul(const int coreid, const int ncores, const int lda,  const data_t A[], const data_t B[], data_t C[] );


//--------------------------------------------------------------------------
// Main
//
// all threads start executing thread_entry(). Use their "coreid" to
// differentiate between threads (each thread is running on a separate core).
  
void thread_entry(int cid, int nc)
{
    static data_t results_data[ARRAY_SIZE];

//     stats(matmul(cid, nc, DIM_SIZE, input1_data, input2_data, results_data); barrier(nc), DIM_SIZE/DIM_SIZE/DIM_SIZE); // this needs to be enablled to calculate IPC

    matmul(cid, nc, DIM_SIZE, input1_data, input2_data, results_data); 
    barrier(nc);

    int res = verify(ARRAY_SIZE, results_data, verify_data);

    // Call the syscall to print the matrix to UART
    if (cid == 0) { // Optional: Only core 0 prints to avoid potential output issues in simple setup
        syscall_print_matrix(DIM_SIZE, results_data);
    }

    exit(res);
}

// void thread_entry(int cid, int nc)
// {
//       printf("Hart ID: %lu\n", cid); // Print the hart ID <- this print function is defined in syscalls.c
// }

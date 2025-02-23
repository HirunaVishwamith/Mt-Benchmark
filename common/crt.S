# See LICENSE for license details.

#include "encoding.h"

#define NUM_CORES 2 // Define the number of cores here. Adjust as needed.
#define DRAM_BASE 0x0000000010000000
#define STKSHIFT 17
#define TLSSHIFT 12

  .section ".text.init" ## code starts here in the elf file
  .globl _start
_start:
  li  x1, 0
  li  x2, 0
  li  x3, 0
  li  x4, 0
  li  x5, 0
  li  x6, 0
  li  x7, 0
  li  x8, 0
  li  x9, 0
  li  x10,0
  li  x11,0
  li  x12,0
  li  x13,0
  li  x14,0
  li  x15,0
  li  x16,0
  li  x17,0
  li  x18,0
  li  x19,0
  li  x20,0
  li  x21,0
  li  x22,0
  li  x23,0
  li  x24,0
  li  x25,0
  li  x26,0
  li  x27,0
  li  x28,0
  li  x29,0
  li  x30,0
  li  x31,0


  # make sure XLEN agrees with compilation choice
  li t0, 1
  slli t0, t0, 31
  bgez t0, 1f  # if t0 >= 0, branch forward to label 1

2:                    # Local label '2'
  li a0, 1           # Load Immediate: puts value 1 into register a0
  sw a0, tohost, t0  # Store Word: stores the value in a0 to address 'tohost', using t0 as temporary
  j 2b               # Jump backwards (b = backward) to label 2
1:


  # initialize trap vector
  la t0, trap_entry       # Load Address of trap_entry into t0
  csrw mtvec, t0         # Write t0 to mtvec CSR (Control/Status Register)

  # initialize global pointer
.option push
.option norelax
  la gp, __global_pointer$
.option pop



  # give each core 128KB of stack
  li  t0, DRAM_BASE
  add sp, a0, 1
  sll sp, sp, STKSHIFT
  add sp, sp, t0          # Add DRAM_BASE to the offset to get final sp


  # get core id
  csrr a0, mhartid

  # load number of cores into a1
  li a1, NUM_CORES

  # a0 and a1 are caller-saved registers variable cid is stored in a0 and nc is on a1

   # jump to thread_entry with cid in a0 and nc in a1
  jal ra, thread_entry  // Jump and Link to thread_entry

  .align 2
trap_entry:
  addi sp, sp, -272

  sd x1, 1*8(sp)
  sd x2, 2*8(sp)
  sd x3, 3*8(sp)
  sd x4, 4*8(sp)
  sd x5, 5*8(sp)
  sd x6, 6*8(sp)
  sd x7, 7*8(sp)
  sd x8, 8*8(sp)
  sd x9, 9*8(sp)
  sd x10, 10*8(sp)
  sd x11, 11*8(sp)
  sd x12, 12*8(sp)
  sd x13, 13*8(sp)
  sd x14, 14*8(sp)
  sd x15, 15*8(sp)
  sd x16, 16*8(sp)
  sd x17, 17*8(sp)
  sd x18, 18*8(sp)
  sd x19, 19*8(sp)
  sd x20, 20*8(sp)
  sd x21, 21*8(sp)
  sd x22, 22*8(sp)
  sd x23, 23*8(sp)
  sd x24, 24*8(sp)
  sd x25, 25*8(sp)
  sd x26, 26*8(sp)
  sd x27, 27*8(sp)
  sd x28, 28*8(sp)
  sd x29, 29*8(sp)
  sd x30, 30*8(sp)
  sd x31, 31*8(sp)

  csrr a0, mcause
  csrr a1, mepc
  mv a2, sp
  jal handle_trap
  csrw mepc, a0

  # Remain in M-mode after eret
  li t0, MSTATUS_MPP
  csrs mstatus, t0

  ld x1, 1*8(sp)
  ld x2, 2*8(sp)
  ld x3, 3*8(sp)
  ld x4, 4*8(sp)
  ld x5, 5*8(sp)
  ld x6, 6*8(sp)
  ld x7, 7*8(sp)
  ld x8, 8*8(sp)
  ld x9, 9*8(sp)
  ld x10, 10*8(sp)
  ld x11, 11*8(sp)
  ld x12, 12*8(sp)
  ld x13, 13*8(sp)
  ld x14, 14*8(sp)
  ld x15, 15*8(sp)
  ld x16, 16*8(sp)
  ld x17, 17*8(sp)
  ld x18, 18*8(sp)
  ld x19, 19*8(sp)
  ld x20, 20*8(sp)
  ld x21, 21*8(sp)
  ld x22, 22*8(sp)
  ld x23, 23*8(sp)
  ld x24, 24*8(sp)
  ld x25, 25*8(sp)
  ld x26, 26*8(sp)
  ld x27, 27*8(sp)
  ld x28, 28*8(sp)
  ld x29, 29*8(sp)
  ld x30, 30*8(sp)
  ld x31, 31*8(sp)

  addi sp, sp, 272
  mret

.section ".tohost","aw",@progbits
.align 6
.globl tohost
tohost: .dword 0  ## this sould be uart
.align 6
.globl fromhost
fromhost: .dword 0

//**************************************************************************
// syscall.h
//--------------------------------------------------------------------------

// Hiruna Vishwamith
// UOM



//--------------------------------------------------------------------------
// gards and includes

#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

// UART base address
#define UART_TX 0xe0001030
#define UART_RX 0xe000102c

// Helper function to send a character to UART
static inline void uart_send_char(char c) {
  volatile uint32_t *uart_tx_reg = (volatile uint32_t *)(UART_TX);
  volatile uint32_t *uart_rx_reg = (volatile uint32_t *)(UART_RX);

    while((*uart_rx_reg)&16)
		;

  *uart_tx_reg = (uint32_t)c; // Assuming writing to this address sends the char
}

// Helper function to send a null-terminated string to UART
void uart_send_string(const char *s) {
  while (*s != '\0') {
    uart_send_char(*s++);
  }
}

void uart_send_integer(int n) {
    char buffer[12]; // Enough for largest 32-bit integer + null terminator
    int i = 0;
    if (n == 0) {
        uart_send_char('0');
        return;
    }
    if (n < 0) {
        uart_send_char('-');
        n = -n;
    }
    while (n != 0) {
        buffer[i++] = (n % 10) + '0';
        n = n / 10;
    }
    while (i > 0) {
        uart_send_char(buffer[--i]);
    }
}

// Syscall function to print the matrix
void syscall_print_matrix(int lda, int *matrix) {
    // Prepare syscall arguments (if needed for a more complex syscall mechanism)
    // In this simple case, we might directly implement the printing here for bare-metal

    uart_send_string("Result Matrix:\n");
    for (int i = 0; i < lda; i++) {
        for (int j = 0; j < lda; j++) {
            uart_send_integer(matrix[j + i * lda]); // Assuming row-major order 
            uart_send_string(" ");
        }
        uart_send_string("\n");
    }
    uart_send_string("\n");

}

void __attribute__((weak)) thread_entry(int cid, int nc)
{
  // multi-threaded programs override this function.
  // for the case of single-threaded programs, only let core 0 proceed.
  while (cid != 0);
}

int __attribute__((weak)) main(int argc, char** argv)
{
  // single-threaded programs override this function.
  uart_send_string("Implement main(), foo!\n");
  return -1;
}

// void* memcpy(void* dest, const void* src, size_t len)
// {
//   if ((((uintptr_t)dest | (uintptr_t)src | len) & (sizeof(uintptr_t)-1)) == 0) {
//     const uintptr_t* s = src;
//     uintptr_t *d = dest;
//     while (d < (uintptr_t*)(dest + len))
//       *d++ = *s++;
//   } else {
//     const char* s = src;
//     char *d = dest;
//     while (d < (char*)(dest + len))
//       *d++ = *s++;
//   }
//   return dest;
// }

// void* memset(void* dest, int byte, size_t len)
// {
//   if ((((uintptr_t)dest | len) & (sizeof(uintptr_t)-1)) == 0) {
//     uintptr_t word = byte & 0xFF;
//     word |= word << 8;
//     word |= word << 16;
//     word |= word << 16 << 16;

//     uintptr_t *d = dest;
//     while (d < (uintptr_t*)(dest + len))
//       *d++ = word;
//   } else {
//     char *d = dest;
//     while (d < (char*)(dest + len))
//       *d++ = byte;
//   }
//   return dest;
// }

// static void init_tls()
// {
//   register void* thread_pointer asm("tp");
//   extern char _tls_data;
//   extern __thread char _tdata_begin, _tdata_end, _tbss_end;
//   size_t tdata_size = &_tdata_end - &_tdata_begin;
//   memcpy(thread_pointer, &_tls_data, tdata_size);
//   size_t tbss_size = &_tbss_end - &_tdata_end;
//   memset(thread_pointer + tdata_size, 0, tbss_size);
// }

void exit(int code)
{
  while (1);
}

// void _init(int cid, int nc)
// {
//   init_tls();
//   thread_entry(cid, nc);

//   // only single-threaded programs should ever get here.
//   int ret = main(0, 0);


//   exit(ret);
// }


#endif // __SYSCALL_H__

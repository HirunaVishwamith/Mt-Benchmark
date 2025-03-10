//**************************************************************************
// syscalls.h 
//--------------------------------------------------------------------------

// Hiruna Vishwamith
// UOM
// 10/03/2025
//
//


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
static void uart_send_char(char c) {
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

// Syscall function to print the array
void syscall_print_array(int lda, int *array) {
    // Prepare syscall arguments (if needed for a more complex syscall mechanism)
    // In this simple case, we might directly implement the printing here for bare-metal

    uart_send_string("Result Array:\n");
    for (int i = 0; i < lda; i++) {
      uart_send_integer(array[i]);
      uart_send_string(" ");
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


void exit(int code)
{
  while (1);
}


#endif // __SYSCALL_H__

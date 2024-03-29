#ifndef UART_PL011_H
#define UART_PL011_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef volatile struct __attribute__((packed)) {
    uint32_t DR;                /* 0x0 Data Register */
    uint32_t RSRECR;            /* 0x4 Receive status / error clear register */
    uint32_t _reserved0[4];     /* 0x8 - 0x14 reserved */
    const uint32_t FR;          /* 0x18 Flag register */
    uint32_t _reserved1;        /* 0x1C reserved */
    uint32_t ILPR;              /* 0x20 Low-power counter register */
    uint32_t IBRD;              /* 0x24 Integer baudrate register */
    uint32_t FBRD;              /* 0x28 Fractional baudrate register */
    uint32_t LCRH;              /* 0x2C Line control register */
    uint32_t CR;                /* 0x30 Control register */
} uart_registers;

#define FR_TXFF         (1 << 5u) /* Transmit FIFO full */
#define FR_RXFE         (1 << 4u) /* Transmit FIFO empty */

#define DR_DATA_MASK    (0xFFu)
#define RSRECR_ERR_MASK (0xFu)

enum uart_error {
        UART_OK = 0,
        UART_INVALID_ARGUMENT_BAUDRATE,
        UART_INVALID_ARGUMENT_WORDSIZE,
        UART_INVALID_ARGUMENT_STOP_BITS,
        UART_RECEIVE_ERROR,
        UART_NO_DATA
};

void uart_putchar(int c);
void uart_write(const char *s, int size);
enum uart_error uart_getchar(int *c);
#endif

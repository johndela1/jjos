#include "uart_pl011.h"

static uart_registers* uart0 = (uart_registers*)0x101f1000u;

void uart_putchar(char c)
{
  // while (uart0->FR && FR_TXFF);
  uart0->DR = (unsigned int)c;
}

void uart_write(const char *s, int size)
{
  while(size--)
    uart0->DR = (unsigned int)*s++;
}

uart_error uart_getchar(char *c)
{
  if (uart0->FR & FR_RXFE) {
    return UART_NO_DATA;
  }
  *c = uart0->DR & DR_DATA_MASK;
  if (uart0->RSRECR & RSRECR_ERR_MASK) {
    /* The character had an error */
    uart0->RSRECR &= RSRECR_ERR_MASK;
    return UART_RECEIVE_ERROR;
  }
  return UART_OK;
}

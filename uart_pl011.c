#include "uart_pl011.h"

static uart_registers* uart0 = (uart_registers*)0x101f1000u;

void uart_putchar(char c)
{
  // while (uart0->FR && FR_TXFF);
  uart0->DR = (unsigned int)c;
}

void uart_write(const char *s)
{
  while(*s)
    uart_putchar(*s++);
}

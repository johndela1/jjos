volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;

void print_char(const char c)
{
  *UART0DR = (unsigned int)c; /* Transmit char */
}

void print_ascii(const char *s) {
  while(*s != '\0')
    print_char(*s++);
}

void hello() {
  print_ascii("Hello world C!\n");
}

int add(int x, int y)
{
  int res = x + y;
  print_char(((char)res)+48);
  print_char('\n');
  return res;
}

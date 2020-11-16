#include "lib.h"
#include "uart_pl011.h"

char buf[64];
char buf_idx = 0;

void run_divmods()
{
  long long res;
  unsigned int t1;
  unsigned int t2;

  t1 = get_time();
  for (int x=0; x<1; x++) {
    res = divmod(7, 3);
    int q = res >> 32;
    int r = res & ~0;
    print_num(q);
    print_num(r);
  }
  t2 = get_time();
  print_num(t2 - t1);

  struct result res2;
  t1 = get_time();

  for (int x=0; x<1; x++) {
    res2 = cdivmod(7, 3);
    print_num(res2.quot);
    print_num(res2.rem);
  }
  t2 = get_time();
  print_num(t2 - t1);
}

static void parse_cmd()
{
  if (!kstrncmp("uname\r", buf, kstrlen("uname\r")))
      uart_write("jjos\n");
  else
    uart_write("command not found\n");
}

void start_kernel(void)
{
  char c;
  uart_write("$ ");
  while(1) {
    if (uart_getchar(&c) == UART_OK) {
      uart_putchar(c);
      buf[buf_idx] = c;
      buf_idx++;
      if (c == '\r') {
	buf[buf_idx] = 0;
	uart_putchar('\n');
	buf_idx = 0;
	parse_cmd();
	uart_write("$ ");
      }
    }
  }
}

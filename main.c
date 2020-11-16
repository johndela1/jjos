#include "lib.h"
#include "uart_pl011.h"

void start_kernel(void)
{
  long long res;
  unsigned int t1;
  unsigned int t2;

  uart_write("uart_write\n");

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

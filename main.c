#include "lib.h"


int c_main()
{
  long long res;
  unsigned int t1;
  unsigned int t2;

  t1 = get_time();
  for (int x=0; x<=50000000; x++) {
    res = divmod(7, 3);
    int q = res >> 32;
    int r = res & ~0;
  }
  t2 = get_time();
  print_num(t2 - t1);
  print_ascii("done asm\n");
  struct result res2;
  t1 = get_time();

  for (int x=0; x<=50000000; x++) {
    res2 = cdivmod(7, 3);
  }
  t2 = get_time();
  print_num(t2 - t1);
  print_ascii("done c\n");
}

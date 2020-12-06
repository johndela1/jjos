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
      kputs("jjos\n");
  else if (!kstrncmp("sort", buf, kstrlen("sort"))) {
    char *bufp = &buf[kstrlen("sort")+1];
    int len = kstrlen(bufp);
    ksort(bufp, len);
    pr_arr(bufp, len);
  }
  else
    kputs("command not found\n");
}

/*
struct tty_operations {
    int (*write)(struct tty_struct * tty,
                      const unsigned char *buf, int count);
}

struct tty_struct {
    struct tty_operations *ops;

}

static const struct tty_operations uart_ops = {
        .write = uart_write;
}

static int n_tty_write(struct tty_struct *tty, struct file *file,
                           const unsigned char *buf, int nr)
{
    const unsigned char *b = buf;
    while(nr > 0) {
        c = tty->ops->write(tty, b, nr);
        b += c;
        nr -= c;
    }
}
*/


void start_kernel(void)
{
  char c;

  kputs("# ");
  while(1) {
    if (uart_getchar(&c) == UART_OK) {
      uart_putchar(c);
      buf[buf_idx % 64] = c;
      buf_idx++;
      if (c == '\r') {
	buf[buf_idx] = 0;

	kputs("\n");
	buf_idx = 0;
	parse_cmd();
	kputs("# ");
      }
    }
  }
}

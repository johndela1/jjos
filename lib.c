#include "uart_pl011.h"

//#define NULL (void *) 0

volatile unsigned int * const RTC = (unsigned int *)0x101E8000;
struct result {int quot; int rem;};

struct result cdivmod(int dev, int div)
{
  struct result res;
  int quot = 0;
  while (dev >= div) {
      quot++;
      dev = dev - div;
  }
  res.quot = quot;
  res.rem = dev;
  return res;
}

unsigned int get_time()
{
  return *RTC;
}

void print_num(int n)
{
  #define BUFLEN 256
  struct result r;
  char buf[BUFLEN];
  char *bufp = &buf[BUFLEN-1];
  if (n == 0)
    *--bufp = '0';
  while (n) {
    r = cdivmod(n, 10);
    *--bufp = 48 + r.rem;
    n = r.quot;
  }
  uart_write(bufp);
  uart_write("\n");
}

void pr_arr(char a[], int size)
{
  for (int i = 0; i < size; i++) {
    uart_putchar(a[i]);
    uart_putchar(' ');
  }
  uart_putchar('\r');
  uart_putchar('\n');
}

#define MEMSIZE 4096
static char memory[MEMSIZE];
static char *basep = memory;

void *kmalloc(int size)
{
  if (basep+size < basep+MEMSIZE) {
    basep += size;
    return basep - size;
  } else {
    return NULL;
  }
}

void kfree(void *ptr)
{
  basep = ptr;
}

void merge(char a[], int size, int m)
{
  char *buf = kmalloc(size * sizeof (int));

  for (int i = 0, left = 0, right = m; i < size; i++) {
    buf[i] =
      left == m ? a[right++]
      : right == size ? a[left++]
      : a[left] <= a[right] ? a[left++]
      : a[right++];
  }

  for (int i = 0; i < size; i++)
    a[i] = buf[i];

  kfree(buf);
}

void sort(char a[], int size)
{
  if (size <= 1)
    return;
  int m = size >> 1;
  sort(a, m);
  sort(a + m, size - m);
  merge(a, size, m);
}

int kstrlen(const char *s)
{
  const char *sp;
  for(sp = s; *sp; sp++)
    ;
  return sp - s;
}

int jj_kstrncmp(const char *s1, const char *s2, int size)
{
  for (int i = 0; i < size; i++) {
    if (s1[i] != s2[i])
      return s1[i] < s2[i] ? -1 : 1;
    if (!s1[i])
      break;
  }
  return 0;
}

int kstrncmp(const char *cs, const char *ct, size_t count)
{
        unsigned char c1, c2;

        while (count) {
                c1 = *cs++;
                c2 = *ct++;
                if (c1 != c2)
                        return c1 < c2 ? -1 : 1;
                if (!c1)
                        break;
                count--;
        }
        return 0;
}

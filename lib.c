#define NULL (void *) 0
volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;
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

void print_char(const char c)
{
  *UART0DR = (unsigned int)c; /* Transmit char */
}

unsigned int get_time()
{
  return *RTC;
}
void print_ascii(const char *s) {
  while(*s != '\0')
    print_char(*s++);
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
  print_ascii(bufp);
  print_ascii("\n");
}

void pr_arr(int a[], int size)
{
  for (int i = 0; i < size; i++) {
    print_char(a[i]+48);
    print_char(' ');
  }
  print_char('\r');
  print_char('\n');

}

int add(int x, int y)
{
  int res = x + y;
  print_char(((char)res)+48);
  print_char('\n');
  return res;
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

void merge(int a[], int size, int m)
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

void sort(int a[], int size)
{
  if (size <= 1)
    return;
  int m = size >> 1;
  sort(a, m);
  sort(a + m, size - m);
  merge(a, size, m);
}

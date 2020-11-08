#define NULL (void *) 0
volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;

void print_char(const char c)
{
  *UART0DR = (unsigned int)c; /* Transmit char */
}

void print_ascii(const char *s) {
  while(*s != '\0')
    print_char(*s++);
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
  if (basep + size < MEMSIZE) {
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

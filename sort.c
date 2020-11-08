#define MEMSIZE 4096
char memory[MEMSIZE];
char *basep = memory;
void *kmalloc(int size)
{
  void *new = basep;
  basep += size;
  return new;
}

void kfree(void *ptr)
{
  return;
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

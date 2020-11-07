#include <stdio.h>
#include <stdlib.h>

void merge(int a[], int size, int m)
{
  char *buf = malloc(size * sizeof (int));

  for (int i = 0, left = 0, right = m; i < size; i++) {
    if (left == m)
      buf[i] = a[right++];
    else if (right == size)
      buf[i] = a[left++];
    else if (a[left] <= a[right]) {
      buf[i] = a[left++];
    }
    else {
      buf[i] = a[right++];
    }
  }

  for (int i = 0; i < size; i++)
    a[i] = buf[i];

  free(buf);
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

void pr_arr(int a[], int size)
{
  for (int i = 0; i < size; i++)
      printf("%d%s", a[i], i == size -1 ? "\n" : " ");
}

int main() {
  int a[] = {3, 2, 1, 9, 0};
  int size = sizeof a / sizeof a[0];

  printf("in  ");
  pr_arr(a, size);
  sort(a, size);
  printf("out ");
  pr_arr(a, size);
}

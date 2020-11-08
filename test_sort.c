#include <stdio.h>
#include "sort.h"

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

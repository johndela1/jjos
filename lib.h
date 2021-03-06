#include "uart_pl011.h"

void print_ascii(const char *str);
void print_char(const char *c);
void print_num(int n);
void kputs(char *buf);
void kputchar(int c);
int kgetchar();
void pr_arr(char a[], int size);
struct result {int quot; int rem;};
struct result cdivmod(int dev, int div);
long long divmod(int div, int dev);
unsigned int get_time();
int kstrlen(const char *s);
int kstrncmp(const char *s1, const char *s2, int size);
void ksort(char a[], int size);

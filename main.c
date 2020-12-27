#include "lib.h"
#include "uart_pl011.h"
#include <stdint.h>

#define UART0_BASE_ADDR 0x101f1000
#define UART0_DR (*((volatile uint32_t *)(UART0_BASE_ADDR + 0x000)))
#define UART0_IMSC (*((volatile uint32_t *)(UART0_BASE_ADDR + 0x038)))

#define VIC_BASE_ADDR 0x10140000
#define VIC_INTENABLE (*((volatile uint32_t *)(VIC_BASE_ADDR + 0x010)))


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

void __attribute__((interrupt)) irq_handler() {
  int c;
    if (c = kgetchar()) { //check if != EOF
      if (c == 0x7f || c == '\b') {
          c = '\b';
          kputchar(c);
          kputchar(' ');
          kputchar(c);
          buf_idx--;
          return;
      }
    if (c == 27) {
        return;
    }
    if (c == 0x15) {
        for (int i = 0; i < buf_idx; i++) {
            buf[i] = 0;
            kputchar('\b');
        }
        for (int i = 0; i < buf_idx; i++)
            kputchar(' ');
        for (int i = 0; i < buf_idx; i++)
            kputchar('\b');

        buf_idx = 0;
        return;
    }
    if (c == 0x17) {
        int orig_buf_idx = buf_idx;
        while(--buf_idx)
            if (buf[buf_idx] == ' ')
                break;
        for (int i = buf_idx; i < orig_buf_idx; i++) {
            buf[i] = 0;
            kputchar('\b');
        }
        for (int i = buf_idx; i < orig_buf_idx; i++)
            kputchar(' ');
        for (int i = buf_idx; i < orig_buf_idx; i++)
            kputchar('\b');
    }
      kputchar(c);
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

/* all other handlers are infinite loops */
void __attribute__((interrupt)) undef_handler(void) { for(;;); }
void __attribute__((interrupt)) swi_handler(void) { for(;;); }
void __attribute__((interrupt)) prefetch_abort_handler(void) { for(;;); }
void __attribute__((interrupt)) data_abort_handler(void) { for(;;); }
void __attribute__((interrupt)) fiq_handler(void) { for(;;); }

void copy_vectors(void) {
 extern uint32_t vectors_start;
 extern uint32_t vectors_end;
 uint32_t *vectors_src = &vectors_start;
 uint32_t *vectors_dst = (uint32_t *)0;

while(vectors_src < &vectors_end)
 *vectors_dst++ = *vectors_src++;
}

void start_kernel(void)
{
  kputs("# ");

 /* enable UART0 IRQ */
 VIC_INTENABLE = 1<<12;
 /* enable RXIM interrupt */
 UART0_IMSC = 1<<4;
 for(;;);
}

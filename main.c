#include "lib.h"
#include "uart_pl011.h"
#include <stdint.h>

#define UART0_BASE_ADDR 0x101f1000
#define UART0_DR (*((volatile uint32_t *)(UART0_BASE_ADDR + 0x000)))
#define UART0_IMSC (*((volatile uint32_t *)(UART0_BASE_ADDR + 0x038)))

#define VIC_BASE_ADDR 0x10140000
#define VIC_INTENABLE (*((volatile uint32_t *)(VIC_BASE_ADDR + 0x010)))

#define TIMER0 ((volatile unsigned int*)0x101E2000)
#define TIMER_VALUE 0x1 /* 0x04 bytes */
#define TIMER_CONTROL 0x2 /* 0x08 bytes */
#define TIMER_INTCLR 0x3 /* 0x0C bytes */
#define TIMER_MIS 0x5 /* 0x14 bytes */
#define TIMER_EN 0x80
#define TIMER_PERIODIC 0x40
#define TIMER_INTEN 0x20
#define TIMER_32BIT 0x02
#define TIMER_ONESHOT 0x01

char buf[64];
char buf_idx = 0;
unsigned long jiffies = 0;

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
  } else if (!kstrncmp("date", buf, kstrlen("date"))) {
        print_num(jiffies);
  } else
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

void timer_init()
{
     *TIMER0 = 1000000;
     *(TIMER0 + TIMER_CONTROL) = TIMER_EN | TIMER_ONESHOT | TIMER_32BIT;
     /* enable UART0 IRQ */
     VIC_INTENABLE = 1<<12;
     /* enable RXIM interrupt */
     UART0_IMSC = 1<<4;
}

void start_kernel(void)
{    timer_init();
     kputs("# ");

     while(1) {
     if(!*(TIMER0+TIMER_VALUE)) {
       jiffies++;
       *TIMER0 = 1000000;
       *(TIMER0 + TIMER_CONTROL) = TIMER_EN | TIMER_ONESHOT | TIMER_32BIT;
     }
 }
}

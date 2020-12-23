ENTRY(_Reset)
SECTIONS
{
 . = 0x10000;
 .head . : { head.o(.text) }
 .text : { *(.text) }
 .data : { *(.data) }
 .bss : { *(.bss COMMON) }
 . = ALIGN(8);
 . = . + 0x1000; /* 4kB of stack memory */
 _stack_top = .;
 . = . + 0x2000; /* 4kB of stack memory */
 fiq_stack_top = .;
 . = . + 0x3000; /* 4kB of stack memory */
 irq_stack_top = .;
}

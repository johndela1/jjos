ENTRY(vectors_start)
SECTIONS
{
 . = 0x10000;
 .text : { head.o (.vector_table) *(.text) }
 .data : { *(.data) }
 .bss : { *(.bss COMMON) }
 . = ALIGN(8);
 . = . + 0x1000; /* 4kB of stack memory */
 stack_top = .;
 . = . + 0x2000; /* 4kB of stack memory */
 irq_stack_top = .;
}

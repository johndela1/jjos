        .global _start
        .text
        .code 32

        @ Register  definition, for ARM VersatilePB with PL011 UART
        .equ    UART0_BASE, 0x101f1000
        .equ    UARTDR,     0x0
.global _Reset
_Reset:
        @ Display 'str' to the UART:
        ldr     r0, =str
        ldr     r1, =UART0_BASE
1:      ldrb    r2, [r0], #1     @ Get next character
        cmp     r2, #0            @ ... until end of string
        beq     done                 @ We are done.
        str     r2, [r1, #UARTDR] @ Print the character to UART
        b       1b                @ Loop back
done:
 LDR sp, =stack_top
 BL c_entry
 mov r2, #2
 mov r3, #3
 add r1, r2, r3

 b .

str:    .asciz  "Hello world asm!\n"

        .end

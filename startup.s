.data
array_buff:
	.word 3, 2, 1, 9, 0
.text
.code 32

@ Register  definition, for ARM VersatilePB with PL011 UART
.equ    UART0_BASE, 0x101f1000
.equ    UARTDR,     0x0

.global _Reset

_Reset:
	b start
	b .
	b . /* 0x8  Software Interrupt */
	b .
	b .
	b . /* 0x14 Reserved */
	b . /* 0x18 IRQ */
	b . /* 0x1C FIQ */

start:
	LDR sp, =stack_top
	bl kernel_main
	b .

kernel_main:
	bl init
	bl hello

	push {r11, lr}
	add r11, sp, #0
	sub sp, sp, #16
	mov r0, #3
	mov r1, #4
	bl add
	sub sp, r11, #0
	pop {r11, pc}

init:	ldr	r0, =str
	ldr	r1, =UART0_BASE
1:      ldrb    r2, [r0], #1     @ Get next character
        cmp     r2, #0            @ ... until end of string
        beq     done
        str     r2, [r1, #UARTDR] @ Print the character to UART
        b       1b
done:	bx lr

add1:
	push {r11}
	add r11, sp, #0
	sub sp, sp, #12
a	add r0, r1
	add sp, r11, #0
	pop {r11}
	bx lr

str:    .asciz  "jjos initialized\n"

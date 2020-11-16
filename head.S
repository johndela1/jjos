.data
array_buf:
	.word 3, 2, 1, 9, 0
.text
.code 32

@ Register  definition, for ARM VersatilePB with PL011 UART
.equ    UART0_BASE, 0x101f1000
.equ    UARTDR,     0x0
.equ	RTC, 	    0x101E8000
.global _Reset
.global divmod
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
	bl start_kernel
        b .
	ldr r0, array_buf_bridge
	mov r1, #5
	bl pr_arr

	ldr r0, array_buf_bridge
	mov r1, #5
	bl sort

	ldr r0, array_buf_bridge
	mov r1, #5
	bl pr_arr

divmod:
    @ r0 dividend, r1 devisor, r2 quotient
    mov r2, #0
    b b2
b1:
    add r2, #1
    sub r0, r1
b2:
    cmp r0, r1
	bge b1
	mov r1, r2
    bx lr

array_buf_bridge:
	.word array_buf

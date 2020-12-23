.equ MODE_FIQ, 0x11
.equ MODE_IRQ, 0x12
.equ MODE_SVC, 0x13

.data
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
	msr cpsr_c, MODE_FIQ
	ldr sp, =fiq_stack_top
	msr cpsr_c, MODE_IRQ
	ldr sp, =irq_stack_top
	msr cpsr_c, MODE_SVC
	ldr sp, =_stack_top

	bl kernel_main
	b .

kernel_main:
	bl start_kernel
	b .

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

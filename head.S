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
	bl test_uart
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

	push {r11, lr}
	add r11, sp, #0
	sub sp, sp, #16
	mov r0, #3
	mov r1, #4
	bl add
	sub sp, r11, #0
	pop {r11, pc}

test_uart:	ldr	r0, =str
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
	add r0, r1
	add sp, r11, #0
	pop {r11}
	bx lr

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

str:    .asciz  "uart: success\n"
array_buf_bridge:
	.word array_buf

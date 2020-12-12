CFLAGS = -g
CC=gcc
export PATH := $PWD/../gcc-arm-none-eabi-9-2020-q2-update/bin:$(PATH)
PRE = ../gcc-arm-none-eabi-9-2020-q2-update/bin/arm-none-eabi-

jjos.bin: head.o lib.o main.o uart_pl011.o
	$(PRE)ld -T test.ld lib.o main.o head.o uart_pl011.o -o jjos.elf
	$(PRE)objcopy -O binary jjos.elf jjos.bin

head.o: head.S
	$(PRE)$(CC) -c $(CFLAGS) -mcpu=arm926ej-s head.S -o head.o

lib.o: lib.c lib.h
	$(PRE)$(CC) -c $(CFLAGS) -mcpu=arm926ej-s lib.c -o lib.o

main.o: main.c
	$(PRE)$(CC) -c $(CFLAGS) -mcpu=arm926ej-s main.c -o main.o
run: jjos.bin
	qemu-system-arm -M versatilepb -m 128M -nographic -kernel jjos.bin

uart_pl011.o: uart_pl011.c
	$(PRE)$(CC) -c $(CFLAGS) -mcpu=arm926ej-s uart_pl011.c -o uart_pl011.o
TAGS: FORCE
	find . -type f | etags -
clean:
	@rm -f *.out *.o *.elf *.bin
	@rm -rf *.dSYM

FORCE:

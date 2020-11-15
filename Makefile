CFLAGS = -g
CC=gcc
export PATH := $PWD/../gcc-arm-none-eabi-9-2020-q2-update/bin:$(PATH)
PRE = ../gcc-arm-none-eabi-9-2020-q2-update/bin/arm-none-eabi-

kernel.bin: head.o lib.o main.o
	$(PRE)ld -T test.ld lib.o main.o head.o -o kernel.elf
	$(PRE)objcopy -O binary kernel.elf kernel.bin

head.o: head.S
	$(PRE)$(CC) -c $(CFLAGS) -mcpu=arm926ej-s head.S -o head.o

lib.o: lib.c lib.h
	$(PRE)$(CC) -c $(CFLAGS) -mcpu=arm926ej-s lib.c -o lib.o

main.o: main.c
	$(PRE)$(CC) -c $(CFLAGS) -mcpu=arm926ej-s main.c -o main.o
run: kernel.bin
	qemu-system-arm -M versatilepb -m 128M -nographic -kernel kernel.bin

clean:
	@rm -f *.out *.o *.elf *.bin
	@rm -rf *.dSYM

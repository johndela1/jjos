CFLAGS = -g
CC=gcc
PRE = arm-none-eabi-
export PATH := $PWD/../gcc-arm-none-eabi-9-2020-q2-update/bin:$(PATH)

kernel.bin: startup.o lib.o
	$(PRE)ld -T test.ld lib.o startup.o -o kernel.elf
	$(PRE)objcopy -O binary kernel.elf kernel.bin

startup.o: startup.s
	$(PRE)$(CC) -c $(CFLAGS) -mcpu=arm926ej-s startup.s -o startup.o

lib.o: lib.c
	$(PRE)$(CC) -c $(CFLAGS) -mcpu=arm926ej-s lib.c -o lib.o

run: kernel.bin
	qemu-system-arm -M versatilepb -m 128M -nographic -kernel kernel.bin

test_sort: sort.h sort.c test_sort.c
	cc sort.c test_sort.c -I. -o test_sort

clean:
	@rm -f *.out *.o *.elf *.bin
	@rm -rf *.dSYM

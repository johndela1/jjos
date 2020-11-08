PATH=$PATH:$PWD/../gcc-arm-none-eabi-9-2020-q2-update/bin
arm-none-eabi-gcc -c -mcpu=arm926ej-s -g startup.s -o startup.o
arm-none-eabi-gcc -c -mcpu=arm926ej-s -g test.c -o test.o
arm-none-eabi-gcc -c -mcpu=arm926ej-s -g sort.c -o sort.o
arm-none-eabi-ld -T test.ld sort.o test.o startup.o -o test.elf
arm-none-eabi-objcopy -O binary test.elf test.bin

qemu-system-arm -M versatilepb -m 128M -nographic -kernel test.bin

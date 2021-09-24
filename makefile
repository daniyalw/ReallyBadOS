all:
	nasm -f elf32 src/boot_sector.asm -o run/boot.o
	i686-elf-g++ src/kernel.cpp run/boot.o -o run/kernel.bin -nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -T src/linker.ld
	qemu-system-x86_64 run/kernel.bin

run:
	qemu-system-x86_64 run/kernel.bin

build:
	nasm -f elf32 src/boot_sector.asm -o run/boot.o
	i686-elf-g++ src/kernel.cpp run/boot.o -o run/kernel.bin -nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -T src/linker.ld

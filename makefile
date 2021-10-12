all:
	nasm -f elf32 src/boot_sector.asm -o run/boot.o
	i686-elf-g++ src/kernel.cpp run/boot.o -o run/ceneos-x86_64.bin -nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -T src/linker.ld
	qemu-system-i386  run/ceneos-x86_64.bin

r:
	qemu-system-i386  run/ceneos-x86_64.bin

build:
	nasm -f elf32 src/boot_sector.asm -o run/boot.o
	i686-elf-g++ src/kernel.cpp run/boot.o -o run/ceneos-x86_64.bin -nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -T src/linker.ld

length:
	python3 check_src_lines.py

clean:
	rm run/boot.o
	rm run/ceneos-x86_64.bin

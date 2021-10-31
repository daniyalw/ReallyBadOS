boot = boot/boot.asm

build:
	nasm -f elf32 -o built/loader.o src/${boot}
	i686-elf-g++ -m32 -nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra  src/kernel.cpp built/loader.o -o built/main.elf -T src/linker.ld

r:
	qemu-system-x86_64 -cdrom main.iso

all:
	nasm -f elf32 -o built/loader.o src/${boot}
	nasm -f elf32 -o built/gdt.o src/sys/descriptors/gdt.asm
	nasm -f elf32 src/sys/interrupts/exception_handler.asm -o built/int.o
	i686-elf-g++ -m32 -nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra  built/loader.o src/kernel.cpp built/gdt.o built/int.o -o built/main.elf -T src/linker.ld
	cp built/main.elf isodir/boot/main.elf
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o main.iso isodir
	qemu-system-x86_64 -cdrom main.iso -soundhw pcspk

clean:
	rm built/*
	rm isodir/*.bin
	rm isodir/*.cfg
	rm main.iso

iso:
	cp built/main.elf isodir/boot/main.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o main.iso isodir

# graphics
test:
	i686-elf-as -o built/loader.o src/boot/loader.s
	nasm -f elf32 -o built/gdt.o src/sys/descriptors/gdt.asm
	nasm -f elf32 src/sys/interrupts/exception_handler.asm -o built/int.o
	i686-elf-g++ -m32 -nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra  built/loader.o src/kernel.cpp built/gdt.o built/int.o -o built/main.elf -T src/linker.ld
	cp built/main.elf isodir/boot/main.elf
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o main.iso isodir
	qemu-system-x86_64 -cdrom main.iso -soundhw pcspk

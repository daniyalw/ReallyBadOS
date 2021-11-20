boot = boot/boot.asm

r:
	qemu-system-x86_64 -cdrom main.iso

all:
	nasm -f elf32 -o built/loader.o src/${boot}
	nasm -f elf32 -o built/gdt.o src/kernel/sys/descriptors/gdt.asm
	nasm -f elf32 src/kernel/sys/interrupts/exception_handler.asm -o built/int.o
	i686-elf-g++ -m32 -nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib  built/loader.o src/kernel/kernel.cpp built/gdt.o built/int.o -o built/main.o -T src/linker.ld
	cp built/main.o isodir/boot/main.o
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o ceneos-x86_64.iso isodir
	qemu-system-x86_64 -cdrom ceneos-x86_64.iso -soundhw pcspk -rtc base=localtime

clean:
	rm *.iso
	rm *.o
	rm built/*.o
	rm isodir/*.cfg

iso:
	cp built/main.elf isodir/boot/main.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o main.iso isodir

# graphics
test:
	i686-elf-as -o built/loader.o src/boot/loader.s
	nasm -f elf32 -o built/gdt.o src/kernel/sys/descriptors/gdt.asm
	nasm -f elf32 src/kernel/sys/interrupts/exception_handler.asm -o built/int.o
	i686-elf-g++ -m32 -nostdlib -ffreestanding -std=c++11 -mno-red-zone -Wno-error built/loader.o src/kernel/kernel.cpp built/gdt.o built/int.o -o built/main.o -T src/linker.ld
	cp built/main.o isodir/boot/main.o
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o ceneos-x86_64.iso isodir
	qemu-system-x86_64 -cdrom ceneos-x86_64.iso -soundhw pcspk

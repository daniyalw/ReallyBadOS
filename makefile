boot = boot/boot.asm

r:
	qemu-system-x86_64 -cdrom ceneos-x86_64.iso -soundhw pcspk -rtc base=localtime

all:
	nasm -f elf32 -o built/loader.o ${boot}
	nasm -f elf32 -o built/gdt.o kernel/sys/descriptors/gdt.asm
	nasm -f elf32 kernel/sys/interrupts/exception_handler.asm -o built/int.o
	i686-elf-g++ -Iinclude -m32 -ffreestanding -Wno-write-strings -std=c++14 -mno-red-zone -nostdlib built/loader.o kernel/kernel.cpp built/gdt.o built/int.o -o built/main.o -T linker.ld
	cp built/main.o isodir/boot/main.o
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o ceneos-x86_64.iso isodir
	qemu-system-x86_64 -cdrom ceneos-x86_64.iso -soundhw pcspk -rtc base=localtime -m 15M -serial stdio

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
	i686-elf-as -o built/loader.o boot/loader.s
	nasm -f elf32 -o built/gdt.o kernel/sys/descriptors/gdt.asm
	nasm -f elf32 kernel/sys/interrupts/exception_handler.asm -o built/int.o
	i686-elf-g++ -m32 -Iinclude -nostdlib -ffreestanding -Wno-write-strings -std=c++14 -mno-red-zone built/loader.o kernel/kernel.cpp built/gdt.o built/int.o -o built/main.o -T linker.ld
	cp built/main.o isodir/boot/main.o
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o ceneos-x86_64.iso isodir
	qemu-system-x86_64 -cdrom ceneos-x86_64.iso -soundhw pcspk -m 15M -serial stdio -rtc base=localtime

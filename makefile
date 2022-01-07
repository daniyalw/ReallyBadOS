boot = boot/loader.s

r:

	qemu-system-x86_64 -cdrom ceneos-x86_32.iso -soundhw pcspk -m 100M -serial stdio -rtc base=localtime

bootloader:
	i686-elf-as -o built/loader.o ${boot}

gdt:
	nasm -f elf32 -o built/gdt.o kernel/sys/descriptors/gdt.asm

interrupts:
	nasm -f elf32 kernel/sys/interrupts/exception_handler.asm -o built/int.o

jmp:
	nasm -f elf32 kernel/jmp.asm -o built/jmp.o

usermode:
	nasm -f elf32 kernel/sys/descriptors/usermode.asm -o built/user.o

textmode:
	make bootloader
	nasm -f elf32 kernel/sys/descriptors/tss.asm -o built/tss.o
	i686-elf-g++ -Iinclude -m32 -ffreestanding -Wno-write-strings -std=c++20 -mno-red-zone -nostdlib built/loader.o built/jmp.o kernel/kernel.cpp built/user.o built/int.o built/gdt.o built/tss.o -o built/main.o -T linker.ld
	cp built/main.o isodir/boot/main.o
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o ceneos-x86_32.iso isodir
	qemu-system-x86_64 -cdrom ceneos-x86_32.iso -soundhw pcspk -m 100M -serial stdio -rtc base=localtime

clean:
	rm *.iso
	rm *.o
	rm built/*.o
	rm isodir/*.cfg

iso:
	cp built/main.elf isodir/boot/main.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o ceneos-x86_32.iso isodir

# graphics
graphics:
	i686-elf-as -o built/loader.o boot/graphics_boot.asm
	nasm -f elf32 -o built/gdt.o kernel/sys/descriptors/gdt.asm
	nasm -f elf32 kernel/sys/interrupts/exception_handler.asm -o built/int.o
	i686-elf-g++ -m32 -Iinclude -nostdlib -ffreestanding -Wno-write-strings -std=c++20 -mno-red-zone built/loader.o kernel/kernel.cpp built/user.o built/gdt.o built/int.o built/tss.o -o built/main.o -T linker.ld
	cp built/main.o isodir/boot/main.o
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o ceneos-x86_32.iso isodir
	qemu-system-x86_64 -cdrom ceneos-x86_32.iso -soundhw pcspk -m 100M -serial stdio -rtc base=localtime

boot = boot/loader.s
INCLUDES = -Istdlib -Ifs -Idrivers -Ikernel -I.
COMPILER_FLAGS = -m32 -nostdlib -ffreestanding -Wno-write-strings -std=c++20 -mno-red-zone -fpermissive
QEMU_FLAGS = -soundhw pcspk -m 1000M -serial stdio -rtc base=localtime -drive format=raw,file=out.img,index=0,media=disk,id=nvm -accel tcg -net nic,model=rtl8139 -net user -vga std -boot d
OUT = ceneos-x86_32.iso

run:

	qemu-system-x86_64 -cdrom ceneos-x86_32.iso ${QEMU_FLAGS}

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

tss:
	nasm -f elf32 kernel/sys/descriptors/tss.asm -o built/tss.o

kernel:
	make gdt
	make interrupts
	make jmp
	make usermode
	make tss

textmode:
	make bootloader
	i686-elf-g++ ${COMPILER_FLAGS} ${INCLUDES} built/loader.o built/jmp.o kernel/kernel.cpp built/user.o built/int.o built/gdt.o built/tss.o -o built/main.o -T linker.ld
	make iso
	make run

clean:
	rm *.iso
	rm *.o
	rm built/*.o
	rm isodir/*.cfg

iso:
	cp built/main.o isodir/boot/main.o
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o ${OUT} isodir

# graphics
graphics:
	i686-elf-as -o built/loader.o boot/graphics_boot.asm
	i686-elf-g++ ${COMPILER_FLAGS} ${INCLUDES} built/loader.o built/jmp.o kernel/kernel.cpp built/user.o built/gdt.o built/int.o built/tss.o -o built/main.o -T linker.ld
	cp built/main.o isodir/boot/main.o
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o ${OUT} isodir
	make run

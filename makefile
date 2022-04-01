boot = boot/loader.s
INCLUDES = -Istdlib -Ifilesystem -Idrivers -Ikernel -I.
COMPILER_FLAGS = -m32 -nostdlib -ffreestanding -Wno-write-strings -std=c++20 -mno-red-zone -fpermissive -lgcc -fno-rtti
QEMU_FLAGS = -soundhw pcspk -m 1G -serial stdio -rtc base=localtime -drive format=raw,file=out.img,index=0,media=disk,id=nvm \
			 -accel tcg -net nic,model=rtl8139 -net user -boot d -device bochs-display -device virtio-serial-pci
OUT = reallybados-x86_32.iso
QEMU = qemu-system-x86_64
ARCH = i686-elf

AS = ${ARCH}-as
GCC = ${ARCH}-g++
NM = ${ARCH}-nm

all:
	make gdt
	make interrupts
	make jmp
	make tss
	make tasking
	make user
	make textmode
	make map
	make ramdisk
	make iso
	make run

run:
	${QEMU} -cdrom reallybados-x86_32.iso ${QEMU_FLAGS}

bootloader:
	${AS} -o built/loader.o ${boot}

gdt:
	nasm -f elf32 -o built/gdt.o kernel/sys/descriptors/gdt.asm

interrupts:
	nasm -f elf32 kernel/sys/descriptors/exception_handler.asm -o built/int.o

jmp:
	nasm -f elf32 kernel/jmp.asm -o built/jmp.o

map:
	${NM} -C -n built/main.o > initrd/kernel.map

tasking:
	nasm -f elf32 kernel/sys/multitasking/switch.asm -o built/switch.o
	${AS} kernel/sys/multitasking/eip.asm -o built/eip.o

tss:
	nasm -f elf32 kernel/sys/descriptors/tss.asm -o built/tss.o

ramdisk:
	cd initrd && make build && cd ..

user:
	make -C usr/src
	make -C usr/apps/apps/write
	make -C usr/apps/apps/read
	make -C usr/apps/apps/echo
	make -C usr/apps/apps/exec
	make -C usr/apps/apps/info
	make -C usr/apps/apps/ls
	make -C usr/apps/apps/mkdir
	make -C usr/apps/apps/mkfile
	make -C usr/apps/apps/test
	make -C usr/apps/apps/time

textmode:
	make bootloader
	make build-cpp

clean:
	rm *.iso
	rm built/*.o
	rm initrd/kernel.map
	rm isodir/boot/grub/*.cfg
	rm usr/src/*.o
	rm initrd/*.o
	rm isodir/boot/main.o
	rm isodir/boot/out.tar

iso:
	cp built/main.o isodir/boot/main.o
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o ${OUT} isodir

build-cpp:
	${GCC} ${COMPILER_FLAGS} ${INCLUDES} built/loader.o built/jmp.o kernel/kernel.cpp built/int.o built/gdt.o built/tss.o built/switch.o built/eip.o -o built/main.o -T linker.ld

# graphics
graphics:
	${AS} -o built/loader.o boot/graphics_boot.asm
	make build-cpp
	make iso
	make run

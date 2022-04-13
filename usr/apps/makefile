CPP = i686-elf-g++
CPP_FLAGS = -m32 -ffreestanding -I../../../include -Wno-write-strings -std=c++20 -mno-red-zone -nostdlib -fconcepts-ts -T ../../link.ld
FOLDER = base

EXTERNAL := $(wildcard ../../../src/*.o) \
			../../start.o

LINKER = i686-elf-ld

#use this to compile start.cpp
#i686-elf-g++ -m32 -ffreestanding -I../include -Wno-write-strings -std=c++20 -mno-red-zone -nostdlib -fconcepts-ts -c start.cpp -o start.o

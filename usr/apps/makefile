CPP = i686-elf-g++
CPP_FLAGS = -m32 -ffreestanding -I../include -Wno-write-strings -std=c++20 -mno-red-zone -nostdlib -fconcepts-ts

CPP_SOURCES = $(wildcard *.cpp)
OBJ = ${CPP_SOURCES:.cpp=.o}
EXTERNAL := ../src/itoa.o \
			../src/print.o \
			../src/time.o \
			../src/length.o

LINKER = i686-elf-ld

out: ${OBJ}
	echo Done!

%.o: %.cpp
	cp ../src/start.o start.o
	${CPP} ${CPP_FLAGS} $< ${EXTERNAL} -o obj/$@ -T link.ld
	cp obj/$@ ../../isodir/boot/test.o

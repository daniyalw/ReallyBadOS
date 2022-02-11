CPP_SOURCES = ${wildcard *.cpp}
OBJ = ${CPP_SOURCES:.cpp=.o}

CPP = i686-elf-g++
CPP_FLAGS = -m32 -ffreestanding -I../include -Wno-write-strings -std=c++14 -mno-red-zone -nostdlib -fconcepts-ts

all: ${OBJ}
	echo Done!

%.o: %.cpp
	${CPP} ${CPP_FLAGS} -c $< -o $@

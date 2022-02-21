FILES := read.o echo.o hello.txt time.o info.o Vera.sfn test.r ls.o mkdir.o mkfile.o write.o \

OUT = out.tar

build:
	tar -cf out.tar ${FILES}
	make copy-to-iso

copy-to-iso:
	cp ${OUT} ../isodir/boot/out.tar

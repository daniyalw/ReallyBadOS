FILES := read.o echo.o hello.txt time.o info.o Vera.sfn exec.o test.r ls.o test.o mkdir.o mkfile.o write.o append.o help.txt
OUT = out.tar

build:
	tar -cf out.tar ${FILES}
	make copy-to-iso

copy-to-iso:
	cp ${OUT} ../isodir/boot/out.tar

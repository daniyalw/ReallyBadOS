FILES := test.o echo.o hello.txt time.o info.o
OUT = out.tar

build:
	tar -cf out.tar ${FILES}
	make copy-to-iso

copy-to-iso:
	cp ${OUT} ../isodir/boot/out.tar

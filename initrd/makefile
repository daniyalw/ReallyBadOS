FILES := test.cos echo.cos hello.txt
OUT = out.tar

build:
	tar -cf out.tar ${FILES}
	make copy-to-iso

copy-to-iso:
	cp ${OUT} ../isodir/boot/out.tar

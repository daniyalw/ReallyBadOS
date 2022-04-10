FILES := bin home usr

OUT = out.tar

build:
	tar -cf ${OUT} ${FILES}
	make copy-to-iso

copy-to-iso:
	cp ${OUT} ../isodir/boot/out.tar

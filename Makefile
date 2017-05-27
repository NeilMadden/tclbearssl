CFLAGS=-DUSE_TCL_STUBS -W -Wall -Os -fPIC -Isrc -I/usr/local/include
LDFLAGS=-L/Library/Frameworks/Tcl.framework -L/usr/local/lib

INSTALL = /usr/local/opt/tcl-tk/lib/bearssl0.4.0

OUT = build

all: libtclbearssl.dylib

mkdirs:
	mkdir -p $(OUT)

clean:
	rm -rf $(OUT)

hash.o: src/hash.c src/hash.h mkdirs
	gcc $(CFLAGS) -c -o $(OUT)/hash.o src/hash.c

libtclbearssl.dylib: hash.o src/tclbear.c mkdirs
	gcc -dynamiclib $(CFLAGS) $(LDFLAGS) src/tclbear.c $(OUT)/hash.o -ltclstub8.6 -lbearssl -o $(OUT)/libtclbearssl.dylib

rebuild: clean all

install: libtclbearssl.dylib
	mkdir -p $(INSTALL)
	cp $(OUT)/libtclbearssl.dylib $(INSTALL)
	cp pkgIndex.tcl $(INSTALL)

.PHONY: all clean rebuild mkdirs install

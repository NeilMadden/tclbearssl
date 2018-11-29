BEARDIR=../BearSSL
CFLAGS=-DUSE_TCL_STUBS -W -Wall -Os -fPIC -Isrc -I$(BEARDIR)/inc
LDFLAGS=-L/Library/Frameworks/Tcl.framework -L/usr/local/lib

INSTALL = /usr/local/opt/tcl-tk/lib/bearssl0.4.0

OUT = build

all: libtclbearssl.dylib

mkdirs:
	mkdir -p $(OUT)

clean:
	rm -rf $(OUT)
	rm -f libtclbearssl.dylib

hash.o: src/hash.c src/hash.h mkdirs
	gcc $(CFLAGS) -c -o $(OUT)/hash.o src/hash.c

rand.o: src/rand.c src/rand.h mkdirs
	gcc $(CFLAGS) -c -o $(OUT)/rand.o src/rand.c

libtclbearssl.dylib: hash.o rand.o src/tclbear.c mkdirs
	gcc -dynamiclib $(CFLAGS) $(LDFLAGS) src/tclbear.c $(OUT)/hash.o $(OUT)/rand.o $(BEARDIR)/build/libbearssl.a -ltclstub8.6 -o ./libtclbearssl.dylib

rebuild: clean all

install: libtclbearssl.dylib
	mkdir -p $(INSTALL)
	cp $(OUT)/libtclbearssl.dylib $(INSTALL)
	cp pkgIndex.tcl $(INSTALL)

.PHONY: all clean rebuild mkdirs install

CFLAGS=-DUSE_TCL_STUBS -W -Wall -Os -fPIC -Isrc

all: libtclbearssl.dylib

clean:
	rm -f *.o *.dylib

hash.o: src/hash.c src/hash.h
	gcc $(CFLAGS) -c -o hash.o src/hash.c

libtclbearssl.dylib: hash.o
	gcc -dynamiclib $(CFLAGS) src/tclbear.c hash.o -L/Library/Frameworks/Tcl.framework -ltclstub8.6 -lbearssl -o libtclbearssl.dylib

.PHONY: all clean

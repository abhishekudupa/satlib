CC=gcc
CFLAGS=-Wall -O3 -fPIC -flto -DNDBLSCR -DNLGLOG -DNDEBUG -DNCHKSOL -DNLGLPICOSAT -Wno-unused-function

default: liblgl-aqw.a liblgl-aqw.so

liblgl-aqw.a: lglib.o Makefile
	ar rc $@ lglib.o
	ranlib $@

liblgl-aqw.so: lglib.o Makefile
	$(CC) -shared $(CFLAGS) -o $@ lglib.o

lglib.o: lglib.c lglib.h Makefile
	$(CC) $(CFLAGS) -c lglib.c

.PHONY:	clean

clean:
	rm -f liblgl-aqw.a lglib.o liblgl-aqw.so

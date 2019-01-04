USE_PHP=1
USE_ZLIB=0
BASEDIR=.
DIFFERENT=0

CC=gcc
LDFLAGS=-O2

ifeq ($(USE_ZLIB), 1)
	LDFLAGS+=-lz
endif

jankyserv: error.o images.o text.o file.o misc.o respond.o main.o
	$(CC) -o $@ $^ $(LDFLAGS)

misc.o:
	$(CC) -o $@ -c src/misc.c

respond.o: error.o images.o text.o file.o
	$(CC) -o $@ -c src/respond.c -D USE_PHP=$(USE_PHP)

main.o: respond.o
	$(CC) -o $@ -c src/main.c -D BASEDIR=$(BASEDIR) -D DIFFERENT=$(DIFFERENT)

error.o:
	$(CC) -o $@ -c src/workers/error.c

images.o:
	$(CC) -o $@ -c src/workers/images.c

text.o:
	$(CC) -o $@ -c src/workers/text.c

file.o:
	$(CC) -o $@ -c src/workers/file.c

.PHONY: clean
clean:
	rm -f *.o

.PHONY: distclean
distclean: clean
	rm -f jankyserv

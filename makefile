#
CC=x86_64-ucb-akaros-gcc
CFLAGS=-Wall -Werror -Ofast -I. -D_BSD_SOURCE
LD=x86_64-ucb-akaros-gcc -static
LDFLAGS=
LDTAIL= -liplib

OFILES=\
	authnone.o\
	authp9any.o\
	des.o\
	dirmodeconv.o\
	fcallconv.o\
	random.o\
	readn.o\
	remotehost.o\
	strecpy.o\
	tokenize.o\
	u9fs.o


HFILES=\
	fcall.h\
	plan9.h

u9fs: $(OFILES)
	$(LD) $(LDFLAGS) -o u9fs $(OFILES) $(LDTAIL)

%.o: %.c $(HFILES)
	$(CC) $(CFLAGS) -c $*.c

clean:
	rm -f *.o u9fs

install: u9fs
	cp u9fs ../../bin

.PHONY: clean install

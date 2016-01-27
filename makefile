#
CC=x86_64-ucb-akaros-gcc
CFLAGS=-Wall -Werror -Ofast -I. -D_BSD_SOURCE
LD=x86_64-ucb-akaros-gcc -static
LDFLAGS=
LDTAIL= -liplib

OFILES=\
	authnone.o\
	authp9any.o\
	convD2M.o\
	convM2D.o\
	convS2M.o\
	des.o\
	dirmodeconv.o\
	doprint.o\
	fcallconv.o\
	print.o\
	random.o\
	readn.o\
	remotehost.o\
	rune.o\
	safecpy.o\
	strecpy.o\
	tokenize.o\
	u9fs.o\
	convM2S.o\
	utfrune.o


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

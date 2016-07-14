#include <sys/types.h>

#include <fcall.h>
#include <fcntl.h>
#include <inttypes.h>		/* for int64_t et al. */
#include <stdlib.h>
#include <stdarg.h>		/* for va_list, vararg macros */
#include <string.h>		/* for memmove */
#include <unistd.h>		/* for write */

typedef uint16_t Rune;

#define nil ((void*)0)
#define	nelem(x)	(sizeof(x)/sizeof((x)[0]))

extern char *argv0;
#define	ARGBEGIN	for((void)(argv0||(argv0=*argv)),argv++,argc--;\
			    argv[0] && argv[0][0]=='-' && argv[0][1];\
			    argc--, argv++) {\
				char *_args, *_argt;\
				Rune _argc;\
				_args = &argv[0][1];\
				if(_args[0]=='-' && _args[1]==0){\
					argc--; argv++; break;\
				}\
				_argc = 0;\
				while(*_args && (_argc = *_args, _args++))\
				switch(_argc)
#define	ARGEND		SET(_argt);USED(_argt);USED(_argc);USED(_args);}\
					USED(argv);USED(argc);
#define	ARGF()		(_argt=_args, _args="",\
				(*_argt? _argt: argv[1]? (argc--, *++argv): 0))
#define	EARGF(x)		(_argt=_args, _args="",\
				(*_argt? _argt: argv[1]? (argc--, *++argv): ((x), abort(), (char*)0)))

#define	ARGC()		_argc

#define	SET(x)	(x) = 0
#define	USED(x)	(void)(x)

extern	char*	strecpy(char*, char*, char*);
extern	int	tokenize(char*, char**, int);
extern	int	getfields(char*, char**, int, int, char*);

/*
 * print routines
 */
typedef	struct	Fconv	Fconv;
struct	Fconv
{
	char*	out;		/* pointer to next output */
	char*	eout;		/* pointer to end */
	int	f1;
	int	f2;
	int	f3;
	int	chr;
};

#define	OREAD	0	/* open for read */
#define	OWRITE	1	/* write */
#define	ORDWR	2	/* read and write */
#define	OEXEC	3	/* execute, == read but check execute permission */
#define	OTRUNC	16	/* or'ed in (except for exec), truncate file first */
#define	OCEXEC	32	/* or'ed in, close on exec */
#define	ORCLOSE	64	/* or'ed in, remove on close */
#define	OEXCL	0x1000	/* or'ed in, exclusive use */

/* bits in Qid.type */
#define QTDIR		0x80		/* type bit for directories */
#define QTAPPEND	0x40		/* type bit for append only files */
#define QTEXCL		0x20		/* type bit for exclusive use files */
#define QTMOUNT		0x10		/* type bit for mounted channel */
#define QTAUTH		0x08
#define QTFILE		0x00		/* plain file */

/* bits in Dir.mode */
#define DMDIR		0x80000000	/* mode bit for directories */
#define DMAPPEND	0x40000000	/* mode bit for append only files */
#define DMEXCL		0x20000000	/* mode bit for exclusive use files */
#define DMMOUNT		0x10000000	/* mode bit for mounted channel */
#define DMREAD		0x4		/* mode bit for read permission */
#define DMWRITE		0x2		/* mode bit for write permission */
#define DMEXEC		0x1		/* mode bit for execute permission */

long readn(int, void*, long);
void remotehost(char*, int);

enum {
	NAMELEN = 28,
	ERRLEN = 64
};

/* DES */
#define DESKEYLEN 7
void	key_setup(char key[DESKEYLEN], char expandedkey[128]);
void	block_cipher(char expandedkey[128], char buf[8], int decrypting);

void	sysfatal(char *fmt, ...);
char	*utfrune(char *s, long c);
void	getremotehostname(char *name, int nname);

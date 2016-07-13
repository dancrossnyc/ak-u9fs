#define	VERSION9P	"9P2000"
#define	MAXWELEM	16

typedef
struct	Fcall
{
	uint8_t	type;
	uint32_t fid;
	uint16_t tag;

	uint32_t msize;		/* Tversion, Rversion */
	char	*version;	/* Tversion, Rversion */

	uint32_t oldtag;	/* Tflush */

	char	*ename;		/* Rerror */

	Qid	qid;		/* Rattach, Ropen, Rcreate */
	uint32_t iounit;		/* Ropen, Rcreate */

	char	*uname;		/* Tattach, Tauth */
	char	*aname;		/* Tattach, Tauth */


	uint32_t perm;		/* Tcreate */ 
	char	*name;		/* Tcreate */
	uint8_t	mode;		/* Tcreate, Topen */

	uint32_t newfid;	/* Twalk */
	uint16_t nwname;	/* Twalk */
	char	*wname[MAXWELEM];	/* Twalk */

	uint16_t nwqid;		/* Rwalk */
	Qid	wqid[MAXWELEM];	/* Rwalk */

	int64_t	offset;		/* Tread, Twrite */
	uint32_t count;		/* Tread, Twrite, Rread */
	char	*data;		/* Twrite, Rread */

	uint16_t nstat;		/* Twstat, Rstat */
	uint8_t	*stat;		/* Twstat, Rstat */

	uint32_t afid;		/* Tauth, Tattach */
	Qid aqid;		/* Rauth */
} Fcall;


#define	GBIT8(p)	((p)[0])
#define	GBIT16(p)	((p)[0]|((p)[1]<<8))
#define	GBIT32(p)	((p)[0]|((p)[1]<<8)|((p)[2]<<16)|((p)[3]<<24))
#define	GBIT64(p)	((uint64_t)((p)[0]|((p)[1]<<8)|((p)[2]<<16)|((p)[3]<<24)) |\
				((int64_t)((p)[4]|((p)[5]<<8)|((p)[6]<<16)|((p)[7]<<24)) << 32))

#define	PBIT8(p,v)	(p)[0]=(v)
#define	PBIT16(p,v)	(p)[0]=(v);(p)[1]=(v)>>8
#define	PBIT32(p,v)	(p)[0]=(v);(p)[1]=(v)>>8;(p)[2]=(v)>>16;(p)[3]=(v)>>24
#define	PBIT64(p,v)	(p)[0]=(v);(p)[1]=(v)>>8;(p)[2]=(v)>>16;(p)[3]=(v)>>24;\
			(p)[4]=(v)>>32;(p)[5]=(v)>>40;(p)[6]=(v)>>48;(p)[7]=(v)>>56

#define	BIT8SZ		1
#define	BIT16SZ		2
#define	BIT32SZ		4
#define	BIT64SZ		8
#define	QIDSZ	(BIT8SZ+BIT32SZ+BIT64SZ)

/* STATFIXLEN includes leading 16-bit count */
/* The count, however, excludes itself; total size is BIT16SZ+count */
#define STATFIXLEN	(BIT16SZ+QIDSZ+5*BIT16SZ+4*BIT32SZ+1*BIT64SZ)	/* amount of fixed length data in a stat buffer */

#define	MAXMSG		10000	/* max header sans data */
#define	NOTAG		~0U	/* Dummy tag */
#define	IOHDRSZ		24	/* ample room for Twrite/Rread header (iounit) */

enum
{
	Tversion =	100,
	Rversion,
	Tauth =		102,
	Rauth,
	Tattach =	104,
	Rattach,
	Terror =	106,	/* illegal */
	Rerror,
	Tflush =	108,
	Rflush,
	Twalk =		110,
	Rwalk,
	Topen =		112,
	Ropen,
	Tcreate =	114,
	Rcreate,
	Tread =		116,
	Rread,
	Twrite =	118,
	Rwrite,
	Tclunk =	120,
	Rclunk,
	Tremove =	122,
	Rremove,
	Tstat =		124,
	Rstat,
	Twstat =	126,
	Rwstat,
	Tmax
};

size_t	u9convM2S(uint8_t*, size_t, Fcall*);
size_t	u9convS2M(Fcall*, uint8_t*, size_t);

int	u9statcheck(uint8_t *abuf, size_t);
size_t	u9convM2D(uint8_t*, size_t, Dir*, char*);
size_t	u9convD2M(Dir*, uint8_t*, size_t);
size_t	u9sizeD2M(Dir*);

int	fcallconv(va_list*, Fconv*);
int	dirconv(va_list*, Fconv*);
int	dirmodeconv(va_list*, Fconv*);

int	read9pmsg(int, void*, size_t);

enum {
	NOFID = 0xFFFFFFFF,
};

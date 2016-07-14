#include <plan9.h>
#include <u9fcall.h>

#include <stdio.h>
#include <string.h>
#include <printf.h>

static uint dumpsome(char*, char*, long);
static void fdirconv(char*, size_t, Dir*);
static char *qidtype(char*, uint8_t);

#define	QIDFMT	"(%.16"PRIu64"x %"PRIu32" %s)"

static int
fcallconv(FILE *stream, const struct printf_info *info, const void *const *args)
{
	const Fcall *f;
	int fid, type, tag, n, i;
	char buf[512], tmp[16];
	const Qid *q;

	f  = *((const Fcall **)(args[0]));
	type = f->type;
	fid = f->fid;
	tag = f->tag;
	switch(type){
	case Tversion:	/* 100 */
		snprintf(buf, sizeof(buf), "Tversion tag %"PRIu16" msize %"PRIu32" version '%s'", tag, f->msize, f->version);
		break;
	case Rversion:
		snprintf(buf, sizeof(buf), "Rversion tag %"PRIu16" msize %"PRIu32" version '%s'", tag, f->msize, f->version);
		break;
	case Tauth:	/* 102 */
		snprintf(buf, sizeof(buf), "Tauth tag %"PRIu16" afid %"PRIu32" uname %s aname %s", tag,
			f->afid, f->uname, f->aname);
		break;
	case Rauth:
		snprintf(buf, sizeof(buf), "Rauth tag %"PRIu16" qid " QIDFMT, tag,
			f->aqid.path, f->aqid.vers, qidtype(tmp, f->aqid.type));
		break;
	case Tattach:	/* 104 */
		snprintf(buf, sizeof(buf), "Tattach tag %"PRIu16" fid %"PRIu32" afid %"PRIu32" uname %s aname %s", tag,
			fid, f->afid, f->uname, f->aname);
		break;
	case Rattach:
		snprintf(buf, sizeof(buf), "Rattach tag %"PRIu16" qid " QIDFMT, tag,
			f->qid.path, f->qid.vers, qidtype(tmp, f->qid.type));
		break;
	case Rerror:	/* 107; 106 (Terror) illegal */
		snprintf(buf, sizeof(buf), "Rerror tag %"PRIu16" ename %s", tag, f->ename);
		break;
	case Tflush:	/* 108 */
		snprintf(buf, sizeof(buf), "Tflush tag %"PRIu16" oldtag %"PRIu16, tag, f->oldtag);
		break;
	case Rflush:
		snprintf(buf, sizeof(buf), "Rflush tag %"PRIu16, tag);
		break;
	case Twalk:	/* 110 */
		n = snprintf(buf, sizeof(buf), "Twalk tag %"PRIu16" fid %"PRIu32" newfid %"PRIu32" nwname %"PRIu16" ",
			tag, fid, f->newfid, f->nwname);
			for(i=0; i<f->nwname; i++)
				n += snprintf(buf+n, sizeof(buf)-n, "%d:%s ", i, f->wname[i]);
		break;
	case Rwalk:
		n = snprintf(buf, sizeof(buf), "Rwalk tag %"PRIu16" nwqid %"PRIu16" ", tag, f->nwqid);
		for(i=0; i<f->nwqid; i++){
			q = &f->wqid[i];
			n += sprintf(buf+n, "%d:" QIDFMT " ", i,
				q->path, q->vers, qidtype(tmp, q->type));
		}
		break;
	case Topen:	/* 112 */
		snprintf(buf, sizeof(buf), "Topen tag %"PRIu16" fid %"PRIu32" mode %"PRIu32, tag, fid, f->mode);
		break;
	case Ropen:
		snprintf(buf, sizeof(buf), "Ropen tag %"PRIu16" qid " QIDFMT " iounit %"PRIu32" ", tag,
			f->qid.path, f->qid.vers, qidtype(tmp, f->qid.type), f->iounit);
		break;
	case Tcreate: {	/* 114 */
		char perm[12];
		dirmodeconv(perm, f->perm);
		snprintf(buf, sizeof(buf), "Tcreate tag %"PRIu16" fid %"PRIu32" name %s perm %s mode %"PRIu16,
			tag, fid, f->name, perm, f->mode);
		break;
	}
	case Rcreate:
		snprintf(buf, sizeof(buf), "Rcreate tag %"PRIu16" qid " QIDFMT " iounit %"PRIu32" ", tag,
			f->qid.path, f->qid.vers, qidtype(tmp, f->qid.type), f->iounit);
		break;
	case Tread:	/* 116 */
		snprintf(buf, sizeof(buf), "Tread tag %"PRIu16" fid %"PRIu32" offset %"PRId64" count %"PRIu32,
			tag, fid, f->offset, f->count);
		break;
	case Rread:
		n = snprintf(buf, sizeof(buf), "Rread tag %"PRIu16" count %"PRIu32" ", tag, f->count);
			dumpsome(buf+n, f->data, f->count);
		break;
	case Twrite:	/* 118 */
		n = snprintf(buf, sizeof(buf), "Twrite tag %"PRIu16" fid %"PRIu32" offset %"PRId64" count %"PRIu32" ",
			tag, fid, f->offset, f->count);
		dumpsome(buf+n, f->data, f->count);
		break;
	case Rwrite:
		snprintf(buf, sizeof(buf), "Rwrite tag %"PRIu16" count %"PRIu32, tag, f->count);
		break;
	case Tclunk:	/* 120 */
		snprintf(buf, sizeof(buf), "Tclunk tag %"PRIu16" fid %"PRIu32, tag, fid);
		break;
	case Rclunk:
		snprintf(buf, sizeof(buf), "Rclunk tag %"PRIu16, tag);
		break;
	case Tremove:	/* 122 */
		snprintf(buf, sizeof(buf), "Tremove tag %"PRIu16" fid %"PRIu32, tag, fid);
		break;
	case Rremove:
		snprintf(buf, sizeof(buf), "Rremove tag %"PRIu16, tag);
		break;
	case Tstat:	/* 124 */
		snprintf(buf, sizeof(buf), "Tstat tag %"PRIu16" fid %"PRIu32, tag, fid);
		break;
	case Rstat:
		n = snprintf(buf, sizeof(buf), "Rstat tag %"PRIu16" ", tag);
		if(f->nstat > 200)
			snprintf(buf+n, sizeof(buf)-n, " stat(%"PRIu16" bytes)", f->nstat);
		else{
			Dir d[2];
			convM2D(f->stat, f->nstat, d, (char*)(d+1));
			strlcpy(buf+n, " stat ", sizeof(buf)-n);
			fdirconv(buf+n+6, sizeof(buf)-n-6, d);
		}
		break;
	case Twstat:	/* 126 */
		n = snprintf(buf, sizeof(buf), "Twstat tag %"PRIu16" fid %"PRIu32, tag, fid);
		if(f->nstat > 200)
			snprintf(buf+n, sizeof(buf)-n, " stat(%"PRIu16" bytes)", f->nstat);
		else{
			Dir d[2];
			convM2D(f->stat, f->nstat, d, (char*)(d+1));
			strlcpy(buf+n, " stat ", sizeof(buf)-n);
			fdirconv(buf+n+6, sizeof(buf)-n-6, d);
		}
		break;
	case Rwstat:
		snprintf(buf, sizeof(buf), "Rwstat tag %"PRIu16, tag);
		break;
	default:
		snprintf(buf, sizeof(buf), "unknown type %d", type);
	}
	return fprintf(stream, "%*s",
	    info->left ? -info->width : info->width, buf);
}

static char*
qidtype(char *s, uint8_t t)
{
	char *p;

	p = s;
	if(t & QTDIR)
		*p++ = 'd';
	if(t & QTAPPEND)
		*p++ = 'a';
	if(t & QTEXCL)
		*p++ = 'l';
	if(t & QTMOUNT)
		*p++ = 'm';
	if(t & QTAUTH)
		*p++ = 'A';
	*p = '\0';
	return s;
}

static void
fdirconv(char *buf, size_t len, Dir *d)
{
	char tmp[16];

	snprintf(buf, len,
	    "'%s' '%s' '%s' '%s' q " QIDFMT " m %#o "
	    "at %"PRIu32" mt %"PRIu32" l %zd "
	    "t %"PRIu16" d %u",
	    d->name, d->uid, d->gid, d->muid,
	    d->qid.path, d->qid.vers, qidtype(tmp, d->qid.type), d->mode,
	    d->atime, d->mtime, d->length,
	    d->type, d->dev);
}

/*
 * dump out count (or DUMPL, if count is bigger) bytes from
 * buf to ans, as a string if they are all printable,
 * else as a series of hex bytes
 */
#define DUMPL 64

static uint
dumpsome(char *ans, char *buf, long count)
{
	int i, printable;
	char *p;

	printable = 1;
	if(count > DUMPL)
		count = DUMPL;
	for(i=0; i<count && printable; i++)
		if((buf[i]<32 && buf[i] !='\n' && buf[i] !='\t') || (uint8_t)buf[i]>127)
			printable = 0;
	p = ans;
	*p++ = '\'';
	if(printable){
		memmove(p, buf, count);
		p += count;
	}else{
		for(i=0; i<count; i++){
			if(i>0 && i%4==0)
				*p++ = ' ';
			p += sprintf(p, "%2.2ux", (uint8_t)buf[i]);
		}
	}
	*p++ = '\'';
	*p = 0;
	return p - ans;
}

static int
arginfo(const struct printf_info *info, size_t n, int *argtypes, int *size)
{
	(void)size;
	if (n > 0)
		argtypes[0] = PA_POINTER;
	return 1;
}

void
initfcallconv(void)
{
	if (register_printf_specifier('J', fcallconv, arginfo) < 0)
		sysfatal("Cannot register format specifier for fcallconv");
}

/*
 * $Log:	bsearch.c,v $
 * Revision 1.3  90/09/20  19:36:00  avg
 * Cash -> cache
 * 
 * Revision 1.2  90/09/01  15:22:04  avg
 * 
 * Revision 1.1  90/08/31  23:15:42  avg
 * Initial revision
 * 
 */
#ifdef DEBUG
#include <stdio.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>

#define RFBUFSIZE       512
#define NRFBUFS         3

struct RFILE {
	char    *rf_ptr;           /* Current ptr */
	char    *rf_buf;           /* Begin of the buffer */
	char    *rf_end;           /* End of the buffer */
	long     rf_blkno;         /* Current block no */
	int      rf_fd;            /* File desc */
	long     rf_size;          /* Size of file */
	struct RF_BUF {
	    char     _rfb[RFBUFSIZE];   /* Working area */
	    long     _rfblk;            /* # of block */
	    unsigned _rfage;            /* Age */
	    unsigned _rflen;            /* Length */
	}        rf_bufs[NRFBUFS]; /* I/O buffers */
};

/* rf_fill's flags */
#define SEEK    0
#define FORW    1
#define BACK    2

#define rGetc(f)   (((f)->rf_ptr <= (f)->rf_buf) ? rf_fill(f, BACK) \
						 : (unsigned)*--((f)->rf_ptr) )
#define Getc(f)    (((f)->rf_ptr >= (f)->rf_end) ? rf_fill(f, FORW) \
						 : (unsigned)*((f)->rf_ptr)++ )

#define rftell(f)  (((f)->rf_blkno * RFBUFSIZE) + ((f)->rf_ptr - (f)->rf_buf))

#define rfsize(f)  ((f)->rf_size)

#ifndef EOF
#define EOF (-1)
#endif

#ifndef NULL
#define NULL    ((char *)0)
#endif


struct RFILE *rfopen(name, fs)
	char         *name;     /* Name of the file */
	struct RFILE *fs;       /* File structure */
{
	register i;
	struct stat st;

	if( (fs->rf_fd = open(name, 0)) == -1 )
		return NULL;
	if( fstat(fs->rf_fd, &st) < 0 ||
	    (st.st_mode & S_IFMT) != S_IFREG ) {
		close(fs->rf_fd);
		return NULL;
	}
	fs->rf_size  = st.st_size;
	fs->rf_ptr   = fs->rf_buf = fs->rf_end = NULL;
	fs->rf_blkno = -1;
	for( i = 0 ; i < NRFBUFS ; i++ ) {
		fs->rf_bufs[i]._rfblk = -2;
		fs->rf_bufs[i]._rfage = 1000;
	}
	return fs;
}

int rf_fill(f, flag)
	struct RFILE *f;        /* File structure */
{
	int len, bno ;
	register i;

	/*
	 * Advance pointer
	 */
	if( flag == FORW )
		f->rf_blkno++;
	else if( flag == BACK ) {
		f->rf_blkno--;
		if( f->rf_blkno < 0 )
			return EOF;
	}

	/*
	 * Try to find an appropriate block in cache
	 */
	for( i = 0 ; i < NRFBUFS ; i++ ) {
		if( f->rf_bufs[i]._rfblk == f->rf_blkno ) {
			bno = i;
			goto found;
		}
	}

	/*
	 * Try to find the oldest block
	 */
	len = 0;
	for( i = 0 ; i < NRFBUFS ; i++ ) {
		if( f->rf_bufs[i]._rfage > len ) {
			len = f->rf_bufs[i]._rfage;
			bno = i;
		}
	}

	/*
	 * Read in the new block
	 */
	lseek(f->rf_fd, RFBUFSIZE * f->rf_blkno, 0);
	len = read(f->rf_fd, f->rf_bufs[bno]._rfb, RFBUFSIZE);
#ifdef DEBUG
	printf("READ BLK %ld --> %d\n", f->rf_blkno, len);
#endif
	if( len <= 0 ) {
		f->rf_ptr = f->rf_buf = f->rf_end = NULL;
		return EOF;
	}
	f->rf_bufs[bno]._rfblk = f->rf_blkno;
	f->rf_bufs[bno]._rflen = len;
found:
	f->rf_buf = f->rf_bufs[bno]._rfb;
	f->rf_end = f->rf_buf + f->rf_bufs[bno]._rflen;
	f->rf_bufs[bno]._rfage = 0;
	for( i = 0 ; i < NRFBUFS ; i++ )
		f->rf_bufs[i]._rfage++;
	if( flag == FORW ) {
		f->rf_ptr = f->rf_buf;
		return (unsigned) *(f->rf_ptr)++;
	}
	if( flag == BACK ) {
		f->rf_ptr = f->rf_end;
		return (unsigned) *--(f->rf_ptr);
	}
	return 0;
}

rfclose(f)
	struct RFILE *f;        /* File structure */
{
	close(f->rf_fd);
}

rfseek(f, offset)
	struct RFILE *f;        /* File structure */
	long          offset;
{
	long    newblk;

	if( offset < 0 )
		return;
	newblk = offset / RFBUFSIZE;

	if( newblk != f->rf_blkno ) {
		if( newblk > (f->rf_size/RFBUFSIZE) )
			return;
		f->rf_blkno = newblk;
		rf_fill(f, SEEK);
	}
	f->rf_ptr = f->rf_buf + (unsigned)(offset % RFBUFSIZE);
}


/*
 * Binary search through sorted text file
 * Returns: NULL if not found or rest of the sting - if found
 */
char *bsearch(key, file)
	char *key;              /* The keyword */
	char *file;             /* File name */
{
	struct RFILE *f;
	static struct RFILE fss;
	long p1, p2, cp;
	long bp, ep;
	register c;
	static char line[300];
	register char *q, *r;

	if( (f = rfopen(file, &fss)) == NULL )
		return NULL;
	p1 = 0L;
	p2 = rfsize(f);
	cp = (p1+p2)/2;
	while( p1 < p2 ) {
		rfseek(f, cp);
		while( (c = rGetc(f)) != '\n' && c != EOF );
		if( c != EOF )
			Getc(f);        /* should return c again */
		bp = rftell(f);
		q = line;
		while( (c = Getc(f)) != '\n' && c != EOF )
			*q++ = c;
		*q = '\0';
		ep = (c==EOF)? rfsize(f) : rftell(f);
		q = line;
		r = key;
		while( *q == *r && *r ) q++, r++;
		if( *r == '\0' && (*q == ' ' || *q == '\t' || *q == '\0') ) {
			while( *q == ' ' || *q == '\t' )
				q++;
			rfclose(f);
			return q;       /* Gogo, We found It */
		}
		if( (unsigned)*q < (unsigned)*r )
			p1 = ep;
		else
			p2 = bp;
		cp = (p1+p2)/2;
		if( bp <= cp && cp < ep )
			break;
	}
	rfclose(f);
	return NULL;
}

#ifdef DEBUG
main()
{
	char l[100];
	char *p;

	for(;;) {
		printf("? ");
		fflush(stdout);
		gets(l);
		p = bsearch(l, "UULIST");
		if( p == NULL )
			printf( "NIL\n" );
		else
			printf( "FOUND: '%s'\n", p);
	}
}
#endif

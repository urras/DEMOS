#ifndef _dbm_h_
#       define  _dbm_h_

#define PBLKSIZ 512
#define DBLKSIZ 8192
#define BYTESIZ 8
#ifndef NULL
#       define NULL    ((char *) 0)
#endif

long    bitno;
long    maxbno;
long    blkno;
long    hmask;

char    pagbuf[PBLKSIZ];
char    dirbuf[DBLKSIZ];

int     dirf;
int     pagf;

typedef struct
{
	char    *dptr;
	int     dsize;
} datum;

datum   fetch();
datum   makdatum();
datum   firstkey();
datum   nextkey();
datum   firsthash();
long    calchash();
long    hashinc();

#endif  _dbm_h_

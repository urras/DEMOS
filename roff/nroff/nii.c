#include "tdef.h"
# include "tw.h"
#include "s.h"
#include "d.h"
#include "v.h"
#include <sgtty.h>

int    *vlist = (int *) & v;

struct s       *frame,
               *stk,
               *ejl,
               *nxf,
               *litlev;

int     pipeflg;
int     hflg;
int     eqflg;


filep roff,
apptr,
nextb,
woff,
offset,
ip;

struct sgttyb   ttys;

int     level;
int     stdi;
int     waitf;
int     nofeed;
int     quiet;
int     stop;
char    ibuf[IBUFSZ];
char    xbuf[IBUFSZ];
char   *ibufp;
char   *xbufp;
char   *eibuf;
char   *xeibuf;
int     cbuf[NC];
int    *cp;
int     nx;
int     mflg;
int     ch = 0;
int     cps;
int     ibf;
int     ttyod;
int     iflg;
char   *enda;
int     rargc;
char  **argp;
#ifndef ASA
char    trtab[256];
#else
int     trtab[512];
#endif
int     lgf;
int     copyf;
int     ch0;
int     cwidth;
int     nlflg;
int    *ap;
int     donef;
int     nflush;
int     nchar;
int     rchar;
int     nfo;
int     ifile;
int     padc;
int     raw;
int     ifl[NSO];
int     ifi;
int     flss;
int     nonumb;
int     trap;
int     tflg;
int     ejf;
int     lit;
int     gflag;
int     dilev;
int     tlss;
int     em;
int     ds;
int     app;
int     ndone;
int     lead;
int     ralss;
int    *argtop;
int     nrbits;
int     nform;
int     oldmn;
int     newmn;
int     macerr;
int     diflg;
int     wbfi;
int     inc[NN];
int     fmt[NN];
int     evi;
int     vflag;
int     noscale;
int     po1;
int     nlist[NTRAP];
int     mlist[NTRAP];
int     evlist[EVLSZ];
int     ev;
int     tty;
int     sfont;
int     sv;
int     esc;
int     widthp;
int     xfont;
int     setwdf;
int     xbitf;
int     over;
int     nhyp;
int   **hyp;
int    *olinep;
int     esct;
int     ttysave = -1;
int     dotT;
char   *unlkp;
int     no_out;

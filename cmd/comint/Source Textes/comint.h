#ifndef _COMINT_H_
#define _COMINT_H_

#include <sys/types.h>
#include <dir.h>
#include <string.h>

#ifdef USG
#  ifndef SYSV
#      define SYSV
#  endif
#  define D_NAME DIRSIZ
#  define index strchr
#endif



#define or  ||
#define and &&
#define not !
#define NO      0
#define YES     1
#define boolean int

#undef NULL
#define NULL    ((char*)0)

#define history    200
#define maxcmd      80
#define maxcat     252  /* Here Mr.Flerow has missed a bug. */
	/* If more than `maxcat` entries are in directory, some files
	 * are not shown.
	 * `maxcat` cannot be made more than 252 because some arrays
	 * are of char[] type.
	 * I also tied to replace some char[] by short[]
	 * and make maxcat=1024. No luck, even worse.
	 * (Currently returned to char[])
	 * A dirty fix was to show warning in upper-left corner
	 * when entering the directory for the first time,
	 * in case directory contains more than `maxcat` files
	 * The largeflag being set in getcat() , keeps track on this.
	 * Please let me know if somebody will fix this more accurately.
	 *                                 A.Rygoff (095)-334-1056
	 */

#define lname     1024
#define lnam1     lname+D_NAME+1
#define maxalt     128
#define wcolumn     16
#define maxmark maxcat

int   addnice;

struct dir2 {
    char d_null;
    char d_mark;
    char d_name[D_NAME];
};

char  hcat[lname+1];
char *arpath;
char *arname, arbuf[D_NAME+1];

char  altcat[lnam1];
char *altpath;
char  altarname[D_NAME+1];
int   nalt;
struct dir2 altnam[maxalt+1];

int   xsize,   ysize;
int   x0;
int   columns, width,
      lines,   current;

long        last_mod;
struct dir2 cat[maxcat+3];
int         lcat;
int         lind;
int         nfiles;
char        indcat[maxcat];
char        indico[maxcat];
int         maxico;

int   only_x;

char  select[D_NAME+1];
char  selsuf;
int   ico_from;

char   marked[maxmark];
int    nmarked;

char   y_head, y_run,
       y_name, y_table;

char   *inern[10];




extern int   errno;
extern char *strcat(), *index(),   *getenv(),    *rindex(), *strcpy();
extern long  time(),    lseek();
char        *conv(),   *lookcom(), *get_shbuf(), *copy(),   *Mkdir(),
	    *chname(), *no_esc(),  *prot_name(), *getuname();
struct dir2 *item();


#endif /*_COMINT_H_ */

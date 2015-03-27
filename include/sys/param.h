/*
 *      Глобальные константы.
 */

#ifdef KERNEL
#include        "h/whoami.h"
#else
# include        <sys/whoami.h>
#endif
#include        <signal.h>
#include        <sys/types.h>

#define MAXMEM  (400*16)        /* max core per process - first # is Kb */
#define MAXUPRC 20              /* max processes per user */
#define SSIZE   20              /* initial stack size (*64 bytes) */
#define SINCR   20              /* increment of stack (*64 bytes) */
#define NOFILE  20              /* max open files per process */
#define CANBSIZ 256             /* max size of typewriter line */
#define MSGBUFS 128             /* Characters saved from error messages */
#define NCARGS  5120            /* # characters in exec arglist */
#ifdef  UCB_METER
#define MAXSLP  20              /* max time a process is considered sleeping */
#endif

/*
 * priorities
 * probably should not be
 * altered too much
 */
#define PMASK 0177
#define PCATCH 0400

#ifdef  CGL_RTP
#define PRTP    0
#define PSWP    5
#else
#define PSWP    0
#endif
#define PINOD   10
#define PRIBIO  20
#define PZERO   25
#define PPIPE   26
#define PWAIT   30
#define PLOCK   35
#define PSLEP   40
#define PUSER   50
#define PIDLE   127

#define NZERO   20

#define ULEVEL  7
#define RTLEVEL 16
#define MAXLEVEL 31

/*
 * fundamental constants of the implementation--
 * cannot be changed easily
 */

#define NBPW    sizeof(int)     /* number of bytes in an integer */
#define NBBY    8               /* number of bit in a byte */

#define BSLOP   0               /* BSLOP can be 0 unless you have a TIU/Spider*/

#ifndef UCB_NKB
#define BSIZE   512             /* size of secondary block (bytes) */
#define NINDIR  (BSIZE/sizeof(daddr_t))
#define BMASK   0777            /* BSIZE-1 */
#define BSHIFT  9               /* LOG2(BSIZE) */
#define NMASK   0177            /* NINDIR-1 */
#define NSHIFT  7               /* LOG2(NINDIR) */
#endif

#if     UCB_NKB == 1
#define CLSIZE  2               /* number of blocks / cluster */
#define BSIZE   1024            /* size of secondary block (bytes) */
#define NINDIR  (BSIZE/sizeof(daddr_t))
#define BMASK   01777           /* BSIZE-1 */
#define BSHIFT  10              /* LOG2(BSIZE) */
#define NMASK   0377            /* NINDIR-1 */
#define NSHIFT  8               /* LOG2(NINDIR) */
#endif

#define UBSIZE  512             /* block size visible to users */
#ifdef  UCB_QUOTAS
#define QCOUNT  (BSIZE/UBSIZE)  /* BSIZE must always be a multiple of UBSIZE */
#endif

#define USIZE   32              /* size of user block (*64) */
#define NULL    0
#define CMASK   0               /* default mask for file creation */
#define NODEV   (dev_t)(-1)
#define ROOTINO ((ino_t)2)      /* i number of all roots */
#define SUPERB  ((daddr_t)1)    /* block number of the super block */
#define DIRSIZ  14              /* max characters per directory */

#define NICINOD 100             /* number of superblock inodes */
#define NICFREE 50              /* number of superblock free blocks */

#define CBSIZE  14              /* number of chars in a clist block */
				/* CBSIZE+sizeof(int *) must be a power of 2 */
#define CROUND  017             /* clist rounding: sizeof(int *) + CBSIZE - 1*/

#define PGSIZE  512             /* bytes per addressable disk sector */
#define PGSHIFT 9               /* LOG2(PGSIZE) */

/*
 * Some macros for units conversion
 */

/* Core clicks (64 bytes) to segments and vice versa */
#define ctos(x)         (((x)+127)/128)
#define stoc(x)         ((x)*128)

/* Core clicks (64 bytes) to disk blocks */
#define ctod(x)         (((x)+7)>>3)

/* I number to disk address */
#ifndef UCB_NKB
#define itod(x)         (daddr_t)((((unsigned)(x)+15)>>3))
#else
#define itod(x)         ((daddr_t)((((unsigned)(x)+2*INOPB-1)/INOPB)))
#endif

/* I number to disk offset */
#ifndef UCB_NKB
#define itoo(x)         (int)(((x)+15)&07)
#else
#define itoo(x)         ((int)(((x)+2*INOPB-1)%INOPB))
#endif

/* file system blocks to disk blocks and back */
#ifndef UCB_NKB
#define fsbtodb(b)      ((daddr_t)(b))
#define dbtofsb(b)      ((daddr_t)(b))
#else
#define fsbtodb(b)      ((daddr_t)((daddr_t)(b)<<1))
#define dbtofsb(b)      ((daddr_t)((daddr_t)(b)>>1))
#endif

/* round a number of clicks up to a whole cluster */
#ifdef  UCB_NKB
#define clrnd(i)        (((i) + (CLSIZE-1)) & ~(CLSIZE-1))
#endif

/* clicks to bytes */
#define ctob(x)         ((x)<<6)

/* bytes to clicks */
#define btoc(x)         ((((unsigned)(x)+63)>>6))

/* low int of a long */
#define loint(l)        ((int) (l) & 0177777)

/* high int of a long */
#define hiint(l)        ((int) ((l) >> 16))

/*
 * Machine-dependent bits and macros
 */

/*
 * Treat PS as byte, to allow restoring value from mfps/movb
 * (see :splfix.*)
 */
#define PS_LOBYTE       ((char *) 0177776)
#ifndef IPK_KPROF
#define splx(ops)       (*PS_LOBYTE = ((char) (ops)))
#endif

#ifndef MIN
# define MIN(a,b)        (((a)<(b))? (a):(b))
#endif

#ifndef MAX
# define MAX(a,b)        (((a)>(b))? (a):(b))
#endif

/*
 * Return values from tsleep().
 */
#define TS_OK   0       /* normal wakeup */
#define TS_TIME 1       /* timed-out wakeup */
#define TS_SIG  2       /* asynchronous signal wakeup */

#define NSYMLNK 8       /* максимальная глубина симв. ссылок */

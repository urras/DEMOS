#ifndef _sys_tty_h_
#       define  _sys_tty_h_

#include <sys/ioctl.h>
#include <sgtty.h>

/*
 * A clist structure is the head
 * of a linked queue of characters.
 * The characters are stored in
 * blocks containing a link and CBSIZE (param.h)
 * characters.  The routines getc, putc, ... in prim.c
 * manipulate these structures.
 */
struct clist
{
	short   c_cc;           /* character count */
	char    *c_cf;          /* pointer to first char */
	char    *c_cl;          /* pointer to last char */
};

struct cblock {
	struct  cblock  *c_next;
	char    c_info[CBSIZE];
};

#ifndef UCB_CLIST
#define lookc(cp)       (*(cp))
#endif

/*
 * A tty structure is needed for
 * each UNIX character device that
 * is used for normal terminal I/O.
 * The routines in tty.c handle the
 * common code associated with
 * these structures.  The definition
 * and device dependent code is in
 * each driver (e.g. dh.c, dz.c, kl.c).
 */
struct tty
{
	union {
		struct {
			struct  clist   T_rawq;
			struct  clist   T_canq;
		} t_t;
#define t_rawq  t_nu.t_t.T_rawq         /* raw characters or partial line */
#define t_canq  t_nu.t_t.T_canq         /* complete input lines */
#if     NBK > 0
		struct {
			struct  buf     *T_bufp;
			char    *T_cp;
			short   T_inbuf;
			short   T_rec;
		} t_n;
#define t_bufp  t_nu.t_n.T_bufp         /* buffer allocated to protocol */
#define t_cp    t_nu.t_n.T_cp           /* pointer into the ripped off buffer */
#define t_inbuf t_nu.t_n.T_inbuf        /* number chars in the buffer */
#define t_rec   t_nu.t_n.T_rec          /* have a complete record */
#endif  NBK
#if     NMPY > 0
		struct {
			short   T_mpichan;
			short   T_mpochan;
			short   T_mpline;
		} t_m;
#define t_mpichan t_nu.t_m.T_mpichan    /* Current MPY input chan */
#define t_mpochan t_nu.t_m.T_mpochan    /* Current MPY out chan */
#define t_mpline  t_nu.t_m.T_mpline     /* Currently active mtys group */
#endif NMPY
	} t_nu;
	struct  clist   t_outq;         /* output list to device */
	short   (*t_oproc)();   /* routine to start output */
	short   (*t_iproc)();   /* routine to start input */
	struct proc *t_rsel;
	struct proc *t_wsel;
	caddr_t t_addr;         /* device address */
	dev_t   t_dev;          /* device number */
	short   t_flags;        /* mode, settable by ioctl call */
	short   t_state;        /* internal state, not visible externally */
	short   t_pgrp;         /* process group name */
	char    t_delct;        /* number of delimiters in raw q */
	char    t_line;         /* line discipline */
	char    t_col;          /* printing column of device */
	char    t_erase;        /* erase character */
	char    t_kill;         /* kill character */
	char    t_char;         /* character temporary */
	char    t_ispeed;       /* input speed */
	char    t_ospeed;       /* output speed */
	char    t_nldly;        /* newline delay */
	char    t_crdly;        /* carriage return delay */
	char    t_htdly;        /* tab delay */
	char    t_vtdly;        /* vertical tab delay */
	char    t_width;        /* width for output */
	char    t_length;       /* length of screen */
	char    t_rocount;      /* chars input since a ttwrite() */
	char    t_rocol;        /* t_col of first input char on this line */
	struct  ltchars t_lchr; /* local special characters */
	short   t_local;        /* local mode word */
	short   t_lstate;       /* local state bits */
	struct clist *t_optr;   /* указатель на выходную очередь */
	int     (*t_tproc)();   /* процедура записи символа в выходную
				 * очередь (NULL - если не определено)
				 * Аргументы: (c, tp)
				 *  ВЫЗОВ: PutcOut(c, tp)
				 */
#ifdef  TEXAS_AUTOBAUD
	short   t_xflags;       /* extension of t_local */
#endif
	union {
		struct  tchars  t_chr;
		struct  clist   t_ctlq;
	} t_un;
/*        struct  buf     *t_ibp, *t_obp;       */
#define DO_ESCAPE       1
#ifdef  DO_ESCAPE
	long t_esctime;
	char t_escstate;
#endif  DO_ESCAPE
};

#define tun     tp->t_un.t_chr
#define tlun    tp->t_lchr

/* Поместить символ в выходную очередь */
#define PutcOutq(c, tp) ( ((tp)->t_tproc==NULL)? putc((c),(tp)->t_optr) \
			  :(*((tp)->t_tproc))((c),(tp)) )

#define TTIPRI  28
#define TTOPRI  29

#define CTRL(c) ('c'&037)

/* default special characters */
#define CERASE  0177            /* DEL */
#define CEOT    CTRL(d)
#define CKILL   CTRL(u)
#define CQUIT   034             /* FS, cntl shift L */
#define CINTR   CTRL(c)
#define CSTOP   CTRL(s)
#define CSTART  CTRL(q)
#define CBRK    0377            /* Undefined now */
#define CRUS    CTRL(n)
#define CLAT    CTRL(o)
#define CESCAPE CTRL([)

/* limits */
#define NSPEEDS 16
#define TTMASK  15
#ifdef  KERNEL
short   tthiwat[NSPEEDS], ttlowat[NSPEEDS];
#define TTHIWAT(tp)     tthiwat[(tp)->t_ospeed&TTMASK]
#define TTLOWAT(tp)     ttlowat[(tp)->t_ospeed&TTMASK]
#endif
#define TTYHOG  256

/* internal state bits */
#define TIMEOUT 01              /* delay timeout in progress */
#define WOPEN   02              /* waiting for open to complete */
#define ISOPEN  04              /* device is open */
#define FLUSH   010             /* outq has been flushed during DMA */
#define CARR_ON 020             /* software copy of carrier-present */
#define BUSY    040             /* output in progress */
#define ASLEEP  0100            /* wakeup when output done */
#define XCLUDE  0200            /* exclusive-use flag against open */
#define TTSTOP  0400            /* output stopped by ctl-s */
#define HUPCLS  01000           /* hang up upon last close */
#define TBLOCK  02000           /* tandem queue blocked */
#define SPEEDS  04000           /* t_ispeed and t_ospeed used by driver */
#define TS_RCOLL        010000  /* collision in read select */
#define TS_WCOLL        020000  /* collision in write select */
#define TS_NBIO         040000  /* tty in non-blocking mode */
#define TS_ASYNC        0100000 /* tty in async i/o mode */

#ifdef  notdef
#define NDQB    010000
#define EXTPROC 020000          /* external processor (kmc) */
#define FSLEEP  040000          /* Wakeup on input framing */
#define BEXT    0100000         /* use (external) system buffers */
#endif

/* define partab character types */
#define ORDINARY        0
#define CONTROL         1
#define BACKSPACE       2
#define NEWLINE         3
#define TAB             4
#define VTAB            5
#define RETURN          6

#ifdef  TEXAS_AUTOBAUD
#define image_mode(tp)  ((tp)->t_xflags & LIMAGE)
#endif

/* local mode extension (t_xflags) */
#ifdef  TEXAS_AUTOBAUD
#define LIMAGE  01              /* ignoring framing errors for autobauding */
#endif

/* local state */
#define LSBKSL  01              /* state bit for lowercase backslash work */
#define LSQUOT  02              /* last character input was \ */
#define LSERASE 04              /* within a \.../ for LPRTRUB */
#define LSLNCH  010             /* next character is literal */
#define LSTYPEN 020             /* retyping suspended input (LPENDIN) */
#define LSCNTTB 040             /* counting width of tab; leave LFLUSHO alone */
#define LIREG   0100            /* RUS/LAT input register */
#define LOREG   0200            /* RUS/LAT output register */
#define LNOWID  0400            /* Don't use <width> facility of newtty */
#define L8BITS  01000           /* Признак 8 бит (для драйвера)         */
#define	LCHREG	02000		/* Признак - в rawq пошел SI/SO */

#endif  _sys_tty_h_

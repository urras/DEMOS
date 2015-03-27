#ifndef _sgtty_h_
#       define  _sgtty_h_

/*
 * Structure for stty and gtty system calls.
 * New version by @VG  4/9/86
 * 14/10/86 - добавлена совместимость с СМ-1700
 */

#include <sys/ioctl.h>

struct  sgttyb {
	char    sg_ispeed;              /* input speed */
	char    sg_ospeed;              /* output speed */
	char    sg_erase;               /* erase character */
	char    sg_kill;                /* kill character */
	short   sg_flags;               /* mode flags */
};

/* @VG full sgttyb table */

struct  sgttya {
	/* sgtty */
	char    sg_ispeed;              /* input speed */
	char    sg_ospeed;              /* output speed */
	char    sg_erase;               /* erase character */
	char    sg_kill;                /* kill character */
	short   sg_flags;               /* mode flags */
	/* delays + locals */
	char    sg_nldly;
	char    sg_crdly;
	char    sg_htdly;
	char    sg_vtdly;
	char    sg_width;
	char    sg_length;
	short   sg_local;
	/* tchars */
	char    sg_intrc;
	char    sg_quitc;
	char    sg_startc;
	char    sg_stopc;
	char    sg_eofc;
	char    sg_brkc;
	/* ltchars */
	char    sg_suspc;
	char    sg_dsuspc;
	char    sg_rprntc;
	char    sg_flushc;
	char    sg_werasc;
	char    sg_lnextc;
};

/*
 * Delays are in bytes, sign bit means 'computed delays'.
 * Bits 0-7 is a delay in 20 msec clicks ( 1 sec / HZ ).
 * For calculated delays, bits 0-7 is unit of delay -
 * 20/16 msec clicks ( 1 sec / (HZ*16) ); full delay is
 * cursor (head) moving units times to bits 0-7.
 */

/*
 * Modes
 */
#define TANDEM  01              /* tandem mode */
#define CBREAK  02              /* CBREAK mode */
#define LCASE   04              /* map upper case to lower case on input */
#define ECHO    010             /* echo input characters */
#define CRMOD   020             /* turn \r to \n */
#define RAW     040             /* RAW mode */
#define ODDP    0100            /* odd parity */
#define EVENP   0200            /* even parity */
#define ANYP    0300            /* no parity checks */
#define XTABS   0400            /* turn tabs to spaces */
#define FRAME7  01000           /* 7-bits char frame format (unimplemented) */
#define DSTOPS  02000           /* Double stop bits (unimplemented) */
#define CYRILL  04000           /* tty has cyrillic */
#define CSTYLE  070000          /* Cyrillic style */
  /* no LCASE */
# define CBITS8  000000          /* 8-bits mode (unimplemented) */
# define CEL15   010000          /* Standard Electronica 15IE-00-013 */
# define CEL15I  020000          /* Electronica 15IE-00-013, IPK fix, also
				    Cuban CM1611 */
# define CEL15K  030000          /* Electronica 15IE-00-013, KIAE fix (unimplemented) */
# define CBMKBD  040000          /* Electronica 15IE-002, BMK fix */
# define C52130  050000          /* VDT52130, SISO input style (unimplemented) */
# define CBITS8Q 060000          /* 8 бит + QWERTY  */
# define CSISOQ  070000          /* SI/SO + QWERTY  */
  /* LCASE */
# define SISO    010000          /* SI-SO input style (else with '\') */
# define UCASE   020000          /* Map output upper case like \A */

/*
 * Speeds
 */
#define B0      0
#define B50     1
#define B75     2
#define B110    3
#define B134    4
#define B150    5
#define B200    6
#define B300    7
#define B600    8
#define B1200   9
#define B1800   10
#define B2400   11
#define B4800   12
#define B9600   13
#define B19200  14
#define EXTA    14
#define EXTB    15

/* Совместимость DEMOS-1700/DEMOS SM4 */
#define sgttyb1 sgttya
#define CS_TYPE0 CEL15
#define CS_TYPE1 CEL15K
#define CS_TYPE2 CEL15I
#define CS_TYPE3 CSISOQ
#define CS_TYPE5 CBITS8Q

#endif _sgtty_h_

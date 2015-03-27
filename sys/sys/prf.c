/*
 * $Log:	prf.c,v $
 * Revision 22.2  90/11/12  19:15:36  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.1  89/04/12  15:29:52  korotaev
 * "param.h" ==> <sys/param.h>
 * 
 * Revision 22.0  89/03/30  16:16:33  korotaev
 * Begining DEMOS 2.2.
 * 
 * Revision 1.7  89/03/24  13:05:28  abs
 * panic теперь выдает имя и номер процесса, 
 * вызвавшего панику.
 * 
 * Revision 1.6  89/01/07  20:49:17  korotaev
 * Слияние с исправлениями насчет FIFO, +fcntl, + разные режимы открытия
 *
 * (from VAX/UTS SV.2)
 *
 * Revision 1.4  87/04/18  14:19:38  avg
 * Сделана правка под EXMOUNT.
 *
 * Revision 1.3  87/01/22  08:37:24  alex
 * Расширили номер устройства при печати.
 *
 * Revision 1.2  86/05/12  11:25:56  avg
 * Сокращена диагностика HARD ERROR.
 *
 * Revision 1.1  86/04/19  15:50:58  avg
 * Initial revision
 *
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/filsys.h>
#include "../include/mount.h"
#include <sys/seg.h>
#include "../include/buf.h"
#include <sys/conf.h>
#include "../include/inline.h"
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/reboot.h>

/*
 * In case console is off,
 * panicstr contains argument to last
 * call to panic.
 */

char    *panicstr;

/*
 * Scaled down version of C Library printf.
 * Only %c %s %u %d (==%u) %o %x %D %O are recognized.
 * Used to print diagnostic information directly on console tty.
 * Since it is not interrupt driven, all system activities are
 * suspended.  Printf should not be used for chit-chat.
 *
 */
#ifdef  UCB_DEVERR
/*
 * One additional format: %b is supported to decode error registers.
 * Usage is:
 *      printf("reg=%b\n", regval, "<base><arg>*");
 * Where <base> is the output base expressed as a control character,
 * e.g. \10 gives octal; \20 gives hex.  Each arg is a sequence of
 * characters, the first of which gives the bit number to be inspected
 * (origin 1), and the next characters (up to a control character, i.e.
 * a character <= 32), give the name of the register.  Thus
 *      printf("reg=%b\n", 3, "\10\2BITTWO\1BITONE\n");
 * would produce output:
 *      reg=3<BITTWO,BITONE>
 */
#endif  UCB_DEVERR

/*VARARGS1*/
printf(fmt, x1)
register char *fmt;
unsigned x1;
{
	prf(fmt, &x1, 0);
}

#ifdef  UCB_UPRINTF
/*
 *      Uprintf prints to the current user's terminal,
 *      guarantees not to sleep (so could be called by interrupt routines;
 *      but prints on the tty of the current process)
 *      and does no watermark checking - (so no verbose messages).
 *      NOTE: with current kernel mapping scheme, the user structure is
 *      not guaranteed to be accessible at interrupt level (see seg.h);
 *      a savemap/restormap would be needed here or in putchar if uprintf
 *      was to be used at interrupt time.
 */
/*VARARGS1*/
uprintf(fmt, x1)
char    *fmt;
unsigned x1;
{
	prf(fmt, &x1, 2);
}
#else

/*VARARGS1*/
uprintf(fmt, x1)
char *fmt;
unsigned x1;
{
	prf(fmt, &x1, 0);
}

#endif  UCB_PRINTF

prf(fmt, adx, touser)
register char *fmt;
register unsigned int *adx;
{
	register c;
	char *s;
#ifdef  UCB_DEVERR
	int     i, any;
	unsigned b;
#endif

loop:
	while((c = *fmt++) != '%') {
		if(c == '\0')
			return;
		putchar(c, touser);
	}
	c = *fmt++;
	if(c == 'd' || c == 'u' || c == 'o' || c == 'x')
		printn((long)*adx, c=='o'? 8: (c=='x'? 16:10), touser);
	else if(c == 'c')
		putchar(*adx, touser);
	else if(c == 's') {
		s = (char *)*adx;
		while(c = *s++)
			putchar(c, touser);
	} else if (c == 'D' || c == 'O' || c == 'X') {
		printn(*(long *)adx, c=='D'?10:c=='O'?8:16, touser);
		adx += (sizeof(long) / sizeof(int)) - 1;
	}
#ifdef  UCB_DEVERR
	else if (c == 'b') {
		b = *adx++;
		s = (char *) *adx;
		printn((long) b, *s++, touser);
		any = 0;
		if (b) {
			putchar('<', touser);
			while (i = *s++) {
				if (b & (1 << (i - 1))) {
					if (any)
						putchar(',', touser);
					any = 1;
					for (; (c = *s) > 32; s++)
						putchar(c, touser);
				} else
					for (; *s > 32; s++)
						;
			}
			putchar('>', touser);
		}
	}
#endif
	adx++;
	goto loop;
}

/*
 * Print an unsigned integer in base b, avoiding recursion.
 */
printn(n, b, touser)
long n;
{
	long a;
	char prbuf[12];
	register char *cp;

	if (b == 10 && n < 0) {
		putchar('-', touser);
		n = -n;
	}
	cp = prbuf;
	do {
		*cp++ = "0123456789ABCDEF"[(int)(n%b)];
	} while (n = n/b);      /* Avoid  n /= b, since that requires alrem */
	do
		putchar(*--cp, touser);
	while (cp > prbuf);
}

int panicreboot;

/*
 * Panic is called on fatal errors.
 * It prints "panic: mesg", syncs, and then reboots if possible.
 * If we are called twice, then we avoid trying to
 * sync the disks as this often leads to recursive panics.
 */
panic(s)
register char *s;
{
	/* machdep hack */
	segm save;

	int     bootopt = (panicstr == NULL) ?  RB_DUMP : (RB_DUMP | RB_NOSYNC | RB_HALT);

	if( !panicreboot )
		bootopt |= RB_HALT;
	if (panicstr == NULL)
		panicstr = s;

	  saveseg5(save); normalseg5();  /* machdep hack */
	  printf("panic(%d)%s: %s\n", u.u_procp->p_pid, u.u_comm, s);
	  restorseg5(save);              /* end of machdep hack */

	(void) _spl0();
	boot(rootdev,bootopt);
	/*NOTREACHED*/
}

/*
 * Warn that a system table is full.
 */
tablefull(tab)
char    *tab;
{
	printf("%s: table is full\n", tab);
}

/*
 * Hard error is the preface to plaintive error messages
 * about failing disk tranfers.
 */
harderr(bp, cp)
struct buf *bp;
char *cp;
{
	printf("%s%d%c: err bn %D ", cp,
	    ((bp->b_dev>>3)&037), 'a' + (minor(bp->b_dev) & 07), bp->b_blkno);
}

/*
 * Fserr prints the name of a file system
 * with an error diagnostic, in the form
 *      filsys:  error message
 */
fserr(fp, cp)
struct  filsys *fp;
char    *cp;
{
	printf("%s: %s\n", fp->s_fsmnt, cp);
}

#ifndef UCB_DEVERR
/*
 * deverr prints a diagnostic from
 * a device driver.
 * It prints the device, block number,
 * and 2 octal words (usually some error
 * status registers) passed as argument.
 */
deverror(bp, o1, o2)
register struct buf *bp;
{
	register struct mount *mp;
	register struct filsys *fp;

	for(mp = mount; mp < mountNMOUNT; mp++)
		if(mp->m_inodp != NULL)
			if(bp->b_dev == mp->m_dev) {
#ifndef EXMOUNT
				fp = &mp->m_filsys;
				fserr(fp, "err");
#else
				fp = (struct filsys *)mapin(mp->m_filsys);
				fserr(fp, "err");
				mapout(mp->m_filsys);
#endif EXMOUNT
			}
	printf("err on dev %u/%u\n", major(bp->b_dev), minor(bp->b_dev));
	printf("bn=%D er=%o,%o\n", bp->b_blkno, o1, o2);
}
#endif  UCB_DEVERR

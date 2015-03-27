/*
 * Драйвер интерфейса внешнего устройства для Электроники-85
 */

#include "kx.h"
#if NKX > 0
#include "param.h"
#include <sys/conf.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/tty.h>
#include <sys/systm.h>
#include <sys/iopage.h>

#define SET_RIE         035
#define SET_XIE         036

#define RES_RIE         075
#define RES_XIE         076

#define RB(x)   *((char *)(x))

struct kxdevice {
	short   kx_buf;
	short   kx_stat;
	short   kx_mode;
	short   kx_com;
};

extern struct kxdevice *KXADDR;

struct  tty kx11[1];
int     nkx11   = 1;          /* for pstat */
int     kxstart();
int     ttrstrt();
extern char partab[];
static  kxinit;

kxattach(addr, unit)
struct kxdevice *addr;
{
	if ((unsigned) unit == 0) {
		kx11[unit].t_addr = addr;
		return(1);
	}
	return(0);
}

/*ARGSUSED*/
kxopen(dev, flag)
dev_t   dev;
{
	register struct kxdevice *addr;
	register struct tty *tp;
	int      i;

	tp = &kx11[0];
	if (tp->t_addr == 0)
		tp->t_addr = KXADDR;
	addr = tp->t_addr;
	tp->t_oproc = kxstart;
	if ((tp->t_state & ISOPEN) == 0) {
		tp->t_state = ISOPEN | CARR_ON;
		tp->t_flags = ANYP | ECHO | CRMOD | LCASE;
		tp->t_line = DFLT_LDISC;
		ttychars(tp);
	} else if (tp->t_state & XCLUDE && u.u_uid != 0) {
		u.u_error = EBUSY;
		return;
	}
	RB(I85_CR1) = SET_RIE;
	RB(I85_CR1) = SET_XIE;

	/* Если в буфере был символ, прочитаем его */
	i = addr->kx_buf;

	ttyopen(dev, tp);
}

/*ARGSUSED*/
kxclose(dev, flag)
dev_t   dev;
int     flag;
{
	ttyclose(&kx11[0]);
}

kxread(dev)
dev_t   dev;
{
	register struct tty *tp;

	tp = &kx11[0];
	(*linesw[tp->t_line].l_read)(tp);
}

kxwrite(dev)
dev_t   dev;
{
	register struct tty *tp;

	tp = &kx11[0];
	(*linesw[tp->t_line].l_write)(tp);
}

kxxint(dev)
dev_t   dev;
{
	register struct tty *tp;

	tp = &kx11[0];
	ttstart(tp);
	if ((tp->t_state & ASLEEP) && tp->t_outq.c_cc <= TTLOWAT(tp))
			wakeup((caddr_t) &tp->t_outq);
}

static kxblock = 0, kxbf = 0;

kxrint(dev)
dev_t   dev;
{
	register int c;
	register struct kxdevice *addr;
	register struct tty *tp;
	int    s;
	static nsd;
	static struct clist que;

	tp = &kx11[0];
	addr = (struct kxdevice *) tp->t_addr;
	c = addr->kx_buf;
	if( tp->t_flags & RAW ) {
		c &= 0377;
		if( tp->t_line == MPYLDISC && c == tun.t_stopc ) {
			tp->t_state |= TTSTOP;
			return;
		}
	} else {
		c &= 0177;
		if( c == tun.t_stopc ) {
			tp->t_state |= TTSTOP;
			return;
		}
	}
	if( nsd ) {
		if( (tp->t_flags & TANDEM) && que.c_cc > TTYHOG/3 && !kxbf ) {
			kxblock++;
			kxbf++;
			kxstart(tp);
		}
		putc( c, &que );
		return;
	}
	nsd++;
	for(;;) {
		s = spl1();
		(*linesw[tp->t_line].l_input)(c, tp);
		if( kxbf && que.c_cc < TTYHOG/6 ) {
			if( putc(tun.t_startc, &tp->t_outq) == 0) {
				kxbf = 0;
				ttstart(tp);
			}
		}
		splx(s);
		if( que.c_cc > 0 ) {
			c = getc( &que );
			continue;
		}
		nsd = 0;
		return;
	}
}

kxioctl(dev, cmd, addr, flag)
caddr_t addr;
dev_t   dev;
{
	switch (ttioctl(&kx11[0], cmd, addr, flag)) {
		case TIOCSETN:
		case TIOCSETP:
		case TIOCSETA:
		case TIOCSETB:
		case 0:
			kxparam();
			break;
		default:
			u.u_error = ENOTTY;
	}
}

kxstart(tp)
register struct tty *tp;
{
	register c;
	register struct kxdevice *addr;

	addr = (struct kxdevice *) tp->t_addr;
	if ((addr->kx_stat & 01) == 0)
		return;
	if ( kxblock ) {
		kxblock = 0;
		addr->kx_buf = tun.t_stopc;
		return;
	}
	if ((c=getc(&tp->t_outq)) >= 0) {
		if (tp->t_lstate & L8BITS)
			addr->kx_buf = c;
		else if (c<=0177)
			addr->kx_buf = c /* | (partab[c] & 0200) */;
		else {
			timeout(ttrstrt, (caddr_t)tp, (c & 0177) );
			tp->t_state |= TIMEOUT;
		}
	}
}

static char kxspeeds[] = {
/* 0    */      0276,
/* 50   */      0260,
/* 75   */      0261,
/* 110  */      0262,
/* 134  */      0263,
/* 150  */      0264,
/* 200  */      0264,
/* 300  */      0265,
/* 600  */      0266,
/* 1200 */      0267,
/* 1800 */      0270,
/* 2400 */      0272,
/* 4800 */      0274,
/* 9600 */      0276,
/* 19200 */     0277,
/* EXTB */      0276
};

#define BITS    014
#define BITS5   0
#define BITS6   04
#define BITS7   010
#define BITS8   014

kxparam()
{
	register struct kxdevice *addr;
	static Oldmode = 0116;
	register struct tty *tp = &kx11[0];

	addr = (struct kxdevice *) tp->t_addr;

	/*
	Oldmode &= ~BITS;

	if( tp->t_flags & RAW )
		Oldmode |= BITS8;
	else
		Oldmode |= BITS7;
	 */
	/* Запись идет в режим работы #1 */
	addr->kx_mode = Oldmode;

	/* Запись идет в режим работы #2 */
	addr->kx_mode = kxspeeds[tp->t_ospeed & 017];
}
#endif NKX

/*
 *   Word line driver. This hard i/o by word.
 *                              (Korotaev)
 * $Log:	wl.c,v $
 * Revision 1.3  91/02/18  14:34:55  root
 * Был потерян один конец коментария * / .
 * 
 * Revision 1.2  91/01/11  18:16:43  korotaev
 * Правлено под два пересылаемых байта.
 * 
 * Revision 1.1  90/11/12  19:01:10  root
 * Initial revision
 * 
 * 
 */

#include "h/wl.h"
#include <sys/param.h>
#include <sys/conf.h>
#include <sys/user.h>
#include <sys/tty.h>
#include <sys/systm.h>

#if NWL > 0

struct wldevice {
	short   wlcsr;
	short   wlxbuf;
	short   wlrbuf;
};

#define WL_RTS  0100000         /* ready to send (read only) */
/* bits 14-8 are unused */
#define WL_TRDY 0000200         /* transmitter ready (read only) */
#define WL_RIE  0000100         /* receiver interrupt enable */
#define WL_DIE  0000040         /* dataset interrupt enable */

#define WL_ESC  0252    /* Пустой старший байт */

struct  tty wl11[NWL];
int     wlstart();
int     ttrstrt();
extern  char    partab[];

wlattach(addr, unit)
struct wldevice *addr;
{
#ifdef WLDEBUG
	printf("wlattach: unit=%d, addr=%o\n", unit, addr);
#endif WLDEBUG
	if ((unsigned) unit < NWL) {
		wl11[unit].t_addr = addr;
		return(1);
	}
	return(0);
}

/*ARGSUSED*/
wlopen(dev, flag)
dev_t   dev;
{
	register struct wldevice *addr;
	register struct tty *tp;
	register d;

	d = minor(dev);
#ifdef WLDEBUG
	printf("wlopen: dev=%d,%d, flag=%d\n",major(dev), d, flag);
#endif WLDEBUG
	tp = &wl11[d];
	if ((d >= NWL) || ((addr = tp->t_addr) == 0)) {
		u.u_error = ENXIO;
		return;
	}
	tp->t_oproc = wlstart;
	if ((tp->t_state & ISOPEN) == 0) {
		tp->t_flags = ANYP | RAW /* ECHO | LCASE | CRMOD */;
		tp->t_line = DFLT_LDISC;
		ttychars(tp);
	} else if(tp->t_state & XCLUDE && u.u_uid != 0) {
		u.u_error = EBUSY;
		return;
	}
	tp->t_state |= CARR_ON;
	ttyopen(dev, tp);
	_spl5();
	addr->wlcsr |= WL_RIE | WL_DIE;
	_spl0();
}

/*ARGSUSED*/
wlclose(dev, flag)
dev_t   dev;
int     flag;
{
	register struct wldevice *addr;
	register struct tty *tp;

#ifdef WLDEBUG
	printf("wlclose: dev=%d,%d, flag=%d\n",major(dev), minor(dev), flag);
#endif WLDEBUG
	tp = &wl11[minor(dev)];
	addr = (struct wldevice *) tp->t_addr;
	ttyclose(&wl11[minor(dev)]);
	addr->wlcsr = 0;
}

wlread(dev)
dev_t   dev;
{
	register struct tty *tp;

#ifdef WLDEBUG
	printf("wlread: dev=%d,%d\n",major(dev), minor(dev));
#endif WLDEBUG
	tp = &wl11[minor(dev)];
	(*linesw[tp->t_line].l_read)(tp);
}

wlwrite(dev)
dev_t   dev;
{
	register struct tty *tp;

#ifdef WLDEBUG
	printf("wlwrite: dev=%d,%d\n",major(dev), minor(dev));
#endif WLDEBUG
	tp = &wl11[minor(dev)];
	(*linesw[tp->t_line].l_write)(tp);
}

wlxint(dev)
dev_t   dev;
{
	register struct tty *tp;

#ifdef WLDEBUG
	printf("wlxint: dev=%d\n", minor(dev));
#endif WLDEBUG
	tp = &wl11[minor(dev)];
	ttstart(tp);
	if (tp->t_state & ASLEEP && tp->t_outq.c_cc <= TTLOWAT(tp))
			wakeup((caddr_t) &tp->t_outq);
}

#ifdef	MENLO_KOV
#define INTR_ARGS(ps)  ,i_sp, i_r1, i_ov, i_nps, i_r0, i_pc, ps
#else
#define INTR_ARGS(ps)  ,i_sp, i_r1, i_nps, i_r0, i_pc, ps
#endif

struct clist wl_inq[NWL];
int wl_ichars;
wlrint(dev INTR_ARGS(ps))
dev_t   dev;
{
	register int c;
	register struct tty *tp;
	int     c1;

	dev = minor(dev);
#ifdef WLDEBUG
	printf("wlrint: dev=%d\n",dev);
#endif WLDEBUG
	{
		register struct wldevice *addr;

		tp = &wl11[dev];
		addr = (struct wldevice *) tp->t_addr;
		addr->wlcsr &= ~WL_RIE;
		if( (addr->wlcsr & WL_TRDY) == 0 )
			goto rtrn;
		c = addr->wlrbuf;
		c1 = (c>>8)&0377;
		c &= 0377;
		if( c1 == WL_ESC ){  /* поступил 1 байт */
			if( wl_ichars++ ) {
				if( putc(c, &wl_inq[dev]) ) wl_ichars--;
				goto rtrn;
			}
		} else {        /* поступило 2 байта (младший - первый) */
			if( wl_ichars ) {  /* пхать оба  байта в очередь */
				if( putc(c, &wl_inq[dev]) == 0 ) wl_ichars++;
				if( putc(c1, &wl_inq[dev]) == 0 ) wl_ichars++;
				goto rtrn;
			} else {    /* пхать только один и идти дальше */
				wl_ichars++;
				if( putc(c1, &wl_inq[dev]) == 0 ) wl_ichars++;
			}
		}
	}
	{
		register int i;
		int s, chars=0;

		i = dev;
		s = spl1();
		goto c_input;
next:
		for(chars=0,i=0;i<NWL && wl_ichars; i++)
			if(wl_inq[i].c_cc) {
				s=spl1();
				tp = &wl11[i];
				c = getc(&wl_inq[i]);
c_input:
				(*linesw[tp->t_line].l_input)(c, tp);
				chars++;
				splx(s);
				wl_ichars--;
			}
		if( wl_ichars && chars) goto next;
		wl_ichars=0;
	}
rtrn:
	((struct wldevice *) tp->t_addr)->wlcsr |= WL_RIE;
	return;
}

wlioctl(dev, cmd, addr, flag)
caddr_t addr;
dev_t   dev;
{
	register struct tty *tp;

#ifdef WLDEBUG
	printf("wlioctl: dev=%d,%d\n",major(dev),minor(dev));
#endif WLDEBUG
	tp = &wl11[minor(dev)];
	switch (ttioctl(&wl11[minor(dev)], cmd, addr, flag)) {
		case TIOCSETN:
		case TIOCSETP:
		case TIOCSETA:
		case TIOCSETB:
		case 0:
			break;
		default:
			u.u_error = ENOTTY;
	}
}

wlstart(tp)
register struct tty *tp;
{
	register struct wldevice *addr;
	register c;
	struct {
		char lob;
		char hib;
	} *cc1;
	int c1, s;

#ifdef WLDEBUG
	printf("wlstart: dev=%d,%d\n",major(tp->t_dev),minor(tp->t_dev));
#endif WLDEBUG
	addr = (struct wldevice *) tp->t_addr;
	if ((addr->wlcsr & WL_RTS) == 0)
		return;
	s = spl1 ();
	cc1 = &c1;
	c = getc(&tp->t_outq);
	if( c >= 0) {
		if( (tp->t_lstate&L8BITS) == 0 )
			if( c <= 0177 )
				c |= (partab[c] & 0200);
			else {
				splx (s);
				timeout(ttrstrt,(caddr_t)tp,(c&0177)+3);
				tp->t_state |= TIMEOUT;
				return;
			}
		cc1->lob = (char)c;
	       /*
		* Символ получаем - lookc, а пропихиваем очередь - getc,
		*/
		if( tp->t_outq.c_cc > 0 &&
		    (c = lookc(tp->t_outq.c_cf)) >= 0 && c != WL_ESC ){
			if( (tp->t_lstate & L8BITS) == 0 ){
				if( c <= 0177 ){
					c |= (partab[c] & 0200);
					if( c != WL_ESC )
						getc(&tp->t_outq);
				} else
					c = WL_ESC;
			} else
				getc(&tp->t_outq);
		} else
			c = WL_ESC;
		cc1->hib = (char)c;
		splx (s);
		addr->wlxbuf = c1;
	} else
		splx (s);
}

#endif NWL

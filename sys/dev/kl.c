/*
 *   KL/DL-11 driver
 *
 * $Log:	kl.c,v $
 * Revision 22.4  90/12/12  16:43:57  korotaev
 * ������ ��� ���
 * 
 * Revision 22.3  90/11/12  19:09:49  root
 * ����� ���� ��� ��1425 � ����������� include.
 * 
 * Revision 22.2  89/04/27  13:46:50  korotaev
 * ��������� ��������� � ��������� ������������ ��������� � ������
 * 
 * Revision 22.1  89/04/12  14:27:16  korotaev
 * "param.h" ==> <sys/param.h>
 * 
 * Revision 22.0  89/03/25  12:28:38  korotaev
 * ������ ���������� ������ 2.2
 * 
 * Revision 1.10  89/03/24  22:05:52  korotaev
 * ����� unifdef'a.
 * 
 * Revision 1.9  89/03/24  15:49:53  korotaev
 * ???
 * 
 * Revision 1.8  88/12/22  16:08:53  root
 * ������� ������ ��� ������ � 8-������ �������.
 * (@VG).
 * 
 * Revision 1.7  88/01/21  11:48:12  korotaev
 * ��������� � �-85.
 * 
 * Revision 1.6  87/03/05  14:06:18  alex
 * ������ �������� �������� ���������� �������� � ��
 * ��������� �� ������ ���������� ��� ��������������
 * ����������� ����� � �������� �����������.
 * ���������� ���������m KL_ROUND
 * 
 * Revision 1.5  87/02/06  21:41:22  alex
 * ���������� ������ � attach.
 * 
 * Revision 1.4  86/12/30  14:43:12  alex
 * ��� �������� ������ ������ "RAW" ��� ����, ����� �������������
 * �������� ����, ��� ���������, �� �������� �� ��������� �������� �� �����
 * ������.
 * 
 * Revision 1.3  86/10/29  03:13:41  alex
 * 1. ������� ���������� ���������� ��� �������� ���������
 *    (��� ������ � ����������� ������ �� ������ �����).
 * 2. ������ �������� �� ������������ ��� �����. 
 *    � ���� ������ ������< �������� � �������������, �� ��������
 *    (��� �������������� ������ escape-�������������������)>
 * 	������ �.�.
 * 
 * Revision 1.2  86/10/08  21:53:57  root
 * ������� ������ ��� ���.
 * 
 * Revision 1.1  86/04/19  17:54:33  avg
 * Initial revision
 * 
 */

#include "h/kl.h"
#include <sys/param.h>
#include <sys/conf.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/tty.h>
#include <sys/systm.h>
#include <sys/file.h>
#include "../include/klreg.h"
#include "h/video.h"

#if NKL > 0

extern  struct  dldevice *KLADDR;
/*
 * Normal addressing:
 * minor 0 addresses KLADDR
 * minor 1 thru n-1 address from KLBASE (0176600),
 *    where n is the number of additional KL11's
 * minor n on address from DLBASE (0176500)
 */

struct  tty kl11[NKL];
int     nkl11   = NKL;          /* for pstat */
int     klstart();
int     ttrstrt();
extern  char    partab[];

klattach(addr, unit)
struct dldevice *addr;
{
	if ((unsigned) unit < NKL) {
		kl11[unit].t_addr = addr;
		return(1);
	}
	return(0);
}

/*ARGSUSED*/
klopen(dev, flag)
dev_t   dev;
{
	register struct dldevice *addr;
	register struct tty *tp;
	register d;

	d = klminor(dev);
	tp = &kl11[d];
	if ((d == 0) && (tp->t_addr == 0))
		tp->t_addr = KLADDR;
	if ((d >= NKL) || ((addr = tp->t_addr) == 0)) {
		u.u_error = ENXIO;
		return;
	}
	tp->t_oproc = klstart;
	if ((tp->t_state & ISOPEN) == 0) {
		tp->t_flags = ANYP | RAW /* ECHO | LCASE | CRMOD */;
		tp->t_line = DFLT_LDISC;
		ttychars(tp);
	} else if(tp->t_state & XCLUDE && u.u_uid != 0) {
		u.u_error = EBUSY;
		return;
	}
#ifdef KL_MODEM
	if( MODEM(dev) && (tp->t_state & CARR_ON)==0 ){
		addr->dlrcsr |= DL_DTR | DL_DIE;
		while ((tp->t_state & CARR_ON)==0)
			sleep((caddr_t) &tp->t_rawq, TTIPRI);
	}
#endif KL_MODEM
	tp->t_state |= CARR_ON;
	addr->dlrcsr |= DL_RIE | DL_DTR | DL_RTS;
	addr->dlxcsr |= DLXCSR_TIE;
	ttyopen(dev, tp);
}

/*ARGSUSED*/
klclose(dev, flag)
dev_t   dev;
int     flag;
{
	register struct dldevice *addr;
	register struct tty *tp;
	tp = &kl11[klminor(dev)];
	addr = (struct dldevice *) tp->t_addr;
	ttyclose(&kl11[klminor(dev)]);
	addr->dlrcsr = addr->dlxcsr = 0;
}

klread(dev)
dev_t   dev;
{
	register struct tty *tp;

	tp = &kl11[klminor(dev)];
	(*linesw[tp->t_line].l_read)(tp);
}

klwrite(dev)
dev_t   dev;
{
	register struct tty *tp;

	tp = &kl11[klminor(dev)];
	(*linesw[tp->t_line].l_write)(tp);
}

klxint(dev)
dev_t   dev;
{
	register struct tty *tp;

	tp = &kl11[klminor(dev)];
	ttstart(tp);
	if (tp->t_state & ASLEEP && tp->t_outq.c_cc <= TTLOWAT(tp))
			wakeup((caddr_t) &tp->t_outq);
}

#ifndef KL_ROUND
klrint(dev)
dev_t   dev;
{
	register int c;
	register struct dldevice *addr;
	register struct tty *tp;
#ifdef KL_MODEM
	int c1;
#endif KL_MODEM

	tp = &kl11[klminor(dev)];
	addr = (struct dldevice *) tp->t_addr;
#ifdef KL_MODEM
	c1 = addr->dlrcsr;
#endif KL_MODEM
	c = addr->dlrbuf;
	addr->dlrcsr |= DL_RE;
#ifdef KL_MODEM
	if( c1 & DL_DSC && tp->t_dev & 0200 )
		klmodem(c1,tp);
#endif KL_MODEM
	if ( (c&(0177400))!=(DLRBUF_OVR|DLRBUF_ERR))
	{
		(*linesw[tp->t_line].l_input)(c&0377, tp);
	}/* else printf("kl overflow %o\n", c);  */
}
#else

#ifdef	MENLO_KOV
#define INTR_ARGS(ps)  ,i_sp, i_r1, i_ov, i_nps, i_r0, i_pc, ps
#else
#define INTR_ARGS(ps)  ,i_sp, i_r1, i_nps, i_r0, i_pc, ps
#endif

struct clist kl_inq[NKL];
int kl_ichars;
klrint(dev INTR_ARGS(ps))
dev_t   dev;
{
	register int c;
	register struct tty *tp;
	dev = klminor(dev);
	{
		register struct dldevice *addr;
#ifdef KL_MODEM
		int     c1;
#endif KL_MODEM

		tp = &kl11[dev];
		addr = (struct dldevice *) tp->t_addr;
#ifdef KL_MODEM
		c1 = addr->dlrcsr;
#endif KL_MODEM
		c = addr->dlrbuf;
		addr->dlrcsr |= DL_RE;
#ifdef KL_MODEM
		if( MODEM(tp->t_dev) && (c1 & DL_DSC) )
			klmodem(c1,tp);
#endif KL_MODEM
		if ( (c&(0177400))==(DLRBUF_OVR|DLRBUF_ERR)) return;
		c &= 0377;
		if( kl_ichars++ ) {
			if( putc(c, &kl_inq[dev])) kl_ichars--;
			return;
		}
	}
	{
		register int i;
		int s, chars=0;
		i = dev;
		s = spl1();
		goto c_input;
next:
		for(chars=0,i=0;i<NKL && kl_ichars; i++)
			if(kl_inq[i].c_cc) {
				s=spl1();
				tp = &kl11[i];
				c = getc(&kl_inq[i]);
c_input:
				(*linesw[tp->t_line].l_input)(c, tp);
				chars++;
				splx(s);
				kl_ichars--;
			}
		if( kl_ichars && chars) goto next;
		kl_ichars=0;
	}
	return;
}
#endif

#ifdef KL_MODEM
/*
 * if modem and interrapt by change modem status
 */
klmodem(c,tp)
register struct tty *tp;
register c;
{
	register s;

	s = spl1();
	wakeup((caddr_t)&tp->t_rawq);
	if( c & DL_CD)         /* carrier detect ON */
		tp->t_state |= CARR_ON;
	else {                  /* carrier detect OFF */
		if( (tp->t_local & LNOHANG)==0 && (tp->t_pgrp > 2 ) ){
			gsignal(tp->t_pgrp, SIGHUP);
			flushtty(tp, FREAD|FWRITE);
		}
		tp->t_state &= ~CARR_ON;
	}
	splx(s);
}
#endif KL_MODEM

klioctl(dev, cmd, addr, flag)
caddr_t addr;
dev_t   dev;
{
	register struct tty *tp;
	register struct dldevice *addr1;

	tp = &kl11[klminor(dev)];
	addr1 = (struct dldevice *) tp->t_addr;
	switch (ttioctl(&kl11[klminor(dev)], cmd, addr, flag)) {
		case TIOCSETN:
		case TIOCSETP:
		case TIOCSETA:
		case TIOCSETB:
		    break;
#ifdef KL_MODEM
		case TIOCSBRK:
		    if( MODEM(tp->t_dev) )
			    addr1->dlxcsr |= DLXCSR_BRK;
		    break;
		case TIOCCBRK:
		    if( MODEM(tp->t_dev) )
			    addr1->dlxcsr &= ~DLXCSR_BRK;
		    break;
		case TIOCSDTR:
		    if( MODEM(tp->t_dev) )
			    addr1->dlrcsr |= DL_DTR | DL_DIE;
		    break;
		case TIOCCDTR:
		    if( MODEM(tp->t_dev) )
			    addr1->dlrcsr &= ~(DL_DTR | DL_DIE);
		    break;
#endif KL_MODEM
		case 0:
			break;
		default:
			u.u_error = ENOTTY;
	}
}

klstart(tp)
register struct tty *tp;
{
	register c;
	register struct dldevice *addr;

	addr = (struct dldevice *) tp->t_addr;
	if ((addr->dlxcsr & DLXCSR_TRDY) == 0)
		return;
#ifdef KL_MODEM
	if( MODEM( tp->t_dev ) )
		while( (addr->dlrcsr & DL_CTS) == 0 )
			sleep((caddr_t) &tp->t_rawq, TTIPRI);
#endif KL_MODEM
	if ((c=getc(&tp->t_outq)) >= 0) {
		if ((tp->t_lstate&L8BITS))
			addr->dlxbuf = c;
		else if (c<=0177)
			addr->dlxbuf = c | (partab[c] & 0200);
		else {
			timeout(ttrstrt, (caddr_t)tp, (c & 0177) + DLDELAY);
			tp->t_state |= TIMEOUT;
		}
	}
}

#endif NKL

char *msgbufp = msgbuf;         /* Next saved printf character */
/*
 * Print a character on console (or users terminal if touser).
 * Attempts to save and restore device
 * status.
 * If the last character input from the console was a break
 * (null or del), all printing is inhibited.
 *
 * Whether or not printing is inhibited,
 * the last MSGBUFS characters
 * are saved in msgbuf for inspection later.
 */

#ifdef  UCB_UPRINTF
putchar(c, touser)
#else
putchar(c)
#endif
register c;
{
	register s;
#if NKL > 0
	register struct dldevice *kladdr = KLADDR;
#endif
	long    timo;
	extern  char *panicstr;

#ifdef  UCB_UPRINTF
	if (touser) {
		register struct tty *tp = u.u_ttyp;
		if (tp && (tp->t_state & CARR_ON)) {
			register s = spl6();

			if (c == '\n')
				(*linesw[tp->t_line].l_output)('\r', tp);
			if ((tp->t_flags & LCASE) && 'a'<=c && c<='z')
				c -= 040;
			(*linesw[tp->t_line].l_output)(c, tp);
			ttstart(tp);
			splx(s);
		}
		return;
	}
#endif
	if (c != '\0' && c != '\r' && c != 0177) {
		*msgbufp++ = c;
		if (msgbufp >= &msgbuf[MSGBUFS])
			msgbufp = msgbuf;
	}

#if NKL > 0
	/* Map lower case to upper on console */
	if ('a'<=c && c<='z')
		c -= 040;
	/*
	 *  If last char was a break or null, don't print
	 */
	if (panicstr == (char *) 0)
		if ((kladdr->dlrbuf & 0177) == 0)
			return;
	timo = 60000L;

	/*
	 * Try waiting for the console tty to come ready,
	 * otherwise give up after a reasonable time.
	 */
	while((kladdr->dlxcsr & DLXCSR_TRDY) == 0)
		if (--timo == 0L)
			break;
	if (c == 0)
		return;
	s = kladdr->dlxcsr;
	kladdr->dlxcsr = 0;
	kladdr->dlxbuf = c;
	if (c == '\n') {
#ifdef  UCB_UPRINTF
		putchar('\r', 0);
#ifndef DVK4
		putchar(0177, 0);
		putchar(0177, 0);
#endif DVK4
#else
		putchar('\r');
#ifndef DVK4
		putchar(0177);
		putchar(0177);
#endif DVK4
#endif
	}
#ifdef  UCB_UPRINTF
	putchar(0, 0);
#else
	putchar(0);
#endif
	kladdr->dlxcsr = s;
#endif NKL
#if NVI > 0
	viputchar(c);
#endif
}

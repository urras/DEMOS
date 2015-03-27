/*
 * LP-11 Line printer driver
 *
 * This driver has been modified to work on printers where
 * leaving LP_IE set would cause continuous interrupts.
 *
 * $Log:	lp.c,v $
 * Revision 22.3  90/11/12  19:10:53  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.2  89/04/27  13:48:17  korotaev
 * Изменения связанные с небольшим перемещением каталогов и файлов
 * 
 * Revision 22.1  89/04/12  14:28:56  korotaev
 * "param.h" ==> <sys/param.h>
 * 
 * Revision 22.0  89/03/25  12:29:43  korotaev
 * Отсюда начинается версия 2.2
 * 
 * Revision 1.10  88/12/16  16:32:19  avg
 * Очередная правка на обработку timeout:
 * 	теперь будет отваливаться в timeout с запрещенными прерываниями
 * 	в случае возникновении ERROR бита - на некоторых интерфейцах
 * 	в этом случае сыплет прерываниями.
 * 
 * Revision 1.9  88/11/01  20:39:54  dvolodin
 * Что сделано и кем сделано - непонятно.
 * 
 * Revision 1.8  87/07/24  14:04:32  alex
 * Внес две правки из-за PRINTRONIX:
 * 1. Были переставлены spl0 на место в открытии!!
 * 2. При выходе из прерывания бит разрешения ставится всегда, если нет
 * еще готовности устройства. Иначе получалось, что за время накопления
 * очередного буфера печать не успевала пройти, и при lpoutput драйвер
 * оставался в режиме ожидания конца печати с закрытым битом
 * разрешения прерываний.
 *
 * Revision 1.7  87/07/06  19:10:49  alex
 * Все те же тайм-ауты.
 *
 * Revision 1.6  87/05/12  22:12:20  alex
 * Увеличен тайм-аут для работы с устройствами,
 * имеющими большие буфера (до 30 секунд).
 *
 * Revision 1.5  87/01/05  21:32:28  alex
 * Исправлена ошибка, приводившая к зависанию при
 * выключении устройства на некоторых выдающихся
 * экземплярах.
 *
 * Revision 1.4  86/05/30  15:03:40  avg
 * При закрытии драйвер теперь ожидает опустошения очереди вывода.
 *
 * Revision 1.3  86/05/13  17:13:29  avg
 * Нормально сделаны таймауты и доделана проверка готовности
 * устройства на готовность при открытии.
 *
 * Revision 1.2  86/05/06  21:39:40  avg
 * Сделаны правки под русские буквы.
 * Переключение флагов драйвера теперь делается
 * через ioctl  LPIOSETP и LPIOGETP.
 *
 * Revision 1.1  86/04/19  17:54:35  avg
 * Initial revision
 *
 */

#include "h/lp.h"
#if NLP > 0
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/tty.h>
#include <sys/file.h>
#include "../include/lpreg.h"

#define LPPRI   (PZERO + 8)
#define LPPRI2  (PZERO + 21)
#define LPLWAT  40
#define LPHWAT  400

#define MAXCOL  160

#define LPUNIT(dev) (minor(dev))

struct lp_softc {
	struct  clist sc_outq;
	int     sc_state;
	int     sc_physcol;
	int     sc_logcol;
	int     sc_physline;
	int     sc_flags; /* Flags are in ioctl.h */
	int     sc_lpchar;
	int     sc_ejline;
	int     sc_maxcol;
	int     sc_timout;      /* Тиков для таймаута */
				/* содержит -1 в случае, если таймаут
				 * не нужен */
} lp_softc[NLP];

/* bits for state */
#define OPEN            1       /* Принтер открыт */
#define ASLP            2       /* Кто-то ждет опустошения очереди */
#define OREG            4       /* Включен русский выходной регистр */
#define TIMER           8       /* Идет таймаут по открытию */
#define CLOSING        16       /* Печатающее у-во закрывается */

extern  lbolt;
struct  lpdevice *lp_addr[NLP];
int     lptout(), lptopen();
int     lptactive;

lpattach(addr, unit)
struct lpdevice *addr;
{
	if ((unsigned) unit >= NLP)
		return(0);
	lp_addr[unit] = addr;
	return(1);
}

lpopen(dev, flag)
dev_t   dev;
int     flag;
{
	register int    unit;
	register struct lp_softc *sc;

	if (((unit = LPUNIT(dev)) >= NLP) || (lp_addr[unit] == 0)) {
		u.u_error = ENXIO;
		return;
	}
	if ((flag & FWRITE) == 0 )
		return;
	if ((sc = &lp_softc[unit])->sc_state & OPEN) {
		u.u_error = EBUSY;
		return;
	}
	if ((lp_addr[unit]->lpcs & (LP_RDY|LP_ERR)) != LP_RDY) {
		u.u_error = EIO;
		return;
	}

/* Check if device is off but set LP_RDY bit */
	lp_addr[unit]->lpcs = LP_IE;
	sc->sc_state &= ~ASLP;
	sc->sc_state |= TIMER;
	sc->sc_timout = -1;
	(void) _spl4();
	timeout(lptopen, (caddr_t)sc, hz/2);
	lp_addr[unit]->lpdb = 0;
	sleep(sc, LPPRI);
	(void) _spl0();
	if ((lp_addr[unit]->lpcs & (LP_RDY|LP_ERR)) != LP_RDY) {
		lp_addr[unit]->lpcs = 0;
		u.u_error = EIO;
		return;
	}

	sc->sc_state &= ~(CLOSING|TIMER);
	sc->sc_state |= OPEN;
	sc->sc_physline = 0;
	if( sc->sc_maxcol < 10 )
		sc->sc_maxcol = MAXCOL;
	(void) _spl4();
	if (!lptactive) {
		lptactive++;
		timeout(lptout, 0, 2 * hz);
	}
	(void) _spl0();
	sc->sc_logcol  = 0;
	sc->sc_physcol = 1;
	if (!(sc->sc_flags & LPRAW))
		lpcanon(dev, '\r');
}

/*
 * Таймаут при открытии печатающего у-ва
 */
lptopen(lp11)
register struct lp_softc *lp11;
{
	if (lp11->sc_state & TIMER) {
		lp11->sc_state &= ~TIMER;
		wakeup((caddr_t)lp11);
	}
}

lpclose(dev, flag)
dev_t   dev;
int     flag;
{
	register struct lp_softc *sc = &lp_softc[LPUNIT(dev)];

	if( (flag & FWRITE) == 0 )
		return;
	if (!(sc->sc_flags & LPRAW) && sc->sc_physline != 0 )
		lpoutput(dev, '\f');
	sc->sc_state |= CLOSING;
	sc->sc_state &= ~ASLP;
	sleep(sc, LPPRI);
	if (sc->sc_outq.c_cc > 0) {
		(void) _spl4();
		lpintr(LPUNIT(dev));
		(void) _spl0();
	}
	sc->sc_timout = -1;
}

lpwrite(dev)
dev_t   dev;
{
	register c;

	while (u.u_count) {
		c = fubyte(u.u_base++);
		if (c < 0) {
			u.u_error = EFAULT;
			break;
		}
		u.u_count--;
		lpcanon(dev, c);
	}
}

lpcanon(dev, c)
dev_t   dev;
register c;
{
	register logcol, physcol;
	register struct lp_softc *sc = &lp_softc[LPUNIT(dev)];

	if (sc->sc_flags & LPRAW) {
		lpoutput(dev, c);
		return;
	}
	c &= 0377;
	if ( (sc->sc_flags & LPCYRILL) == 0 )
		c &= 0177;
#ifdef Ucode
	else
		c = conv_out(c);
#endif
	if ( c >= 0200 && c < 0300 )
		return;
	if ((sc->sc_flags & LPCAP) == 0) {
		register c2;

		if (c >= 'a' && c <= 'z')
			c += 'A'-'a';
		else if (c==0177 && (sc->sc_flags & (LPZNAK|LPBAR)))
			c = '#';
		else if ((c&0300) == 0300) {
			c |= 040;
			c &= 0177;
			if(c==0177 && !(sc->sc_flags & LPZNAK))
				c = 'x';        /* Зто мягкий знак в КОИ-8 */
		} else switch (c) {

			case '{':
				c2 = '(';
				goto esc;

			case '}':
				c2 = ')';
				goto esc;

			case '`':
				c2 = '\'';
				goto esc;

			case '|':
				if(sc->sc_flags & LPBAR) {
					c = 0177;
					break;
				}
				c2 = '!';
				goto esc;

			case '~':
				c2 = '^';

			esc:
				lpcanon(dev, c2);
				sc->sc_logcol--;
				c = '-';
		}
	} else {
		if( c == 0377 && !(sc->sc_flags & LPZNAK) )
			c = 0337;       /* маленький твердый знак */
		if( sc->sc_flags & LPSISO ) {
			if( (c&0300) == 0300 && !(sc->sc_state & OREG) ) {
				lpoutput(dev, CRUS);
				sc->sc_state |= OREG;
			} else
			if( ((c&0300) == 0100 || c == 033 ) &&
			    (sc->sc_state & OREG) ) {
				lpoutput(dev, CLAT);
				sc->sc_state &= ~OREG;
			}
			c &= 0177;
		}
	}
	logcol = sc->sc_logcol;
	physcol = sc->sc_physcol;
	if (c == ' ')
		logcol++;
	else switch(c) {

		case '\t':
			logcol = (logcol + 8) & ~7;
			break;

		case '\f':
			if (sc->sc_physline == 0 && physcol == 0)
				break;
			/* fall into ... */

		case '\n':
			if (c == '\f') {
				sc->sc_physline = 0;
				lpoutput(dev, c);
			} else {
				sc->sc_physline++;
				if( (sc->sc_flags & LPEJECT) &&
				     sc->sc_ejline == sc->sc_physline ) {
					lpoutput(dev, '\f');
					sc->sc_physline = 0;
				} else
					lpoutput(dev, c);
			}
			physcol = 0;
			/* fall into ... */

		case '\r':
			logcol = 0;
			(void) _spl4();
			lpintr(LPUNIT(dev));
			(void) _spl0();
			if( (sc->sc_flags & (LPSISO|LPCYRILL|LPCAP)) ==
					    (LPSISO|LPCYRILL|LPCAP) ) {
				lpoutput(dev, CLAT);
				sc->sc_state &= ~OREG;
			}
			break;

		case '\b':
			if (logcol > 0)
				logcol--;
			break;

		default:
			if (logcol < physcol) {
				lpoutput(dev, '\r');
				physcol = 0;
			}
			if (logcol < sc->sc_maxcol) {
				while (logcol > physcol) {
					lpoutput(dev, ' ');
					physcol++;
				}
				lpoutput(dev, c);
				physcol++;
			}
			logcol++;
	}
	if (logcol > 1000)      /* ignore long lines  */
		logcol = 1000;
	sc->sc_logcol = logcol;
	sc->sc_physcol = physcol;
}

lpoutput(dev, c)
dev_t   dev;
int     c;
{
	register struct lp_softc *sc = &lp_softc[LPUNIT(dev)];

	if (sc->sc_outq.c_cc >= LPHWAT) {
		(void) _spl4();
		lpintr(LPUNIT(dev));                            /* unchoke */
		while (sc->sc_outq.c_cc >= LPHWAT) {
			sc->sc_state |= ASLP;           /* must be LP_ERR */
			sleep((caddr_t)sc, LPPRI2);
		}
		(void) _spl0();
	}
	while (putc(c, &sc->sc_outq))
		sleep((caddr_t)&lbolt, LPPRI);
}

lpintr(lp11)
int     lp11;
{
	register n;
	register struct lp_softc *sc = &lp_softc[lp11];
	register struct lpdevice *lpaddr;
	int s;

	/* Device is opened */
	if(sc->sc_state & TIMER) {
		sc->sc_state &= ~TIMER;
		wakeup((caddr_t)sc);
		return;
	}

	lpaddr = lp_addr[lp11];
	if ( !(lpaddr->lpcs&LP_RDY)) return;   /* Пусть timeout разбирается */
	lpaddr->lpcs &= ~LP_IE;
	s = spl1();
	n = sc->sc_outq.c_cc;
	if (sc->sc_lpchar < 0)
		sc->sc_lpchar = getc(&sc->sc_outq);
	while ((lpaddr->lpcs & (LP_RDY|LP_ERR)) == LP_RDY &&
	       sc->sc_lpchar >= 0) {
		lpaddr->lpdb = sc->sc_lpchar;
		sc->sc_lpchar = getc(&sc->sc_outq);
	}
	sc->sc_timout = 30 * hz;
	splx(s);
	if ((sc->sc_outq.c_cc > 0 || sc->sc_lpchar >= 0)&&
	      (lpaddr->lpcs & LP_ERR) == 0      )
		lpaddr->lpcs |= LP_IE;  /* ok and more to do later */
	if ( !(sc->sc_state & ASLP) ) {
		if((sc->sc_lpchar < 0) && (sc->sc_state & CLOSING)) {
			sc->sc_state &= ~(OPEN|CLOSING);    /* no longer open */
			sc->sc_timout = -1;
			lpaddr->lpcs = 0;
			wakeup(sc);
		}
		return;
	}
	if ((n>LPLWAT && sc->sc_outq.c_cc<=LPLWAT) || sc->sc_outq.c_cc==0) {
		sc->sc_state &= ~ASLP;
		wakeup((caddr_t)sc);            /* top half should go on */
	}
}

lptout()
{
	register struct lp_softc *sc;
	register struct lpdevice *lpaddr;
	int unit;

	for( unit = 0 ; unit < NLP ; unit++ ) {
		lpaddr = lp_addr[unit];
		sc = &lp_softc[unit];

		if (sc->sc_state && sc->sc_timout < 0)
			continue;
		sc->sc_timout -= 2*hz;

		if( sc->sc_timout <= 0 ) {
			sc->sc_timout = -1;
			if (sc->sc_outq.c_cc) {
				if( (lpaddr->lpcs & (LP_ERR|LP_RDY)) == LP_RDY )
					lpintr(unit);       /* ready to go */
				else {
					sc->sc_flags |= LPERR;
					sc->sc_timout = 5*hz;
				}
			 } else if (sc->sc_state & CLOSING) {
				sc->sc_state &= ~(OPEN|CLOSING); /* no longer open */
				lpaddr->lpcs = 0;
				wakeup(sc);
			}
		}
	}
	timeout(lptout, 0, 2*hz);
}

lpioctl(dev, cmd, addr, flag)
caddr_t addr;
{
	register struct lp_softc *sc = &lp_softc[LPUNIT(dev)];
	struct lpmodes LPM;
	register struct lpdevice *lpaddr;

	register i;
#       define LPMASK (LPCAP|LPRAW|LPCYRILL|LPSISO|LPEJECT|LPZNAK|LPBAR)

	switch( cmd ) {

	case LPIOSETP:
		if ( !(flag & FWRITE) && (sc->sc_state & OPEN) ) {
			u.u_error = EBUSY;
			break;
		}
		if ( copyin(addr, (caddr_t)&LPM, sizeof LPM) ) {
			u.u_error = EFAULT;
			break;
		}
		sc->sc_flags &= ~LPMASK;
		sc->sc_flags |= LPM.lpm_flags & LPMASK;
		i = LPM.lpm_ejline;
		if( i < 0 || i > 256 )
			i = 0;
		sc->sc_ejline = i;
		i = LPM.lpm_maxcol;
		if( i > MAXCOL || i < 10 )
			i = MAXCOL;
		sc->sc_maxcol = i;
		break;

	case LPIOGETP:
		lpaddr = lp_addr[LPUNIT(dev)];
		i = sc->sc_flags & (LPMASK|LPERR);
		if( sc->sc_state & OPEN )
			i |= LPOPEN|LPON;
		else if( lpaddr->lpcs & LP_RDY )
			i |= LPON;
		if( lpaddr->lpcs & LP_ERR )
			i &= ~LPON;
		LPM.lpm_flags = i;
		LPM.lpm_ejline = sc->sc_ejline;
		LPM.lpm_maxcol = sc->sc_maxcol;
		if ( copyout((caddr_t)&LPM, addr, sizeof LPM) )
			u.u_error = EFAULT;
		else if( flag & FWRITE )
			sc->sc_flags &= ~LPERR;
		break;

	default:
		u.u_error = ENOTTY;
	}
}

#endif NLP

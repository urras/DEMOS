/*
 * Мультиплексирование линии связи - "Мультиплексная line-discipline"
 *
 * $Log:        mpy.c,v $
 * Revision 1.1  89/04/27  13:49:08  korotaev
 * Изменения связанные с небольшим перемещением каталогов и файлов
 *
 *
 * Типы символов:
 *
 * 0-0177,0221-0377 - обычный (регулярный) символ
 * 0201-0220        - номер канала
 * 0200             - символ экранирования
 * 0221             - экранированный CTRL/Q
 * 0222             - экранированный CTRL/S
 */

#include "h/mpy.h"
	/* в mpy.h должны быть константы: NMPY      - число mpy - линий
					  NMPYCHANS - число каналов в линии
	 */

#if NMPY > 0

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/tty.h>
#include <sys/conf.h>
#include <sys/file.h>

#define MLESC           02000   /* "escape catched" local state flag */

#define MPY_ESCAPE      0200
#define MPY_CHAN(n)     ((n)+0201)

#define MPY_XON         0221    /* quoted XON  */
#define MPY_XOFF        0222    /* quoted XOFF */

#if NMPYCHANS > 16
# undef  NMPYCHANS
# define NMPYCHANS 16
#endif

#define XON     ('Q'&037)
#define XOFF    ('S'&037)

/*
 * Структуры mpy-терминалов
 */
struct tty mpytty[NMPY*NMPYCHANS];
int nmpytty = NMPY*NMPYCHANS;

/*
 * Указатели на структуру tty линии (если NULL - mpy-терминал свободен)
 */
struct tty *mpyline[NMPY];

static mtystart();
static mtytproc();

#define mline(dev)    ((int)minor(dev)/NMPYCHANS)    /* номер линии */
#define mchan(dev)    ((int)minor(dev)%NMPYCHANS)    /* номер канала */
#define ttno(dev)     minor(dev)

/*
 * Открытие mpy-терминала
 */
mtyopen(dev, flag)
dev_t dev;
int   flag;
{
	short unit = mline(dev);
	register struct tty *tp;

	if( unit >= NMPY ) {
		u.u_error = ENXIO;
		return;
	}
	tp = &mpytty[ttno(dev)];
	if( (tp->t_state & XCLUDE) && u.u_uid != 0 ) {
		u.u_error = EBUSY;
		return;
	}
	tp->t_oproc = mtystart;
	tp->t_state |= WOPEN;
	while( mpyline[unit] == NULL )
		sleep((caddr_t)&mpyline[unit], TTIPRI);
	tp->t_state |= CARR_ON;
	if( (tp->t_state & ISOPEN) == 0 ) {
		ttychars(tp);
		tp->t_ispeed = mpyline[unit]->t_ispeed;
		tp->t_ospeed = mpyline[unit]->t_ospeed;
		tp->t_flags = ODDP | EVENP | RAW;
		tp->t_line = DFLT_LDISC;
	}
	ttyopen(dev, tp);
	tp->t_tproc = mtytproc;
	tp->t_optr  = &(mpyline[unit]->t_outq);
}

/*
 * Процедура close драйвера
 */
mtyclose(dev, flag)
dev_t dev;
int   flag;
{
	ttyclose( &mpytty[ttno(dev)] );
}

/*
 * Чтение/запись в у-во mty
 */
mtyread(dev)
dev_t dev;
{
	register struct tty *tp = &mpytty[ttno(dev)];

	(void) (*linesw[tp->t_line].l_read)(tp);
}

mtywrite(dev)
dev_t dev;
{
	register struct tty *tp = &mpytty[ttno(dev)];

	(void) (*linesw[tp->t_line].l_write)(tp);
}

/*
 * Процедура ioctl
 */
mtyioctl(dev, cmd, addr, flag)
dev_t   dev;
caddr_t addr;
int     flag;
{
	switch( ttioctl(&mpytty[ttno(dev)], cmd, addr, flag) ) {
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

/*
 * Процедура вывода символов на устройство mty
 */
mtystart(tp)
register struct tty *tp;
{
	if(tp->t_state & CARR_ON) {
		tp = mpyline[mline(tp->t_dev)];
		ttstart(tp);
		tp->t_state |= ASLEEP;  /* will do wakeup */
	}
}

/*
 * Процедура помещения символа в очередь
 */
mtytproc(c, tp)
register struct tty *tp;
register c;
{
	register struct tty *ltp = mpyline[mline(tp->t_dev)];
	extern struct cblock *cfreelist;
	int chan, chanswitched;
	static time_t lasttime = -1l;
	extern time_t time;

	if( cfreelist == NULL ) /* I wanna be sure about space for the following bytes */
		return c;
	chan = mchan(tp->t_dev);
	chanswitched = 0;
	if( (unsigned)(time-lasttime) > 3 || (ltp->t_mpochan != chan) ) {
		putc(MPY_CHAN(chan), &ltp->t_outq);
		ltp->t_mpochan = chan;
		chanswitched++;
	}
	if( MPY_ESCAPE <= c && c <= MPY_XOFF )
		putc(MPY_ESCAPE, &ltp->t_outq);
	if(      c == XON  ) c = MPY_XON;
	else if( c == XOFF ) c = MPY_XOFF;
	putc(c, &ltp->t_outq); /* Can not fail! */
	lasttime = time;
	if( chanswitched )
		ttstart(ltp);   /* We got channel switched -- gogo! */
	return 0;
}

/*
 * Процедура ввода символа в мультиплексной дисциплине
 */
mpyinput(c, tp)
register struct tty *tp;
register c;
{
	register struct tty *mtp;

	c &= 0377;
	if( c == XON ) {
		if( tp->t_state & TTSTOP ) {
			tp->t_state &= ~TTSTOP;
			tp->t_state |= ASLEEP;      /* always do wakeup */
			ttstart(tp);
		}
		return;
	}
	if( c == XOFF ) {
		if( !(tp->t_state & TTSTOP) ) {
			tp->t_state |= TTSTOP;
			(*cdevsw[major(tp->t_dev)].d_stop)(tp, 0);
		}
		return;
	}
	if( tp->t_lstate & MLESC ) {
		tp->t_lstate &= ~MLESC;
		goto escaped;
	}
	if( c == MPY_ESCAPE ) {
		tp->t_lstate |= MLESC;
		return;
	}
	if( MPY_CHAN(0) <= c && c < MPY_CHAN(NMPYCHANS) ) {
		c -= MPY_CHAN(0);
		tp->t_mpichan = c;
		return;
	}
	if(      c == MPY_XON  ) c = XON;
	else if( c == MPY_XOFF ) c = XOFF;
escaped:
	mtp = &mpytty[tp->t_mpichan + tp->t_mpline*NMPYCHANS];
	if( !(mtp->t_state & ISOPEN) )
		return;         /* No such channel, alas */
	if( !(tp->t_state & CARR_ON) )
		mtp->t_state &= ~CARR_ON;
	(*linesw[mtp->t_line].l_input)(c, mtp);
}

/*
 * Процедура, вызываемая для переключения на
 * мультиплексную дисциплину
 */
mpyopen(tp)
register struct tty *tp;
{
	int i;

	tp->t_lstate |= L8BITS;         /* Just 8 bits i/o supported */
	tp->t_lstate &= ~MLESC;
	tp->t_flags   = RAW|TANDEM;
	tun.t_startc = XON;
	tun.t_stopc  = XOFF;
	tp->t_mpochan = -1;
	tp->t_mpichan = 0;
	if( tp->t_line != MPYLDISC )
		flushtty(tp, FREAD|FWRITE);
	/*
	 * Search for free lines
	 */
	for( i = 0 ; i < NMPY ; i++ ) {
		if( mpyline[i] == 0 )
			goto Found;
	}
	u.u_error = ENXIO;
	return;
Found:
	tp->t_mpline = i;
	mpyline[i] = tp;
	wakeup((caddr_t)&mpyline[i]);   /* for sleeps in mtyopen */
}

mpyclose(tp)
register struct tty *tp;
{
	register struct tty *mtp;
	int i;

	mpyline[tp->t_mpline] = NULL;
	tp->t_lstate &= ~(L8BITS|MLESC);
	mtp = &mpytty[tp->t_mpline * NMPYCHANS];
	for( i = 0 ; i < NMPYCHANS ; i++, mtp++ ) {
		mtp->t_state &= ~(CARR_ON|BUSY|TIMEOUT|TTSTOP);
		mtp->t_outq.c_cc = 0;
		mtp->t_outq.c_cf = NULL;
		mtp->t_outq.c_cl = NULL;
		mtp->t_optr = &(mtp->t_outq);
		mtp->t_tproc = NULL;
		if( (mtp->t_state & WOPEN)   == 0  &&
		    (mtp->t_local & LNOHANG) == 0  &&
		    (mtp->t_pgrp > 2) )
			gsignal(mtp->t_pgrp, SIGHUP);
		flushtty(mtp, FREAD|FWRITE);
	}
	tp->t_rawq.c_cc = 0;
	tp->t_rawq.c_cf = NULL;
	tp->t_rawq.c_cl = NULL;
	tp->t_canq.c_cc = 0;
	tp->t_canq.c_cf = NULL;
	tp->t_canq.c_cl = NULL;
}

/*
 * Процедура для l_read
 */
mpyread(tp)
struct tty *tp;
{
	u.u_error = EIO;
}

/*
 * Процедура для l_write
 */
mpywrite(tp)
struct tty *tp;
{
	u.u_error = EIO;
}

/*
 * Процедура для l_ioctl
 */
mpyioctl(tp, com, addr, flag)
register struct tty *tp;
caddr_t addr;
{
	short c;

	if( com == MPYGETL ) {
		c = tp->t_mpline;
		if( copyout((caddr_t)&c, addr, sizeof(short)) )
			u.u_error = EFAULT;
		return 0;
	}
	switch(com) {
	    case TIOCSETA:
		com = TIOCSETB; /* Do not flush any buffers */
	    case TIOCSETB:
		break;
	    case TIOCSETP:
		com = TIOCSETN;
	    case TIOCSETN:
		break;
	    case TIOCGETA:
	    case TIOCGETP:
	    case TIOCSETD:
	    case TIOCGETD:
		break;
	default:
		u.u_error = ENOTTY;
		return 0;
	}
	return com;
}
#endif NMPY

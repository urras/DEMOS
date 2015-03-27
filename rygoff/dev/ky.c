/*
 * ������� ����������������� ���������� ��� �����������-85
 *
 * $Log:        ky.c,v $
 * Revision 22.3  89/04/27  13:47:40  korotaev
 * ��������� ��������� � ��������� ������������ ��������� � ������
 *
 * Revision 22.2  89/04/12  14:28:18  korotaev
 * "param.h" ==> "param.">
 *
 * Revision 22.1  89/04/06  16:48:11  avg
 * �������� Log ��� rcs.
 *
 */
#include "ky.h"
#if NKY > 0
#include "param.h"
#include <sys/conf.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/tty.h>
#include <sys/systm.h>
#include <sys/iopage.h>

/*
 * ����������� ������ ����������������� �����
 */
struct  kydevice {
	short   ky_dbuf; /* ����� ������ */
	short   ky_csrA; /* �������/��������� A */
	short   ky_res;  /* ������������ */
	short   ky_csrB; /* �������/��������� B */
	short   ky_mcr0; /* ������� 0 ���������� ������� */
	short   ky_mcr1; /* ������� 1 ���������� ������� */
	short   ky_brate; /* �������� ������ */
};

/* ������� ������/��������� A */

/* WR0 */
#define KYAWR0_RR      0007    /* ���� ������ �������� */
#define KYAWR0_SA      0010    /* ������� abort */
#define KYAWR0_RESI    0020    /* ����� ��������/���.����. ���������� */
#define KYAWR0_CR      0030    /* ����� ������ */
#define KYAWR0_EINRC   0040    /* ��������� ���������� �� ������ ������� */
#define KYAWR0_RTIP    0050    /* �������� ����������� ����. ����������� */
#define KYAWR0_ER      0060    /* ����� ������ */
#define KYAWR0_EI      0070    /* ����� ���������� */

/* WR1 */
#define KYAWR1_EIE     0001    /* ���������� �������� ���������� */
#define KYAWR1_TIE     0002    /* ���������� ���������� �� ����������� */
#define KYAWR1_RIE0    0010    /* ���������� ���������� 0 �� ��������� */
#define KYAWR1_RIE1    0020    /* ���������� ���������� 1 �� ��������� */

/* WR2 - �� ������������ */

/* WR3 */
#define KYAWR3_RXEN    0001    /* �������� ����� */
#define KYAWR3_SCLH    0002    /* ��������� ����� �������������� ������� */
#define KYAWR3_ASM     0004    /* ����� ������ ������ */
#define KYAWR3_RCE     0010    /* �������� ��� ��������� */
#define KYAWR3_EHP     0020    /* ���� ������������ ����� */
#define KYAWR3_R7BIT   0100    /* ����� ������� �� ������ 7 ��� */
#define KYAWR3_R8BIT   0300    /* ����� ������� �� ������ 8 ��� */

/* WR4 */
#define KYAWR4_PEN     0001    /* �������� �������� �������� */
#define KYAWR4_EP      0002    /* ������ �������� */
#define KYAWR4_1SB     0004    /* 1 �������� ��� */
#define KYAWR4_15SB    0010    /* 1.5 �������� ���� */
#define KYAWR4_2SB     0014    /* 2 �������� ���� */
#define KYAWR4_CM16    0100    /* 16-������� ������� ������� */

/* WR5 */
#define KYAWR5_TXEN    0010    /* �������� ��������� */
#define KYAWR5_T7BIT   0040    /* ����� ������� �� �������� 7 ��� */
#define KYAWR5_T8BIT   0140    /* ����� ������� �� �������� 8 ��� */

/* WR6 � WR7 - ������ ��� ����������� ������ */

/* RR0 */
#define KYARR0_RXCA    0001    /* ������ ������ */
#define KYARR0_INTR    0002    /* ���������� �� ��������� */
#define KYARR0_TBMT    0004    /* ����� �������� ���� */
#define KYARR0_TUEM    0100    /* ���������������� ����������� */
#define KYARR0_B       0200    /* ������ ����� */

/* RR1 */
#define KYARR1_AS      0001    /* ��� ��������� �������� */
#define KYARR1_RXPE    0020    /* ������ �������� ��������� */
#define KYARR1_RXOE    0040    /* ������������ ��������� */
#define KYARR1_FE      0100    /* ������ ������� */

/* ������� ������/��������� B */
/* WR0 */
#define KYBWR0_RP      0007    /* ���� ������ ��������  */

/* WR1 */
#define KYBWR1_ON      0004    /* ������ ���� �������� */

#ifndef NEW
/* RR2 - ������� ������������� ���������� */
#define KYBRR2_V         0034    /* ����� */
#define KY_TBMT          0020    /* ����� ����������� ���� -
				  * (��������! ������ � "Pro350 Handbook") */
#define KY_ESC           0024    /* ��������� ���������/������� ������� */
#define KY_RXCA          0030    /* ������ ������ */
#define KY_SRC           0034    /* ������ �� ������ */

/* ������� 0 ���������� ������� */
#define KYMCR0_105      04      /* ������ ��������                      */
#define KYMCR0_111      010     /* ������������� ��������               */
#define KYMCR0_108      020     /* ��������� ������������ ������ ������ */

#define RB(x) (*(char *)x)
#define RES_IE  073
#define SET_IE  053
#define CLR_INT 033

/*
 * ���� �������� �������� ������
 */

short kyspeed[] = {
	0,
	0,      /*    50 ��� */
	01,     /*    75 ��� */
	02,     /*   110 ��� */
	03,     /*   134 ��� */
	04,     /*   150 ��� */
	04,     /*   200 ��� */
	05,     /*   300 ��� */
	06,     /*   600 ��� */
	07,     /*  1200 ��� */
	010,    /*  1800 ��� */
	012,    /*  2400 ��� */
	014,    /*  4800 ��� */
	016,    /*  9600 ��� */
	017,    /* 19200 ��� */
	017     /*   EXT B    */
};

extern struct kydevice *KYADDR;

struct  tty ky11;
int     nky11 = 1;      /* ��� pstat */
int     kystart();
int     ttrstrt();
extern  char    partab[];
static kyinited = 0;

#define KYDELAY                04

static kyinit()
{
	register struct kydevice *addr = KYADDR;
	int timf;

	addr->ky_csrA = KYAWR0_CR;       /* ����� */

	/* �������� 2+ ���. */
	for( timf = 3; timf > 0 ; timf-- )
		sleep(&lbolt, TTIPRI);

	addr->ky_csrA = 04;      /* WR4 */
	addr->ky_csrA = KYAWR4_CM16|KYAWR4_1SB;

	addr->ky_csrA = 03;      /* WR3 */
	addr->ky_csrA = KYAWR3_RXEN|KYAWR3_R7BIT;

	addr->ky_csrA = 05;      /* WR5 */
	addr->ky_csrA = KYAWR5_TXEN|KYAWR5_T7BIT;

	addr->ky_brate = kyspeed[B9600] | (kyspeed[B9600]<<4);

	addr->ky_csrB = 01;      /* WR1 B */
	addr->ky_csrB = KYBWR1_ON;

	addr->ky_csrB = 02;      /* WR2 B */
	addr->ky_csrB = 0;

	addr->ky_mcr0 = KYMCR0_105 | KYMCR0_108;        /* �-�� ������� */

	RB(I85_CR1) = CLR_INT;
}


/*ARGSUSED*/
kyopen(dev, flag)
dev_t   dev;
{
	register struct kydevice *addr;
	register struct tty *tp;

	if( !kyinited ) { kyinit(); kyinited++; }
	tp = &ky11;
	addr = tp->t_addr = KYADDR;
	tp->t_oproc = kystart;
	if ((tp->t_state & ISOPEN) == 0) {
		tp->t_state = ISOPEN | CARR_ON;
		tp->t_flags = ANYP | LCASE | CRMOD;
		tp->t_line = DFLT_LDISC;
		ttychars(tp);
	} else if (tp->t_state & XCLUDE && u.u_uid != 0) {
		u.u_error = EBUSY;
		return;
	}
	addr->ky_csrA = 01;      /* WR1 A */
	addr->ky_csrA = KYAWR1_TIE|KYAWR1_RIE1;
	RB(I85_CR1) = SET_IE; /* ��������� ���������� */
	ttyopen(dev, tp);
}

/*ARGSUSED*/
kyclose(dev, flag)
dev_t   dev;
int     flag;
{
	ttyclose(&ky11);
}

kyread(dev)
dev_t   dev;
{
	(*linesw[ky11.t_line].l_read)(&ky11);
}

kywrite(dev)
dev_t   dev;
{
	(*linesw[ky11.t_line].l_write)(&ky11);
}

static kyblock, kybf;

kyintr(dev)
dev_t   dev;
{
	register int              c;
	register struct kydevice *addr;
	register struct tty      *tp;
	static struct clist que;
	static nsd;
	int s;

	RB(I85_CR1) = RES_IE; /* ��������� ���������� */
	tp = &ky11;
	addr = (struct kydevice *)tp->t_addr;
	addr->ky_csrB = 02;      /* RR2 */

	switch (addr->ky_csrB & KYBRR2_V) {     /* ������� ������������� ���������� */
	case KY_TBMT:                           /* ����� ����������� ���� */
		tp->t_state &= ~BUSY;
		if( tp->t_state & (TTSTOP|TIMEOUT) )
			addr->ky_csrA = KYAWR0_RTIP;
		else
			kystart(tp);
		if (tp->t_state & ASLEEP && tp->t_outq.c_cc <= TTLOWAT(tp)) {
				wakeup((caddr_t) &tp->t_outq);
		}
		/* �������� ������ � ����� */

	case KY_RXCA:                            /* ������ ������ */
		while (addr->ky_csrA & KYARR0_RXCA) {
			addr->ky_csrA = 01;      /* RR1 */
			if (addr->ky_csrA & (KYARR1_RXPE|KYARR1_RXOE|KYARR1_FE))
				addr->ky_csrA = KYAWR0_ER;

			c = addr->ky_dbuf;
			c &= 0377;
			if ((tp->t_flags&RAW) == 0) {
				c &= 0177;
				if( c == tun.t_stopc ) {
					tp->t_state |= TTSTOP;
					continue;
				}
			} else if( tp->t_line == MPYLDISC &&
				   c == tun.t_stopc ) {
				tp->t_state |= TTSTOP;
				continue;
			}
			if( nsd ) {
				if( (tp->t_flags & TANDEM) &&
				    que.c_cc > TTYHOG/2 && !kybf ) {
					kyblock++;
					ttstart(tp);
				}
				putc(c, &que);
				continue;
			}
			nsd++;
			(void) _spl5();
			RB(I85_CR1)   = SET_IE;
			addr->ky_csrA = KYAWR0_EI;
			for(;;) {
				(void) _spl1();
				(*linesw[tp->t_line].l_input)(c, tp);
				if( kybf && que.c_cc < TTYHOG/6 ) {
					if( tp->t_state & TBLOCK )
						kybf = 0;
					else
					if( putc(tun.t_startc, &tp->t_outq) == 0) {
						kybf = 0;
						ttstart(tp);
					}
				}
				(void) _spl5();
				if( que.c_cc > 0 ) {
					c = getc( &que );
					continue;
				}
				nsd = 0;
				return;
			}
		}
out:
		addr->ky_csrA = KYAWR0_EI;
		break;

	case KY_SRC:                            /* ������ �� ������ */
		addr->ky_csrA = KYAWR0_ER;      /* ����� ������ */
		goto out;

	case KY_ESC:                             /* ������� ������� */
		if (addr->ky_csrA&KYARR0_B) {    /* ������ ����� */
			if (tp->t_flags&(RAW|CBREAK))
				(*linesw[tp->t_line].l_input)(0,tp);
		}
		addr->ky_csrA = KYAWR0_RESI;
		goto out;
	}
	(void) _spl5();
	RB(I85_CR1) = SET_IE; /* ��������� ���������� */
}

kyioctl(dev, cmd, addr, flag)
caddr_t addr;
dev_t   dev;
{
	switch (ttioctl(&ky11, cmd, addr, flag)) {
		case TIOCSETN:
		case TIOCSETP:
		case TIOCSETA:
		case TIOCSETB:
			kyparam();
			break;
		case 0:
			break;
		default:
			u.u_error = ENOTTY;
	}
}

kystart(tp)
register struct tty *tp;
{
	register c;
	register struct kydevice *addr;

	addr = (struct kydevice *) tp->t_addr;
	/* addr->ky_csrA = 01;    /* RR1 */
	if ((addr->ky_csrA & KYARR0_TBMT) == 0 )
		return;
	if ( kyblock ) {
		kyblock = 0;
		kybf++;
		tp->t_state |= BUSY;
		addr->ky_dbuf = tun.t_stopc;
		return;
	}
	if ((c=getc(&tp->t_outq)) >= 0) {
		if ((tp->t_lstate & L8BITS) || c<0200 || c>=0300) {
			tp->t_state |= BUSY;
			addr->ky_dbuf = c;
			return;
		} else {
			timeout(ttrstrt, (caddr_t)tp, (c & 0177) + KYDELAY);
			tp->t_state |= TIMEOUT;
		}
	}
	addr->ky_csrA = KYAWR0_RTIP;
}

kyparam()
{
	register struct  kydevice *addr = KYADDR;
	register struct  tty      *tp   = &ky11;
	register                   wr4;
	int                        wr3, wr5;
	int                        s;

	RB(I85_CR1) = RES_IE;          /* ��������� ���������� */
	switch (tp->t_flags & (EVENP|ODDP)) {
		case EVENP:
			wr4 = KYAWR4_PEN|KYAWR4_EP|KYAWR4_CM16;
			break;
		case ODDP:
			wr4 = KYAWR4_PEN|KYAWR4_CM16;
			break;
		default:
			wr4 = KYAWR4_CM16;
	}

	if (tp->t_ospeed == B110)
		wr4 |= KYAWR4_2SB;
	else
		wr4 |= KYAWR4_1SB;
	wr5 = KYAWR5_T8BIT|KYAWR5_TXEN;
	wr3 = KYAWR3_R8BIT|KYAWR3_RXEN;
	addr->ky_csrA = 04;      /* WR4 */
	addr->ky_csrA = wr4;
	addr->ky_csrA = 03;      /* WR3 */
	addr->ky_csrA = wr3;
	addr->ky_csrA = 05;      /* WR5 */
	addr->ky_csrA = wr5;
	addr->ky_brate = kyspeed[tp->t_ispeed] | (kyspeed[tp->t_ospeed]<<4);
	RB(I85_CR1) = SET_IE; /* ��������� ���������� */
}
#endif

#endif NKY

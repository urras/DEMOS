/*
 * standalone ������� ������� ����� MY (��� Qbus22, ����. ���4)
 * $Header: my.c,v 1.1 90/12/12 17:12:37 korotaev Exp $
 * $Log:	my.c,v $
 * Revision 1.1  90/12/12  17:12:37  korotaev
 * Initial revision
 * 
 * Revision 1.2  89/01/12  17:57:17  chech
 * ������ ���� � ����������� MY;
 * ������ ������� �������� �����������, �� �������� � ������� ������.
 * 
 * Revision 1.1  88/12/30  14:54:00  chech
 * Initial revision
 * 
 * �����: ����� �.�. & Co (���, ���.���.)
 */
/* ������� ������������:
 * 1. ���� segflag != 0, �� ������� ��������� ������
 * 2. ������� �������� � �������� "segflag", � ������� ��������������
 *    ��������.
 * ��� ������������� ����� ��� boot (������������ ��-��, ���������� segflag)
 */
#include <sys/param.h>
#include <sys/inode.h>
#include "../saio.h"

/*#define SINGLE /* ��������� ��� ������������� ������� */

/*      ������ ���� � ��������� ����������� MY
 * �����������, ��������� � ��� �� ���������� ���������� ������
 * ������ ����� ���������� (���� 13-7 ���). ������� ���� ����������
 * ������ ���������� � ������ �������� ������ �����������.
 * ������ ���� ������� ��� ������������ MY_BUGS
 */
#define MY_BUGS 1

#define MYADDR  ((struct device *)0172140)
#define NMY     4
#define NMYSEC  10
#define NMYTRK  80
#ifdef SINGLE
#define NMYSID  1
#else
#define NMYSID  2
#endif SINGLE
#define NMYBLK  (NMYSEC*NMYTRK*NMYSID)

#define	GO	01
#define DONE    040
#define TR      0200
#define RESET   040000

#define RCOM    0
#define	WCOM	2
struct	device
{
	int     mycs;
	int     mydat;
};

mystrategy(io, func)
register struct iob *io;
{
	struct  {
		char        p_dev;
		char        p_xmem;
		unsigned    p_addr;
		char        p_sec;
		char        p_trk;
		unsigned    p_wcnt;
	} myparm;
#ifdef MY_BUGS
	char    tmpbuf [sizeof (myparm)];
#endif
	daddr_t bn;
	register nerr=10;
retop:
	if ((bn = io->i_bn) < 0) {
		printf ("my: negative block, ");
		goto Fatal;
	}
	if (bn >= NMYBLK) {
		printf ("my: bad block %d, ", (short)bn);
		goto Fatal;
	}
	myparm.p_sec = 1 + bn%NMYSEC;
	bn = bn / NMYSEC;
	myparm.p_trk = bn >> (NMYSID-1);
	myparm.p_dev = (io->i_unit & 03) | ((NMYSID-1)?((bn & 01)<<2):0);
	myparm.p_xmem = segflag;
	myparm.p_addr = io->i_ma;
	myparm.p_wcnt = io->i_cc >> 1;
#ifdef MY_BUGS
	if (segflag) {
/* ������ ������ �������� �� �������� "0" � �����; �������� ��������� */
		cop_from (0, &myparm, sizeof (myparm) >> 1, tmpbuf);
		cop_to   (0, &myparm, sizeof (myparm) >> 1, &myparm);
	}
	MYADDR->mycs = ((func == READ) ? RCOM:WCOM) | GO;
#else
	MYADDR->mycs = ((func == READ) ? RCOM:WCOM) | GO | (segflag << 8);
#endif
	while ((MYADDR->mycs & TR) == 0) ;
	MYADDR->mydat = & myparm;
	while ((MYADDR->mycs & DONE) == 0) ;

	if (MYADDR->mycs<0) {	/* error bit */
		printf("MY%d disk error: trk=%d, sec=%d, side=%c, er=%o\n",
		    myparm.p_dev & 03, myparm.p_trk, myparm.p_sec,
		    (myparm.p_dev & 04) ? 'U' : 'D', MYADDR->mydat);
		MYADDR->mycs = RESET | GO;
		while ((MYADDR->mycs & DONE) == 0) ;
		if(--nerr) goto retop;
Fatal:
		printf("fatal!\n");
		nerr = -1;
	}
	else nerr = io->i_cc;
#ifdef MY_BUGS
	if (segflag) {
/* �������� �� ��, ��� ����������� ������ �� ����������� 0-��������
   �� �������� �.�. boot ����� ������ � ����������� ����� (������� 3)
 */
		cop_to (0, &myparm, sizeof (myparm) >> 1, tmpbuf);
	}
#endif
	return nerr;
}

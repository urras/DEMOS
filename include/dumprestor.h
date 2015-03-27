#ifndef _dumprestor_h_
#       define  _dumprestor_h_

#include <time.h>
/* $Header: dumprestor.h,v 1.1 86/10/21 05:52:50 alex Exp $ */

#ifdef  NONSEPARATE
#define MAXINO  1000    /* ����. ����� I-����� �� ����� */
#else
#define MAXINO  3000
#endif

#define BITS    8       /* ����� � ����� */
#define MAXXTR  60

#ifndef STANDALONE
#define NCACHE  3       /* ����� ���������� ���-������� */
#else
#define NCACHE  1       /* Size reduction as refered to above */
#endif

/* ������ � �������� ������� */
#define MWORD(m,i)   (m[(unsigned)(i-1)/MLEN])
#define MBIT(i)    (1<<((unsigned)(i-1)%MLEN))
#define BIS(i,w)        (MWORD(w,i) |=  MBIT(i))
#define BIC(i,w)        (MWORD(w,i) &= ~MBIT(i))
#define BIT(i,w)        (MWORD(w,i) & MBIT(i))

#if     UCB_NKB == 1
#define NTREC           10      /* ����� ���������� ������� (��) */
#endif
#ifndef UCB_NKB
#define NTREC           20
#endif
#define MLEN            16      /* ����� � int-����� */
#define MSIZ            3072    /* mapsize: 4096 ��� SEPID */
			/* ������� �������� int-���� ��� ������� ����� */

	/* ���� header-�� */
#define TS_TAPE         1
#define TS_INODE        2
#define TS_BITS         3
#define TS_ADDR         4
#define TS_END          5
#define TS_CLRI         6
#define MAGIC           (int)60011
#define CHECKSUM        (int)84446
	/* ��������� header-� */
struct  spcl
{
	int     c_type;
	time_t  c_date;
	time_t  c_ddate;
	int     c_volume;
	daddr_t c_tapea;
	ino_t   c_inumber;
	int     c_magic;
	int     c_checksum;
	struct  dinode  c_dinode;
	int     c_count;
	char    c_addr[BSIZE];
} spcl;

/* ������ ������� /etc/ddate
 */
struct  idates
{
	char    id_name[16];
	char    id_incno;
	time_t  id_ddate;
};



/*
 * �������� ��� ������ ����� /etc/dumpdate
 */
#define DUMPDN  "/etc/dumpdate"
#define DUMPDO  "/etc/dumpdate.old"
#define DUMPDNN "/etc/dumpdate.new"
#define DDMAXL  128 /* ����. ����� 1 ������ */

struct dumpdate {
	char dd_name[32];
	short dd_incr;
	struct tm dd_tm;
/* ����� �������� ������ ���������� */
};
#endif  _dumprestor_h_


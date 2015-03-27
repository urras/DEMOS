/*
 * equel.h
 * (������ ����������� �� /usr/include/rubin/equel.h)
 * �������� �������� ����� � ���������� � EQUEL
 * (������, � ���������� libq.a)
 */

/* 1. ���������� ������� ������ */
extern int  IIerrflag;           /* ������� - ���� ������      */
extern IIret_err(), IIno_err();  /* �������� ��������� ������  */
extern int  (*IIprint_err)();    /* ��������� �� �/� ��������� */
extern int  (*IIout_err)();      /* ��������� ������ ������ ����������� */
				 /* �� ��������� (���� NULL)    - puts */
extern  int     (*IIabort)();    /* ��������� ���������� ����������.   */
extern  int     (*IIinterrupt)(); /* ��������� ��������� ���������� */
/* 2. �������������� ���������� */
extern  int     IItupcnt;       /* ����� ���������/���������� �������� */
extern  char    *IImainpr;      /* ��� ���������� �������       */

/* 3. ��������� ����� */
#define         EQ_ERR          (IIerrflag != 0)
#define         EQ_ONERR        IIprint_err = IIret_err
#define         EQ_OFFERR       IIprint_err = IIno_err

/* 4. ���� ������ */
typedef short  EQ_i2;
typedef long   EQ_i4, EQ_date, EQ_time;
typedef char   EQ_c;
typedef float  EQ_f4;
typedef double EQ_f8;

# include <defines.h>
# include <datetime.h>
# include <aux.h>
# include "../iutil/form_defs.h"

/*
 *  ��������� �����/������ ���� � �������
 *  �������� �������
 *  ������������� ������
 */
union cdunion {
struct cd_date da;
struct cd_time ti;
} cdbuf;
/* ���������. ������������� ��� ������ ��������� */


char dfl_date[MAX_LDATE+1] = "DD-MON-YYYY";
char dfl_time[MAX_LDATE+1] = "HH:MI:SS";

char *d_date = dfl_date, *d_time = dfl_time; /* ������� ��� MAIN */
/*
 * �������� �������
 */
# define D(n) &(cdbuf.da.n)
# define T(n) &(cdbuf.ti.n)

struct tabform tb_date[] = {
"YYYY", D(cd_year),     "%04d", 0,      4,
"YY",   D(cd_2year),    "%02d", 0,      2,
"����", D(cd_year),     "%04d", 0,      4,
"��",   D(cd_2year),    "%02d", 0,      2,
"MM",   D(cd_month),    "%02d", 0,      2,
"��",   D(cd_month),    "%02d", 0,      2,
"DD",   D(cd_day),      "%02d", 0,      2,
"��",   D(cd_day),      "%02d", 0,      2,
"W",    D(cd_week),     "%01d", 0,      1,
"�",    D(cd_week),     "%01d", 0,      1,
"DAYWEEK",D(cd_week),   "\1Sunday\2Monday\3Tuesday\4Wensday\5Thusday\6Fridey\7Satuday",1,7,
"DAY",  D(cd_week),     "\1Sun\2Mon\3Tue\4Wen\5Thu\6Fri\7Sat", 1, 3,
"�������",D(cd_week),   "\1�����������\2�����������\3�������\4�����\5�������\6�������\7�������",1,11,
"���",  D(cd_week),     "\1���\2���\3���\4���\5���\6���\7���", 1,3,
"MONTH",D(cd_month),    "\1January\2February\3March\4April\5May\6June\7July\10August\11September\12October\13November\14December", 0, 9,
"MON", D(cd_month),     "\1Jan\2Feb\3Mar\4Apr\5May\6Jun\7Jul\10Aug\11Sep\12Oct\13Nov\14Dec", 0, 3,
"������",D(cd_month),   "\1������\2�������\3�����\4������\5���\6����\7����\10�������\11��������\12�������\13������\14�������", 0, 8,
"�����" ,D(cd_month),   "\1������\2�������\3����\4������\5���\6����\7����\10������\11��������\12�������\13������\14�������", 0, 8,
"���",  D(cd_month),    "\1���\2���\3���\4���\5���\6���\7���\10���\11���\12���\13���\14���", 0, 3,
NULL };

struct tabform tb_time[] = {
"HH", T(cd_hh), "%02d", 0, 2,
"MI", T(cd_mm), "%02d", 0, 2,
"SS", T(cd_ss), "%02d", 0, 2,
NULL};

extern long cd_date(), cd_time();
extern int date_cd();
extern int time_cd();
/* �������� ����������� ������� */
struct datebase b_date=
{
	cd_date,
	date_cd,
	(char *)&cdbuf,
	tb_date,
	dfl_date
};
struct datebase b_time=
{
	cd_time,
	time_cd,
	(char *)&cdbuf ,
	tb_time,
	dfl_time
};

/* ��������� �����������. ������ ���������������� ������
 * ���� ��������, �������� ������ ���������� �� ����������������
*/
char *do_compl(s,t)
char *s;
struct datebase *t;
{
	register char *out;
	register int i;
	i = 0;
	if (!s )
	{
		s = t->dflt;
		i = MAX_LDATE;
	}
	if (s[0] == D_COMPL)
		return(s);
	if (!i)
		i = length(s);
	out = to_compl(s,t->tb);
	if(out == NULL)
		return(NULL);
# ifndef EQUEL
	if (length(out) <= i)
		smove(out,s);
# endif
	return(out);
}

/*
 * date_len(s)
 * - ����� ������ ���� �� ������� s
 */
date_len(s)
char *s;
{
register char *o;
if (!(o = do_compl(s,&b_date))) return(-1); /*???*/
return(o[1]-' ');
}

/*
 * time_len(s)
 * - ����� ������ ������� �� ������� s
 */
time_len(s)
char *s;
{
register char *o;
if (!(o = do_compl(s,&b_time))) return(-1); /*???*/
return(o[1]-' ');
}

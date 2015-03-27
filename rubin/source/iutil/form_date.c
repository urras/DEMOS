# include <defines.h>
# include <datetime.h>
# include <aux.h>
# include "../iutil/form_defs.h"

/*
 *  Программы ввода/вывода даты и времени
 *  Основные шаблоны
 *  Инициализация данных
 */
union cdunion {
struct cd_date da;
struct cd_time ti;
} cdbuf;
/* Умолчания. Компилируются при первом обращении */


char dfl_date[MAX_LDATE+1] = "DD-MON-YYYY";
char dfl_time[MAX_LDATE+1] = "HH:MI:SS";

char *d_date = dfl_date, *d_time = dfl_time; /* Признак для MAIN */
/*
 * Основные таблицы
 */
# define D(n) &(cdbuf.da.n)
# define T(n) &(cdbuf.ti.n)

struct tabform tb_date[] = {
"YYYY", D(cd_year),     "%04d", 0,      4,
"YY",   D(cd_2year),    "%02d", 0,      2,
"ГГГГ", D(cd_year),     "%04d", 0,      4,
"ГГ",   D(cd_2year),    "%02d", 0,      2,
"MM",   D(cd_month),    "%02d", 0,      2,
"ММ",   D(cd_month),    "%02d", 0,      2,
"DD",   D(cd_day),      "%02d", 0,      2,
"ДД",   D(cd_day),      "%02d", 0,      2,
"W",    D(cd_week),     "%01d", 0,      1,
"Н",    D(cd_week),     "%01d", 0,      1,
"DAYWEEK",D(cd_week),   "\1Sunday\2Monday\3Tuesday\4Wensday\5Thusday\6Fridey\7Satuday",1,7,
"DAY",  D(cd_week),     "\1Sun\2Mon\3Tue\4Wen\5Thu\6Fri\7Sat", 1, 3,
"ДЕНЬНЕД",D(cd_week),   "\1Воскресенье\2Понедельник\3Вторник\4Среда\5Четверг\6Пятница\7Суббота",1,11,
"ДЕН",  D(cd_week),     "\1Вск\2Пон\3Вто\4Сре\5Чет\6Пят\7Суб", 1,3,
"MONTH",D(cd_month),    "\1January\2February\3March\4April\5May\6June\7July\10August\11September\12October\13November\14December", 0, 9,
"MON", D(cd_month),     "\1Jan\2Feb\3Mar\4Apr\5May\6Jun\7Jul\10Aug\11Sep\12Oct\13Nov\14Dec", 0, 3,
"МЕСЯЦА",D(cd_month),   "\1Января\2Февраля\3Марта\4Апреля\5Мая\6Июня\7Июля\10Августа\11Сентября\12Октября\13Ноября\14Декабря", 0, 8,
"МЕСЯЦ" ,D(cd_month),   "\1Январь\2Февраль\3Март\4Апрель\5Май\6Июнь\7Июль\10Август\11Сентябрь\12Октябрь\13Ноябрь\14Декабрь", 0, 8,
"МЕС",  D(cd_month),    "\1Янв\2Фев\3Мар\4Апр\5Май\6Июн\7Июл\10Авг\11Сен\12Окт\13Ноя\14Дек", 0, 3,
NULL };

struct tabform tb_time[] = {
"HH", T(cd_hh), "%02d", 0, 2,
"MI", T(cd_mm), "%02d", 0, 2,
"SS", T(cd_ss), "%02d", 0, 2,
NULL};

extern long cd_date(), cd_time();
extern int date_cd();
extern int time_cd();
/* Основные управляющие таблицы */
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

/* Программа компиляциии. Выдает скомпилированную строку
 * Если возможно, исходная строка заменяется на скомпилированную
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
 * - Длина вывода даты по шаблону s
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
 * - Длина вывода времени по шаблону s
 */
time_len(s)
char *s;
{
register char *o;
if (!(o = do_compl(s,&b_time))) return(-1); /*???*/
return(o[1]-' ');
}

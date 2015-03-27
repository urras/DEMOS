/*
 * Программа для получения системных даты и времени
 *
 * long get_date()
 * long get_time()
 * new_sys()
 *
 * В datetime,h нужно включить строки
 *
 * extern newdati();
 * extern long sys_date,sys_time;
 * define get_date() (sys_date<0? (newdati(), sys_date):sys_date)
 * define get_time() (sys_time<0? (newdati(), sys_time):sys_time)
 * define new_datime()  {sys_date = sys_time = -1;}
 */

# include <defines.h>
# include "../iutil/form_defs.h"
# include <datetime.h>
# include <time.h>
long sys_date = -1;
long sys_time = -1;
/*
 * Функция "newdati()" вычисляет текущие значения
 * переменных sys_date, sys_time
 * Это одна функция, так как эти переменные должны иметь
 * согласованные значения
 */
newdati()
{
	struct cd_date cdd;
	struct cd_time cdt;
	long tt;
	extern long time();
	extern long cd_date(), cd_time();
	register struct tm *pt;
	extern struct tm *localtime();
	tt = time(0);
	pt = localtime (&tt);
	cdd.cd_year = pt->tm_year+1900;
	cdd.cd_month= pt->tm_mon+1;
	cdd.cd_day  = pt->tm_mday;
	cdt.cd_hh = pt->tm_hour;
	cdt.cd_mm = pt->tm_min;
	cdt.cd_ss = pt->tm_sec;
	sys_date = cd_date( &cdd);
	sys_time = cd_time( &cdt);
	return;
}

# include <defines.h>
# include <datetime.h>
# include "../iutil/form_defs.h"

/* Дней в четырех годах и в 2 веках */
# define d_y4 ((long)365*3+366)
# define d_y200 ((long)365*200+49)
# define d_y1    365
# define WEEK0 1 /* Начальный день недели */

/* Распределение по месяцам */
char dm0[12]= { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char dm1[12]= { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
 * date_cd
 * - преобразование даты в структуру
 */
date_cd(dd,cd)
t_date dd;
register struct cd_date *cd;
{
	long rest;
	register char *dp;
	register int i,j;
	long int y200, y4, y1, mn, day, dw;
	/* 1. Прибавим 199 лет */
	rest = dd + (d_y200 - 1*365l);
	y200 = rest/d_y200;
	rest -= y200*d_y200;
	y4   = rest / d_y4;
	rest -= y4  *d_y4;
	y1 = rest / d_y1;
	if(y1>3) y1 = 3;
	day = rest = rest-y1*d_y1;
	cd->cd_year = y200*200 + y4*4 + y1 - 199;
	/* Вычисляем день месяца */
	if (y1 == 3 && !(y4 == 49))
		dp = dm1;
	else
		dp = dm0;
	for(i=0; i<11; i++)
	{
		j = dp[i];
		if (day < j) break;
		day -= j;
	}
	cd->cd_day = day+1;
	cd->cd_month = i+1;
	cd->cd_2year = cd->cd_year % 100;
	cd->cd_week = (dd + WEEK0) % 7;
	return(0);
}

/*
 *  cd_date(cd) - обратное преобразование
 */
t_date cd_date(cd)
register struct cd_date *cd;
{
	t_date day;
	int yy;
	register int i;
	register char *dp;
	if (cd->cd_year == 0 && cd->cd_2year != 0)
		cd->cd_year = 1900+cd->cd_2year;
	yy = cd->cd_year + 199;
	day = (long)yy*d_y1 + yy/4 - yy/200 - (d_y200-1*365);
	yy = cd->cd_year;
	if (yy%4 == 0 && yy%200 != 0)
		dp = dm1;
	else
		dp = dm0;
	for(i=0; i<cd->cd_month-1; i++)
		day += dp[i];
	day += cd->cd_day -1;
	return(day);
}

/*
 * time_cd(dd, cd)
 * преобразование времени из секунд в часы, минуты, секунды
 */
time_cd(dd,cd)
t_time dd;
register struct cd_time *cd;
{
	cd->cd_ss = dd%60;
	dd /= 60;
	cd->cd_mm = dd%60;
	cd->cd_hh = dd/60;
	return(0);
}

/*
 * cd_time(cd)
 * преобразование времени из часов-минут-секунд в секунды
 */
t_time cd_time(cd)
register struct cd_time *cd;
{
	return( (long)(cd->cd_hh)*3600 + cd->cd_mm*60 + cd->cd_ss);
}

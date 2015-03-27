/* $Header: /usr/users/korotay/DEMOS_2.2/rcs/src/rcs/RCS/time.h,v 1.3 1986/02/23 22:23:47 avg Exp $ */

/* Описание структуры для работы процедур выдачи/задания времени.
 * Используются следующие библиотечные программы:
 *	libc: ctime, localtime, gmtime, asctime
 *      libcx: partime, maketime (могут пока отсутствовать)
 */

/* $Log: time.h,v $
 * Revision 1.3  1986/02/23  22:23:47  avg
 * Сделаны изменения для МНОС и для работы с U-кодом.
 *
 * Revision 1.1  85/12/26  22:29:43  fox
 * Initial revision
 * 
 * Revision 1.1  85/12/26  22:29:43  fox
 * Система хранения версий файлов
 * 
 */

struct tm {     /* See defines below for allowable ranges */
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
	int tm_zon;	/* NEW: mins westward of Greenwich */
	int tm_ampm;	/* NEW: 1 if AM, 2 if PM */
};

#define LCLZONE (5*60)	/* Until V7 ftime(2) works, this defines local zone*/
#define TMNULL (-1)	/* Items not specified are given this value
			 * in order to distinguish null specs from zero
			 * specs.  This is only used by partime and
			 * maketime. */

	/* Indices into TM structure */
#define TM_SEC 0	/* 0-59			*/
#define TM_MIN 1	/* 0-59			*/
#define TM_HOUR 2	/* 0-23			*/
#define TM_MDAY 3	/* 1-31			day of month */
#define TM_DAY TM_MDAY	/*  "			synonym      */
#define TM_MON 4	/* 0-11			*/
#define TM_YEAR 5	/* (year-1900) (year)	*/
#define TM_WDAY 6	/* 0-6			day of week (0 = Sunday) */
#define TM_YDAY 7	/* 0-365		day of year */
#define TM_ISDST 8	/* 0 Std, 1 DST		*/
	/* New stuff */
#define TM_ZON 9	/* 0-(24*60) minutes west of Greenwich */
#define TM_AMPM 10	/* 1 AM, 2 PM		*/

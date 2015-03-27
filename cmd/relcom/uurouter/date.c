/*
 * THIS PROCEDURE IS SYSTEM-DEPENDENT
 */
#include <time.h>
#if  defined(VMUNIX) || defined(demos)
#include <sys/types.h>
#include <sys/timeb.h>
#endif

static char *wdays[] = {
	"Sun",  "Mon",  "Tue",  "Wed",  "Thu",  "Fri",  "Sat"
};

static char *months[] = {
	"Jan",  "Feb",  "Mar",  "Apr",  "May",  "Jun",
	"Jul",  "Aug",  "Sep",  "Oct",  "Nov",  "Dec"
};

/*
 * ARPA-styled date
 * RFC822 format  'Sun, 20 Nov 91 17:35:54 PST\0' or
 *                'Fri, 26 May 85 02:13:10 -0700 (PST)\0'
 */
char *adate()
{
	static char out[50];
	long now;
	struct tm *t;
	int tzshift;
	char tzsign;
#if  defined(VMUNIX) || defined(demos)
	struct timeb tb;

	ftime(&tb);
	t = localtime(&tb.time);
	tzshift = tb.timezone;
#else   /* not VMUNIX */
	time(&now);
	t = localtime(&now);
#ifdef  M_XENIX
	tzshift = t->tm_tzadj/60;
#else
	tzshift = timezone/60;
#endif
#endif  /* not VMUNIX */
	tzsign = tzshift <= 0 ? '+' : '-';
	if( tzshift < 0 ) tzshift = -tzshift;
	tzshift = 100*(tzshift/60) + (tzshift%60);
	sprintf(out, "%3.3s, %2d %3.3s %2d %02d:%02d:%02d %c%04d (%s)",
		     wdays[t->tm_wday],
		     t->tm_mday,
		     months[t->tm_mon],
		     t->tm_year % 100,
		     t->tm_hour, t->tm_min, t->tm_sec,
		     tzsign,
#if  defined(VMUNIX) || defined(demos)
		     tzshift + (tb.dstflag != 0)*100,
		     timezone(tb.timezone, tb.dstflag)
#else
		     tzshift,
		     tzname[t->tm_isdst]       /* Name of local timezone */
#endif
		);
	return out;
}

#ifdef TEST
main()
{
	printf("%s\n", adate());
}
#endif

/*
 * Unix Date
 */
char *udate()
{
	static char out[40];
	long now;
	struct tm *t;
#ifdef  VMUNIX
	struct timeb tb;
	ftime(&tb);
	t = localtime(&tb.time);
#else
	time(&now);
	t = localtime(&now);
#endif
	sprintf(out, "%3.3s %3.3s %2d %02d:%02d:%02d %3.3s %4d",
		     wdays[t->tm_wday],
		     months[t->tm_mon],
		     t->tm_mday,
		     t->tm_hour, t->tm_min, t->tm_sec,
#ifdef  VMUNIX
		     timezone(tb.timezone, tb.dstflag),
#else
		     tzname[daylight],        /* Name of local timezone */
#endif
		     1900 + t->tm_year );
	return out;
}


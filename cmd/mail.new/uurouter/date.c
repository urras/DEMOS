
#include <sys/types.h>
#include <time.h>
#if defined(VMUNIX) || defined(DEMOS)
#include <sys/timeb.h>
#endif

char *months[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
char *wdays[7]  =  {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

/*
 * Unix Date
 */
char *udate()
{
	static char out[40];
	long now;
	struct tm *t;
#if defined(VMUNIX) || defined(DEMOS)
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
#if defined(VMUNIX) || defined(DEMOS)
		     timezone(tb.timezone, tb.dstflag),
#else
		     tzname[daylight],        /* Name of local timezone */
#endif
		     1900 + t->tm_year );
	return out;
}

/*
 * Arpa Date
 * RFC822 format  'Mon, 21 Nov 83 11:31:54 PST\0' or
 *                'Mon, 16 May 88 02:13:10 -0700\0'
 */
char *adate()
{
	static char out[40];
	long now;
	struct tm *t;
	int tzshift;
	char tzsign;
#if defined(VMUNIX) || defined(DEMOS)
	struct timeb tb;
#endif

#if defined(VMUNIX) || defined(DEMOS)
	ftime(&tb);
	t = localtime(&tb.time);
	tzshift = (int)(tb.timezone/60);
#else
	time(&now);
	t = localtime(&now);
	tzshift = (int)(timezone/60);
#endif
	tzsign = tzshift <= 0 ? '+' : '-';
	if( tzshift < 0 ) tzshift = -tzshift;
	tzshift = 100*(tzshift/60) + (tzshift%60);
	sprintf(out, "%3.3s, %2d %3.3s %2d %02d:%02d:%02d %c%04d (%s)",
		     wdays[t->tm_wday],
		     t->tm_mday,
		     months[t->tm_mon],
		     t->tm_year % 100,
		     t->tm_hour, t->tm_min, t->tm_sec,
		     tzsign, tzshift,
#if defined(VMUNIX) || defined(DEMOS)
		     timezone(tb.timezone, tb.dstflag));
#else
		     tzname[daylight] );      /* Name of local timezone */
#endif
	return out;
}

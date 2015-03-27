# include "tm.h"

# define STOREINT(p,nn) {\
	register char *cp = p;\
	register n = nn;\
	*cp++ = n/10 + '0';\
	*cp = n%10 + '0';\
	}

static char pattern [] = "00-Mon-1900 00:00:00";

extern char *strncpy ();

char *timestr (tim)
long tim;
{
	register char *p;
	struct TM *t;

	t = TMlocaltime (&tim);
	p = pattern;
	STOREINT (p, t->tm_mday);
	if (*p == '0')
		*p = ' ';


	strncpy (p+3, "JanFebMarAprMayJunJulAugSepOctNovDec"+t->tm_mon*3, 3);
	STOREINT (p+9, t->tm_year);
	if (t->tm_year < 100) {
		p[7] = '1';
		p[8] = '9';
	} else {
		p[7] = '2';
		p[8] = '0';
	}
	STOREINT (p+12, t->tm_hour);
	STOREINT (p+15, t->tm_min);
	STOREINT (p+18, t->tm_sec);
	return (p);
}

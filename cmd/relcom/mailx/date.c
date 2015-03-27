#include "rcv.h"
#include <time.h>
#if defined(VMUNIX) || defined(__TURBOC__)
#include <sys/timeb.h>
#endif

/*
 * Mail -- a mail program
 *
 * Date manipulation.
 *
 * $Log: date.c,v $
 * Revision 1.9  1991/07/22  16:36:47  ache
 * Port to Borland C
 *
 * Revision 1.8  1991/05/19  15:25:55  ache
 * pRAWKI PO TABLICE ZON I DLQ VAX
 *
 * Revision 1.7  1991/04/16  19:19:42  ache
 * w SWQZI S PEREHODOM NA ^AS BLIVE K ZAPADU, IZMENENA ms[kd]
 *
 * Revision 1.6  91/01/12  19:45:29  ache
 * tb.timezone W minutah
 *
 * Revision 1.5  91/01/01  21:46:29  ache
 * Little bug with month
 * 
 * Revision 1.4  90/12/26  23:46:43  ache
 * pRAWKI POD VAX
 * 
 * Revision 1.3  90/12/25  03:53:57  ache
 * pRAWLENA DREWNQQ O[IBKA S ZATIRANIEM PAMQTI
 * 
 * Revision 1.2  90/12/22  22:54:07  ache
 * sORTIROWKA + WYDA^A fio
 * 
 * Revision 1.1  90/12/20  21:17:27  ache
 * Initial revision
 * 
 */

/*NOXSTR
static char rcsid[] = "$Header: /usr/src/Relcom/relcom/mailx/RCS/date.c,v 1.9 1991/07/22 16:36:47 ache Exp $";
YESXSTR*/

static long GetTime();
extern int uglyfromflag;

static char *months[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
static char *rms[] = {
	"QNW", "FEW", "MAR", "APR", "MAJ", "I@N",
	"I@L", "AWG", "SEN", "OKT", "NOQ", "DEK"
};
static char *wdays[]  = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

/*
 * Turn the name of month into standard form
 */
void
patchmonth(m)
	char m[];
{
	static char mm[4];
	register int i;

	mm[0] = m[0]; mm[1] = m[1]; mm[2] = m[2];
	for( i = 0 ; i < 12 ; i++ )
		if(    icequal(mm, months[i])
		    || icequal(mm, rms[i])
		  )
			goto found;
	return;
found:
	strcpy(m, ediag(months[i], rms[i]));
	m[3] = ' ';
}

long
GetDate(mp)
struct message *mp;
{
	FILE *ibuf;
	char *cp;
	int i;
	struct headline hl;
	char headline[LINESIZE];
	char pbuf[BUFSIZ];
	long t;
	char *s, sign;
	extern char *ctime();
	char mdate[50];

	/*
	 * Replace date field from ugly old From line by
	 * field from Arpa-style Date: regular header field.
	 */
	if (!uglyfromflag && (cp = hfield("date", mp)) != NOSTR) {
		s = &cp[strlen(cp)];
		while (s > cp && isspace(s[-1]))
			s--;
		*s = '\0';
	}
	else {
		ibuf = setinput(mp);
		readline(ibuf, headline);
		parse(headline, &hl, pbuf);
		cp = hl.l_date;
	}
	/*
	 * Collect the date in format MMM DD HH:MM:SS YYYY +HHMM
	 *                            01234567890123456789012345
	 */
	/* skip spaces */
	while (isspace(*cp))
		cp++;
	if (   isdigit(*cp)     /* ARPA without week day */
	    || cp[3] == ','     /* or standard ARPA      */
	   ) {
		if (!isdigit(*cp)) {
			cp += 4;
			while (isspace(*cp))
				cp++;
		}
		mdate[4] = *cp++;
		if( isdigit(*cp) )
			mdate[5] = *cp++;
		else {
			mdate[5] = mdate[4];
			mdate[4] = ' ';
		}
		while (isspace(*cp))
			cp++;
		mdate[0] = *cp++;                         /* copy month */
		mdate[1] = *cp++;
		mdate[2] = *cp++;
		mdate[3] = ' ';
		while (isspace(*cp))
			cp++;
		/* copy year */
		if (isdigit(*cp)) {
			mdate[18] = mdate[19] = ' ';
			for (i = 16; isdigit(*cp); cp++, i++)
				mdate[i] = *cp;
		}
		else {
			time(&t);
			s = ctime(&t);
			mdate[16] = s[20];
			mdate[17] = s[21];
			mdate[18] = s[22];
			mdate[19] = s[23];
		}
		while (isspace(*cp))
			cp++;
		mdate[6]  = ' ';
		/* hours */
		if (*(cp + 1) == ':')
			mdate[7] = '0';
		else
			mdate[7]  = *cp++;
		mdate[8]  = *cp++;
		/* : */
		mdate[9]  = *cp++;
		/* minutes */
		mdate[10] = *cp++;
		mdate[11] = *cp++;
		/* : */
		mdate[12] = *cp++;
		/* seconds */
		mdate[13] = *cp++;
		mdate[14] = *cp++;
		mdate[15] = ' ';
		if (mdate[18] == ' ') { /* Last two digit */
			time(&t);
			s = ctime(&t);
			mdate[18] = mdate[16];
			mdate[19] = mdate[17];
			mdate[16] = s[20];
			mdate[17] = s[21];
		}
		while (isspace(*cp))
			cp++;
	} else {                /* Unix format */
		if (!isdigit(*cp))
			cp += 4;        /* Skip week day */
		for( i = 0 ; i < 15 ; i++ )
			mdate[i] = *cp++;
		while (isspace(*cp))
			cp++;
		mdate[15] = ' ';
		if (*cp && isalpha(*cp))
			s = cp;
		else
			s = NOSTR;
		while(*cp && !isdigit(*cp))
			cp++;
		if (*cp) {
			for (i = 16; i < 20; i++)
				mdate[i] = *cp++;
		}
		if (s != NOSTR)
			cp = s;
	}
	mdate[20] = ' ';
	if (*cp == '+' || *cp == '-') {
		mdate[21] = *cp++;
		mdate[22] = *cp++;
		mdate[23] = *cp++;
		mdate[24] = *cp++;
		mdate[25] = *cp++;
	}
	else {
		if (*cp && isalpha(*cp)) {
			i = findzone(cp);
			sign = i <= 0 ? '+' : '-';
			if (i < 0) i = -i;
		}
		else {  /* Current zone */
			struct tm *tm;
#if defined(VMUNIX) || defined(__TURBOC__)
			struct timeb tb;

			ftime(&tb);
			i = tb.timezone - (tb.dstflag != 0) * 60;
#else   /* not VMUNIX */

			time(&t);
			tm = localtime(&t);
#ifdef  M_XENIX
			i = tm->tm_tzadj/60;
#else
			i = timezone/60;
#endif
#endif  /* not VMUNIX */
			sign = i <= 0 ? '+' : '-';
			if (i < 0) i = -i;
			i = 100*(i/60) + (i%60);
		}
		sprintf(&mdate[21], "%c%04d", sign, i);
	}
	mdate[26] = '\0';
	if( mdate[4] == '0' )
		mdate[4] = ' ';      /* Remove leading zero in day of month */
	if( mdate[7] == ' ' )
		mdate[7] = '0';      /* Vice versa in field of hours */

	{ short se = _ediag;
	_ediag = 1;
	patchmonth(mdate);
	_ediag = se;
	}

	return GetTime(mdate);
}

/* The timezone table. */
static struct TABLE {
	char *z_name;
	int z_off;
}
TimezoneTable[] = {
    { "gmt",   0 },  /* Greenwich Mean */
    { "ut",    0 },  /* Universal (Coordinated) */
    { "utc",   0 },
	{ "wet",   0 },  /* Western European */
	{ "bst",   0+100 },  /* British Summer */
    { "wat",   100 },  /* West Africa */
    { "at",    200 },  /* Azores */
#if 0
    /* For completeness.  BST is also British Summer, and GST is
     * also Guam Standard. */
    { "bst",   300 },  /* Brazil Standard */
    { "gst",   300 },  /* Greenland Standard */
#endif
    { "nft",   330 }, /* Newfoundland */
    { "nst",   330 }, /* Newfoundland Standard */
    { "ndt",   330 }, /* Newfoundland Daylight */
	{ "ast",    400 },  /* Atlantic Standard */
    { "adt",    400+100 },  /* Atlantic Daylight */
    { "est",    500 },  /* Eastern Standard */
    { "edt",    500+100 },  /* Eastern Daylight */
    { "cst",    600 },  /* Central Standard */
    { "cdt",    600+100 },  /* Central Daylight */
    { "mst",    700 },  /* Mountain Standard */
	{ "mdt",    700+100 },  /* Mountain Daylight */
	{ "pst",    800 },  /* Pacific Standard */
    { "pdt",    800+100 },  /* Pacific Daylight */
    { "yst",    900 },  /* Yukon Standard */
    { "ydt",    900+100 },  /* Yukon Daylight */
    { "hst",   1000 },  /* Hawaii Standard */
    { "hdt",   1000+100 },  /* Hawaii Daylight */
    { "cat",   1000 },  /* Central Alaska */
    { "ahst",  1000 },  /* Alaska-Hawaii Standard */
    { "nt",    1100 },  /* Nome */
    { "idlw",  1200 },  /* International Date Line West */
    { "cet",   -100 },  /* Central European */
    { "met",   -100 },  /* Middle European */
	{ "mewt",  -100 },  /* Middle European Winter */
    { "mest",  -100-100 },  /* Middle European Summer */
    { "swt",   -100 },  /* Swedish Winter */
    { "sst",   -100-100 },  /* Swedish Summer */
    { "fwt",   -100 },  /* French Winter */
    { "fst",   -100-100 },  /* French Summer */
    { "eet",   -200 },  /* Eastern Europe, USSR Zone 1 */
	{ "msk",   -200 },  /* Moscow Standard */
	{ "msd",   -200-100 },  /* Moscow Daylight */
    { "bt",    -300 },  /* Baghdad, USSR Zone 2 */
    { "it",    -330 },/* Iran */
    { "zp4",   -400 },  /* USSR Zone 3 */
    { "zp5",   -500 },  /* USSR Zone 4 */
    { "ist",   -530 },/* Indian Standard */
    { "zp6",   -600 },  /* USSR Zone 5 */
#if 0
    /* For completeness.  NST is also Newfoundland Stanard, nad SST is
     * also Swedish Summer. */
    { "nst",   -630 },/* North Sumatra */
    { "sst",   -700 },  /* South Sumatra, USSR Zone 6 */
#endif
    { "wast",  -700 },  /* West Australian Standard */
    { "wadt",  -700-100 },  /* West Australian Daylight */
    { "jt",    -730 },/* Java (3pm in Cronusland!) */
    { "cct",   -800 },  /* China Coast, USSR Zone 7 */
    { "jst",   -900 },  /* Japan Standard, USSR Zone 8 */
    { "cast",  -930 },/* Central Australian Standard */
	{ "cadt",  -930-100 },/* Central Australian Daylight */
	{ "east",  -1000 }, /* Eastern Australian Standard */
    { "eadt",  -1000-100 }, /* Eastern Australian Daylight */
    { "gst",   -1000 }, /* Guam Standard, USSR Zone 9 */
    { "nzt",   -1200 }, /* New Zealand */
    { "nzst",  -1200 }, /* New Zealand Standard */
    { "nzdt",  -1200-100 }, /* New Zealand Daylight */
    { "idle",  -1200 }, /* International Date Line East */
    {  NULL  }
};

findzone(s)
char *s;
{
	char *p;
	struct TABLE *t;

	for (p = s; *p && *p != ' ' && *p != '\t'; p++)
		;
	*p = '\0';
	for (t = TimezoneTable; t->z_name; t++)
		if (icequal(t->z_name, s))
			return t->z_off;
	return 0;
}

static short mdays[12] = {
   31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static
long
GetTime(date)
char date[];
{
	int i, year, month, day, hour, minute, second, zone;
	char *head, *tail;
	long days, time1970;

	/* Year field */
	head = &date[16];
	tail = &head[4];
	*tail++ = '\0';
	year = atoi(head);
	if (year < 1970)
		year = 1970;

	/* Zone field */
	head = tail;
	zone = -atoi(head);
	if (zone > 1200 || zone < -1200)
		zone = 0;

	/* Months field */
	head = date;
	tail = &head[3];
	*tail++ = '\0';
	month = -1;
	for (i = 0; i < 12; i++)
		if (icequal(months[i], date)) {
			month = i;
			break;
		}
	if (month < 0 || month > 11)
		month = 0;

	/* Day field */
	head = tail;
	tail = &head[2];
	*tail++ = '\0';
	day = atoi(head);
	if (day < 1 || day > 31)
		day = 1;

	/* Hours field */
	head = tail;
	tail = &head[2];
	*tail++ = '\0';
	hour = atoi(head);
	if (hour < 0 || hour > 24)
		hour = 0;

	/* Minutes field */
	head = tail;
	tail = &head[2];
	*tail++ = '\0';
	minute = atoi(head);
	if (minute < 0 || minute > 60)
		minute = 0;

	/* Seconds field */
	head = tail;
	tail = &head[2];
	*tail++ = '\0';
	second = atoi(head);
	if (second < 0 || second > 60)
		second = 0;

	mdays[1] = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
		  ? 29 : 28;
	for (days = day - 1, i = 0; i < month; i++)
		days += mdays[i];
	for (i = 1970; i < year; i++)
		days += 365 + (i % 4 == 0);
	time1970 = second + 60l * (minute + (zone % 100)
				   + 60l * (hour + (zone / 100) + 24 * days));
	if (time1970 < 0)
		time1970 = 0;

	return time1970;
}


/*
 * Unix Date
 */
char *udate()
{
	static char out[50];
	long now;
	struct tm *t;
#if defined(VMUNIX) || defined(__TURBOC__)
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
			 tzname[t->tm_isdst],        /* Name of local timezone */
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
	static char out[50];
	long now;
	struct tm *t;
	int tzshift;
	char tzsign;
#if defined(VMUNIX) || defined(__TURBOC__)
	struct timeb tb;

	ftime(&tb);
	t = localtime(&tb.time);
	tzshift = tb.timezone - (tb.dstflag != 0)*60;
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
			 tzshift,
#ifdef  VMUNIX
			 timezone(tb.timezone, tb.dstflag)
#else
			 tzname[t->tm_isdst]       /* Name of local timezone */
#endif
		);
	return out;
}

#ifndef _time_h_
#       define  _time_h_
#include        <sys/types.h>

struct  tm      { /* see ctime(3) */
	short   tm_sec;
	short   tm_min;
	short   tm_hour;
	short   tm_mday;
	short   tm_mon;
	short   tm_year;
	short   tm_wday;
	short   tm_yday;
	short   tm_isdst;
};

struct  timeval {       /* see select(2x) */
	long tv_sec;
	long tv_usec;
};
#endif  _time_h_

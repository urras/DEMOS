
/* lint junk: */
#ifdef lint		/* Here a is an integer, s a char pointer */
#define ignore(a)	Ignore(a)
#define forget(s)	Forget(s)
#define ignors(s)	Ignors(s)
#else
#define ignore(a)	a
#define forget(s)
#define ignors(s)	s
#endif lint

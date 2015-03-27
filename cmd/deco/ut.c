
# define SYSTEMID "/etc/systemid"

# define DELIM ','

extern char *ctime ();
extern long time ();
extern char *strncpy ();

char *getmachine ()
{
	static char buf [16];
	register f;
	register char *p;

# ifdef HOSTNAME
	if (gethostname (buf, sizeof (buf) - 1) >= 0)
		return (buf);
# endif
	if ((f = open (SYSTEMID, 0)) >= 0) {
		read (f, buf, sizeof (buf) - 1);
		close (f);
		for (p=buf; *p; ++p)
			if (*p<' ' || *p>'~') {
				*p = 0;
				break;
			}
		return (buf);
	}
	return (0);
}

char *extension (p)
register char *p;
{
	register char *s;

	if (p[0]=='.' && p[1]=='.' && p[2]==0)
		return (p+2);
	s = 0;
	while (*++p)
		if (*p == '.')
			s = p;
	return (s ? s : p);
}

char *ltoac (l)
long l;
{
	char num [15];
	static char buf [15];
	register char *p, *q;
	register n;

	sprintf (num, "%ld", l);
	for (p=num; *p; ++p);           /* find end of num */
	q = buf + sizeof (buf) - 1;     /* set q to the end of buf */
	*q-- = 0;
	for (n=0; p>num; ++n) {
		if (n && n%3 == 0)
			*q-- = DELIM;
		*q-- = *--p;
	}
	return (q+1);
}

char *basename (p)
register char *p;
{
	register char *s, *e;
	static char buf [32];

	/* convert "zzz/name" to "name" */
	for (s=p; *s; ++s)
		if (*s == '/')
			p = s+1;
	if (! *p)
		return ("");

	/* convert "name.ext" to "name" */
	for (e=s, s=p+1; *s; ++s)
		if (*s == '.')
			e = s;
	strncpy (buf, p, e-p);
	buf [e-p] = 0;
	return (buf);
}

char *strtail (p, delim, maxlen)
register char *p;
{
	register char *s;

	/* return tail of string */
	for (s=p; *s; ++s);
	s -= maxlen;
	if (s <= p)
		return (p);
	for (; *s; ++s)
		if (*s == delim)
			return (s+1);
	return (s - maxlen);
}

strlencmp (a, b)
register char *a, *b;
{
	register cmp;

	/* if strlen (a) > strlen (b)
	 * then return (1)
	 * else if strlen (a) < strlen (b)
	 * then return (-1)
	 * else return strcmp (a, b)
	 */
	for (cmp=0; *b; ++b, ++a) {
		if (! *a)
			return (-1);    /* strlen (a) < strlen (b) */
		if (! cmp && *a != *b)
			cmp = *a > *b ? 1 : -1;
	}
	if (*a)
		return (1);             /* strlen (a) > strlen (b) */
	return (cmp);                   /* strcmp (a, b) */
}

strbcmp (s, b)
register char *s, *b;
{
	/* compare string s with string b */
	/* return 1 if s == b or s == b+' '+x */

	while (*b && *b == *s)
		++b, ++s;
	if (*b || *s && *s != ' ')
		return (0);
	return (1);
}

/* VARARGS 1 */
outerr (s, par)
char *s, *par;
{
	char buf [80];
	register char *p;

	sprintf (buf, s, par);
	for (p=buf; *p; ++p)
		if (*p == '\033')
			*p = '[';
	write (2, buf, (unsigned) (p - buf));
}

#ifdef pdp11

char * memcpy(to,from,n)
register char *to, *from;
register int   n;
{
	char *beg = to;

	while( --n >= 0 )
		*to++ = *from++;
	return( beg );
}

memcmp(a,b,n)
register char *a, *b;
register int   n;
{
	while( --n >= 0 )
		if( *a++ != *b++ )
			return( (a[-1]&0377)>(b[-1]&0377)?1:-1 );
	return(0);
}

#endif pdp11

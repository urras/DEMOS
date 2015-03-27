#include <stdio.h>
main()
{
register int f;
int c;
	f = creat(".ocopy", 0666);
	while (read(0, &c, 1) == 1) {
		c &= 0377;
		write (1, &c, 1);
		put(c, f);
	}
	fl(f);
	close(f);
}

static char ln[BUFSIZ];
char *p = ln;

put(c, f)
register c, f;
{
	*p++ = c;
	if (c == '\n') {
		fl(f);
		p = ln;
	}
}

fl(f)
register f;
{
	register char *s;

	s = ln;
	if (*s == '$') {
		s++;
		if(*s == ' ') s++;
	}
	if( s < p )
		write(f, s, p-s);
}

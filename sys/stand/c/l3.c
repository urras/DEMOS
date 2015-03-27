/*
 * $Log:	l3.c,v $
 * Revision 1.1  86/04/21  20:05:48  root
 * Initial revision
 * 
 */

/*
 * Convert longs to and from 3-byte disk addresses
 */
ltol3(cp, lp, n)
char    *cp;
long    *lp;
int     n;
{
	register i;
	register char *a, *b;

	a = cp;
	b = (char *)lp;
	for(i=0;i<n;i++) {
#ifdef interdata
		b++;
		*a++ = *b++;
		*a++ = *b++;
		*a++ = *b++;
#else
#ifdef sparc
		*a++ = b[1];
		*a++ = b[3];
		*a++ = b[2];
		b += 4;
#else
		*a++ = *b++;
		b++;
		*a++ = *b++;
		*a++ = *b++;
#endif
#endif
	}
}

l3tol(lp, cp, n)
long    *lp;
char    *cp;
int     n;
{
	register i;
	register char *a, *b;

	a = (char *)lp;
	b = cp;
	for(i=0;i<n;i++) {
#ifdef interdata
		*a++ = 0;
		*a++ = *b++;
		*a++ = *b++;
		*a++ = *b++;
#else
#ifdef sparc
                a[0] = 0;
                a[1] = *b++;
                a[3] = *b++;
                a[2] = *b++;
                a += 4;
#else
		*a++ = *b++;
		*a++ = 0;
		*a++ = *b++;
		*a++ = *b++;
#endif
#endif
	}
}

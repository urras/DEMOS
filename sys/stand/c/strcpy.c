/*
 * $Log:	strcpy.c,v $
 * Revision 1.1  86/04/21  20:06:52  root
 * Initial revision
 * 
 */

/*
 * Copy string s2 to s1.  s1 must be large enough.
 * return s1
 */

char *
strcpy(s1, s2)
register char *s1, *s2;
{
	register char *os1;

	os1 = s1;
	while (*s1++ = *s2++)
		;
	return(os1);
}

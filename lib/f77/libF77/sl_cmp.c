/* $Header: sl_cmp.c,v 1.1 88/12/07 23:54:45 ache Exp $
 * $Log:	sl_cmp.c,v $
 * Revision 1.1  88/12/07  23:54:45  ache
 * Initial revision
 * 
 */
int sl_cmp(a, b, la, lb) /* compare two strings */
register char *a, *b;
long int la, lb;
{
char *aend, *bend;
aend = a + la;
bend = b + lb;
if(la <= lb)
	{
	while(a < aend)
		if(*a != *b)
			return( lcmp (*a, *b) );
		else
			{ ++a; ++b; }
	while(b < bend)
		if(*b != ' ')
			return(' ' - (*b&0377));
		else	++b;
	}
else
	{
	while(b < bend)
		if(*a == *b)
			{ ++a; ++b; }
		else
			return( lcmp (*a, *b) );
	while(a < aend)
		if(*a != ' ')
			return((*a&0377) - ' ');
		else	++a;
	}
return(0);
}

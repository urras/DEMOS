# include	<useful.h>
# include	<sccs.h>

SCCSID(@(#)xalloc.c	7.1	2/5/81)

/*
**  XALLOC -- allocate block of memory.
**
**	This is just like malloc, except that it is guaranteed
**	to succeed.  It will syserr if it fails.
**
**	Parameters:
**		sz -- size in bytes of memory area to allocate.
**
**	Returns:
**		pointer to area allocated.
**
**	Side Effects:
**		none.
**
**	Trace Flags:
**		none.
*/
#ifdef pdp11
extern long all_hi,all_low;
#else
extern unsigned long all_hi,all_low;
#endif
char *
xalloc(sz)
int	sz;
{
	register char	*p;
	extern char	*malloc();
#ifdef pdp11
	long up;
#else
	unsigned long up;
#endif
	p = malloc(sz);
	if (p == NULL)
		syserr("Out of memory");
#ifdef pdp11
	up = (long)p;
#else
	up = (unsigned long)p;
#endif
	if ( all_hi == 0 ) all_hi = all_low = up;
	if ( all_hi <  up ) all_hi = up;
	if ( all_low>  up ) all_low= up;
	return (p);
}

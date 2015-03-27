# include	<sccs.h>

SCCSID(@(#)xfree.c	7.1	2/5/81)

/*
**  XFREE -- free memory only if dynamically allocated.
**
**	This acts just like "free", except that it does nothing
**	if the area handed to it hasn't been dynamically allocated.
**
**	Parameters:
**		p -- a pointer to the area to free.
**
**	Returns:
**		none.
**
**	Side Effects:
**		Free memory queue is changed.
**
**	WARNING:
**		This routine depends on the implementation of malloc
**		in C; it may have to be changed on other systems.
*/
#ifdef pdp11
long all_hi,all_low;
#else
unsigned long all_hi,all_low;
#endif
xfree(p)
char	*p;
{
	extern char	end[];
#ifdef pdp11
	long up;
	up = (long)p;
#else
	unsigned long up;
	up = (unsigned long)p;
#endif
	if (all_hi && up >= all_low && up <= all_hi)
	{
		free(p);
/*
 *              printf("Xfree: High=%lx p=%lx Low=%lx\n",all_hi,up,all_low);
 */
	}
/*
 *      else
 *              printf("Xfree NOT: High=%lx p=%lx Low=%lx\n",all_hi,up,all_low);
 */
}

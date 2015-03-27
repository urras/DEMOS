# include	<stdio.h>
# include	<sccs.h>

SCCSID(@(#)IIsyserr.c	7.1	2/5/81)


/*
**  SYSERR -- SYStem ERRor message print and abort
**
**	Syserr acts like a printf with up to five arguments.
**
**	If the first argument to syserr is not zero,
**	the message "SYSERR:" is prepended.
**
**	If the extern variable `IIproc_name' is assigned to a
**	string, that string is prepended to the message.
**
**	All arguments must be null-terminated.
**
**	The function pointed to by `Exitfn' is then called.
**	It is initialized to be `exit'.
*/
extern char	*IIproc_name;
extern  int     (*IIout_err)();         /* Функция вывода строки */
# define        OUTL    128
	int     (*IIabort)();    /* Программа аварийного завершения.   */

IIsyserr(pv)
char	*pv;
{
	int		pid;
	register char	**p;
	extern int	errno;
	register int	usererr;
	char line_out[OUTL+1];
	register char *s = line_out;
	p = &pv;
	if ( !IIout_err)
		printf("\n");
	usererr = (int)(pv == NULL);
	*s = 0;
	if (!usererr)
	{
		if (IIproc_name)
			IIconcatv(s,IIproc_name," ","SYSERR: ",NULL);
		else
			IIconcatv(s,"SYSERR: ",NULL);
		while (*++s);
	}
	else
		p++;
	sprintf(s,p[0], p[1], p[2], p[3], p[4], p[5]);
	if (IIout_err)
		(*IIout_err)(line_out);
	else
	{
		puts(line_out);
		puts("");
		if (!usererr && errno)
			printf("\tsystem error %d\n", errno);
	}
	fflush(stdout);
	if (IIabort) (*IIabort)(-1);
	exit (-1);
}

# include	<stdio.h>
# include	<defines.h>
# include	<aux.h>
# include	<sccs.h>

SCCSID(@(#)error.c	7.2	5/31/83)
/*
 * ��� ����, ����� ������� CmOfile:
 */
long CmOfile;

# define	ERRDELIM	'~'

/*
**  PROCESS ERROR MESSAGE (Standalone override)
**
**	This routine replaces the "error" routine for use in
**	standalone routines such as creatdb and printr.  Its usage
**	is identical to that of normal "error".
**
**	This routine is assumed to be called by process five; hence,
**	all error messages it produces have a 5000 offset.
**
*/
error(number, argvect)
int	number;
char	*argvect;
{
	FILE		*iop;
	char		**pv;
	int		i;
	register char	*p;
	register int	err;
	char		buf[10];
	register char	c;
	char		*errfilen();

	pv = &argvect;
	err = number;
	if ((iop = fopen(errfilen(5), "r")) == NULL)
		syserr("error: open");

	/* read in the code and check for correct */
	for (;;)
	{
		p = buf;
		while ((c = getc(iop)) != '\t')
		{
			if (c <= 0)
			{
				/* no code exists, print the first parm */
				printf("%d: %s\n\n", err, pv[0]);
				fclose(iop);
				return (err);
			}
			*p++ = c;
		}
		*p = 0;
		i = atoi(buf);
		if (i != err)
		{
			while ((c = getc(iop)) != ERRDELIM)
				if (c <= 0)
					syserr("proc_error: format err %d", err);
			getc(iop);	/* throw out the newline */
			continue;
		}

		/* got the correct line, print it doing parameter substitution */
		printf("%d: ", err);
		c = '\n';
		for (;;)
		{
			c = getc(iop);
			if (c == EOF || c == ERRDELIM)
			{
				printf("\n");
				fclose(iop);
				return (err);
			}
			if (c == '%')
			{
				c = getc(iop);
				for (p = pv[c - '0']; c = *p; p++)
				{
					putchar(c);
				}
				continue;
			}
			putchar(c);
		}
	}
}

nferror(number, arg1, arg2, arg3, arg4, arg5, arg6)
int	number;
{
	error(number, arg1, arg2, arg3, arg4, arg5, arg6);
}

# include	<pipes.h>
# include	<defines.h>
# include	<aux.h>
# include	<version.h>
# include	"IIglobals.h"
# include	<sccs.h>

SCCSID(@(#)IIp_err.c	7.1	2/5/81)


/*
**  IIp_err 
**
**	This routine processes an IIerror call for printout.This involves doing
**	a lookup in the .../files/error? files, where ? is the thous-
**	ands digit of the error number.  The associated error message
**	then goes through parameter substitution and is printed.
**
**	In the current version, the error message is just printed.
*/
# define        OUT            {*linep = 0;if(IIout_err) (*IIout_err)(line_out);else puts(line_out);linep = line_out;}
# define        PUTC(c)        {if(c=='\n' || linep >= linee) OUT else *linep++ = c;}
int     (*IIout_err)();         /* Функция вывода строки */
# define        OUTL    128

IIp_err(err_num, argc, argv,s0)
int		err_num;
int		argc;
char		**argv;
char            *s0;
{
	register char	*p;
	char		buf[512];
	int		i;
	struct iob	iob;
	register short  c;
	extern char	*IIerrfilen();
	extern char	*IIconcatv();
	char line_out[OUTL+1];
	char *linee = line_out + OUTL;
	register char *linep = line_out;

	/* IIfopen the appropriate error file */
	p = buf;
	IIerrfilen(err_num / 1000, p);

#	ifdef xETR2
	if (IIdebug > 1)
		printf("IIp_err : with IIerrfilen ret \"%s\"\n", p);
#	endif

	if (IIfopen(p, &iob) < 0)
		return (0);

	/* read in the code and check for correct */
	for (;;)
	{
		p = buf;
		while ((c = IIgetc(&iob)) != '\t')
		{
			if (c <= 0)
			{
				if (IIclose(&iob) < 0)
					IIsyserr("IIp_err: bad close 1");
				return (0);
			}
			*p++ = c;
		}
		*p = 0;
		if (IIatoi(buf, &i))
			IIsyserr("IIp_err: bad error file %d\n%s",
				err_num, buf);
		if (i != err_num)
		{
			while ((c = IIgetc(&iob)) != ERRDELIM)
				if (c <= 0)
					IIsyserr("IIp_err: format err %d", err_num);
			IIgetc(&iob);	/* throw out the newline */
			continue;
		}

		/* got the correct line, print it doing parameter substitution */
		sprintf(line_out,"%s%d: ", s0, err_num);
		while (*++linep);
		for (;;)
		{
			c = IIgetc(&iob);
			if (c <= 0 || c == ERRDELIM)
			{
				OUT;
				 if (IIclose(&iob) < 0)
					IIsyserr("IIp_err: bad close 2");
				return (1);
			}
			if (c == '%')
			{
				  c = IIgetc(&iob);
				  for (p = argv[c - '0']; c = (*p&0377); p++)
				  {
					if (!p) break;
					if (c < 040 || (c >= 0177 && c<=0277))
					{
						PUTC('^');
						PUTC((c|0100));
					}
					else
						PUTC(c);
				}
				continue;
			}
			PUTC(c);
		}
	}
}

/*
** IIerrfilen -- Returns the pathname where the error file can be found
**	into "buf".
**
**	It is assumed that the error number cannot be more than 999.
**	Returned is concat of : IIPathname, "/files/error", VERSION (w/o mod),
**	"_num".
*/

char *
IIerrfilen(num, buf)
int	num;
char	*buf;
{
	register char	*cp;
	char		ver[12];
	extern char	*IIconcatv();

	IIconcatv(ver, VERSION, 0);

	/* now insert the "_X" */
	cp = &ver[IIlength(ver)];
	*cp++ = '_';
	*cp = '\0';
	IIconcatv(ver, ver, IIitos(num), 0);


	return (IIconcatv(buf, IIPathname, "/files/error", ver, 0));
}

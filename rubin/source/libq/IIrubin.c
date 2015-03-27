# include	<defines.h>
# include	<symbol.h>
# include	<opsys.h>
# include	<aux.h>
# include	"IIglobals.h"
# include	<pipes.h>
# include	<sccs.h>
# include	<signal.h>

SCCSID(@(#)IIrubin.c	7.3	9/26/83)

# define	CLOSED	'?'


# ifdef xV7_UNIX
char	*IImainpr =	"rubin";
# else /* xV7_UNIX */
char	*IImainpr =	"/usr/bin/rubinx";
# endif /* xV7_UNIX */

char	IIPathname[41];

/*
**  IIGETPATH -- initialize the IIPathname variable
**
**	Parameters:
**		none
**
**	Returns:
**		none
**
**	Side Effects:
**		Sets IIPathname to the home directory of the USERRUBIN
**		[unix.h] user.
**
**	Called By:
**		IIrubin.c
**
**	History:
**		3/26/79 -- (marc) written
*/

IIgetpath()
{
	char			line[MAXLINE + 1];
	static char		reenter;
	register int            i,j;
	register char		*lp;
	struct iob		iobuf;
	char			*field[UF_NFIELDS];

	if (reenter)
		return;
	else
		reenter++;

	if ((i = IIfopen("/etc/passwd", &iobuf)) < 0)
		IIsyserr("IIgetpath: no /etc/passwd");

	do
	{
		/* get a line from the passwd file */
		i = 0;
		for (lp = line; (*lp = (j=IIgetc(&iobuf))) != '\n'; lp++)
		{
			if (j == -1)
				IIsyserr("IIgetpath: no user 'rubin' in /etc/passwd");
			if (++i > sizeof line - 1)
			{
				*lp = '\0';
				IIsyserr("IIgetpath: line overflow: \"%s\"",
				line);
			}
		}
		*lp = '\0';
		for (i = 0, lp = line; *lp != '\0'; lp++)
		{
			if (*lp == ':')
			{
				*lp = '\0';
				if (i > UF_NFIELDS)
					continue;
				field[i++] = lp + 1;
			}
		}
		/* check for enough fields for valid entry */
	}  while (i<3 || i>UF_NFIELDS || !IIsequal(line, USERRUBIN));
	IIclose(&iobuf);

	/* check that pathname won't overflow static buffer */
	if (field[i - 1] - field[i - 2] > sizeof IIPathname)
		IIsyserr("IIgetpath: path too long \"%s\"", field[i - 2]);

	/* move pathname into buffer */
	IIbmove(field[i - 2], IIPathname, field[i - 1] - field[i - 2]);
}

/*
**	IIrubin opens the needed pipes and
**	forks an rubin process.
**
**	rubin recognizes the EQUEL flag followed by
**	three control characters as being an equel processes
**
**	parameters to rubin are passed directly. only
**	the first 9 are counted.
*/

IIrubin(p1, p2, p3, p4, p5, p6, p7, p8, p9)
char	*p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;
{
	int		pipes[4];		/* pipe vector buffer */
	char		eoption[15];	/* dummy variable to hold -EQUEL option */
	register char	*cp;
	char		*argv[12];
	register char	**ap;
	extern		IIresync();
	extern		*(IIinterrupt)(),	exit();
	pb_t		pb;

#	ifdef xETR1
	if (IIdebug)
		printf("IIrubin\n");
#	endif
	/* test if rubin is already invoked */
	if (IIingpid)
		IIsyserr("Attempt to invoke RUBIN twice");

	IIgetpath();
	/* open RUBIN pipes */
	if (pipe(&pipes[0]) || pipe(&pipes[2]))
		IIsyserr("pipe error in IIrubin");

	IIw_down = pipes[1];	/* file desc for equel->parser */
	IIr_down = pipes[2];	/* file desc for parser->equel */

	/* catch interupts if they are not being ignored */
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, IIresync);

	/* set up equel option flag */
	cp = eoption;
	*cp++ = '-';
	*cp++ = EQUEL;
	*cp++ = pipes[0] | 0100;
	*cp++ = pipes[1] | 0100;
	*cp++ = CLOSED;
	*cp++ = pipes[3] | 0100;
	*cp++ = CLOSED;	/* this will be the equel->ovqp pipe in the future */
	*cp++ = CLOSED;	/* old ovqp->equel pipe */
	/* put "6.3" at end of flag for OVQP to not do flush after
	 * every tuple
	 */
	*cp++ = '6';
	*cp++ = '.';
	*cp++ = '3';
	*cp = '\0';
	if (cp - eoption >= sizeof eoption)
		IIsyserr("IIrubin: too big an eoption");

	if ((IIingpid = fork()) < 0)
		IIsyserr("IIrubin:cant fork %d", IIingpid);
	/* if parent, close the unneeded files and return */
	if (IIingpid)
	{
		if (close(pipes[0]) || close(pipes[3]))
			IIsyserr("close error 1 in IIrubin");
		IIinput = IIr_down;
		IIpb_prime(&pb, PB_NOTYPE);
#		ifdef xETR1
		if (IIdebug)
			printf("calling rubin with '%s' database %s\n", eoption, p1);
#		endif
		return;
	}
	/* the child overlays /usr/bin/rubin */
	ap = argv;
	*ap++ = "rubin";
	*ap++ = eoption;
	*ap++ = p1;
	*ap++ = p2;
	*ap++ = p3;
	*ap++ = p4;
	*ap++ = p5;
	*ap++ = p6;
	*ap++ = p7;
	*ap++ = p8;
	*ap++ = p9;
	*ap = 0;
# ifdef xV7_UNIX
	execvp(IImainpr, argv);
# else /* xV7_UNIX */
	execv(IImainpr, argv);
# endif /* xV7_UNIX */
	IIsyserr("cannot exec RUBIN in IIrubin");
}

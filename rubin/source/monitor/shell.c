# include	"monitor.h"
# include	<defines.h>
# include	<aux.h>
# include	<opsys.h>
# include	<sccs.h>

SCCSID(@(#)shell.c	7.1	2/5/81)



/*
**  SHELL -- call unix shell
**
**	The UNIX shell is called.  Shell() first tries to call an
**	alternate shell defined by the macro {shell}, and if it fails
**	calls /bin/sh.
**
**	If an argument is supplied, it is a shell file which is
**	supplied to the shell.
**      exec=1 - call command (sh -c)
**	Trace Flags:
**		34
*/

shell(exec)
{
	register int	i;
	register char	*p;
	register char	*shellfile;
	extern char	*getfilenm();
	extern char	*macro();
	char *a1,*a2;
	if (exec)
	{
		a1 = "-c";
		a2 = getfilenm(1);
	}
	else
	{
		a1 = getfilenm(0);
		if (*a1 == 0)
			a1 = NULL;
		a2 = Qbname;
	}
	fclose(Qryiop);
	if ((Xwaitpid = fork()) == -1)
		syserr("shell: fork");
	if (Xwaitpid == 0)
	{
		setuid(getuid());
#		ifndef xB_UNIX
		setgid(getgid());
#		endif
		for (i = 3; i < NOFILE; i++)
			close(i);
		p = NULL;
		if (!exec)
			p = macro("{shell}");
#		ifdef xMTR3
		tTfp(34, 0, "{shell} = '%o'\n", p);
#		endif
		if (p != 0)
		{
			execl(p, p, a1, a2, 0);
			printf("Cannot call %s; using /bin/sh\n", p);
		}
		execl("/bin/sh", "sh", a1, a2, 0);
		syserr("shell: exec");
	}

	if (Nodayfile >= 0 && !exec)
		printf(">>shell\n");
	/* wait for shell to complete */
	xwait();
}

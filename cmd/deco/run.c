# include <signal.h>
# include <errno.h>
# include <setjmp.h>
# include "mem.h"
# include "env.h"

# ifndef NULL
#    define NULL ((char *) 0)
# endif

# ifdef VFORK
#    define FORK vfork
# else
#    define FORK fork
# endif

# define QUANT 16

typedef (*funcptr) ();

static char **arg;
static arglen, argp;

# ifdef SIGTSTP
static jmp_buf beforewait;
# endif /* SIGTSTP */

static run ();

# ifdef SIGTSTP
int tstp ();
# endif

extern errno;
extern execve ();

runset (dest)
char *dest;
{
	arg = (char **) MemAlloc (QUANT * sizeof (char *));
	arglen = QUANT;
	arg [0] = dest;
	argp = 1;
}

runarg (name)
char *name;
{
	MemCheckIndex (arg, char **, arglen, QUANT, argp);
	arg [argp++] = name;
}

rundone (name, a0)
char *name, *a0;
{
	register char **p, **q, *a;
	int ret;

	runarg (a0);
	for (p=arg, q=arg+argp-1; p<q; ++p, --q) {
		a = *p;
		*p = *q;
		*q = a;
	}
	runarg (NULL);
	ret = runvs (name, arg) == 0;
	MemFree ((mem *) arg);
	return (ret);
}

runcancel ()
{
	MemFree ((mem *) arg);
}

static char *execat (s1, s2, si)
register char *s1, *s2;
char *si;
{
	register char *s;

	s = si;
	while (*s1 && *s1!=':')
		*s++ = *s1++;
	if (si != s)
		*s++ = '/';
	while (*s2)
		*s++ = *s2++;
	*s = 0;
	return (*s1 ? ++s1 : 0);
}

static execvpe (name, argv, envstr)
char *name, **argv, **envstr;
{
	static char *pathstr;
	char fname [128];
	char *newargs [256];
	int i;
	register char *cp;
	register unsigned etxtbsy = 1;
	register eacces = 0;

	if (! pathstr && ! (pathstr = EnvGet ("PATH")))
		pathstr = ":/bin:/usr/bin";
	for (cp=name; ; ++cp) {         /* check if name contains '/' */
		if (! *cp) {
			cp = pathstr;   /* if no, set cp to pathstr */
			break;
		}
		if (*cp == '/') {
			cp = "";        /* else path is empty */
			break;
		}
	}
	do {
		cp = execat (cp, name, fname);
retry:          execve (fname, argv, envstr);
		switch (errno) {
		case ENOEXEC:
			newargs [0] = "sh";
			newargs [1] = fname;
			for (i=1; newargs [i+1] = argv [i]; ++i) {
				if (i >= 254) {
					errno = E2BIG;
					return (-1);
				}
			}
			execve ("/bin/sh", newargs, envstr);
			return (-1);
		case ETXTBSY:
			if (++etxtbsy > 5)
				return (-1);
			sleep (etxtbsy);
			goto retry;
		case EACCES:
			++eacces;
			break;
		case ENOMEM:
		case E2BIG:
			return (-1);
		}
	} while (cp);
	if (eacces)
		errno = EACCES;
	return (-1);
}

/* VARARGS 2 */
runl (name, a0)
char *name, *a0;
{
	return (run (name, &a0, execve, 0));
}

/* VARARGS 2 */
runls (name, a0)
char *name, *a0;
{
	return (run (name, &a0, execve, 1));
}

/* VARARGS 2 */
runvp (name, a0)
char *name, **a0;
{
	return (run (name, a0, execvpe, 0));
}

/* VARARGS 2 */
runvs (name, a0)
char *name, **a0;
{
	return (run (name, a0, execve, 1));
}

static run (name, a0, exe, silent)
char *name, **a0;
int (*exe) ();
{
	register t;
	int status;

	if ((t = FORK ()) == -1)
		/* cannot fork */
		return (0x7e00);
	if (t == 0) {
		/* child */
		if (silent) {
			close (0);
			close (1);
			close (2);
		}
		sigdfl ();
		(*exe) (name, a0, EnvVector);
		if (! silent)
			outerr ("%s: Command not found.\n", name);
		_exit (0x7f);                   /* file not found */
	}
# ifdef SIGTSTP
	if (setjmp (beforewait)) {
		outerr ("\n[%d]", t);
		for (; *a0; ++a0) {
			write (2, " ", 1);
			write (2, *a0, strlen (*a0));
		}
		write (2, " &\n", 3);
		signal (SIGTSTP, SIG_IGN);
		return (0);
	}
	signal (SIGTSTP, tstp);
# endif
	/* parent */
	while (t != wait (&status));
# ifdef SIGTSTP
	signal (SIGTSTP, SIG_IGN);
# endif
	return (status);
}

sigign ()
{
	signal (SIGTERM, SIG_IGN);
	signal (SIGQUIT, SIG_IGN);
	signal (SIGINT, SIG_IGN);
# ifdef SIGTSTP
	signal (SIGTSTP, SIG_IGN);
# endif
}

sigdfl ()
{
	signal (SIGTERM, SIG_DFL);
	signal (SIGQUIT, SIG_DFL);
	signal (SIGINT, SIG_DFL);
}

# ifdef SIGTSTP
tstp ()
{
	longjmp (beforewait, -1);
}
# endif /* SIGTSTP */

#ifndef	lint
static char *sccsid = "@(#)lastcomm.c	4.3 (Berkeley) 12/14/80";
#endif
#

/*
 * last command
 */

# include <stdio.h>
# include <sys/types.h>
# include <sys/acct.h>
# include <signal.h>
# include <pwd.h>
# include <stat.h>

# define N_USER		1000

struct acct	acct_buff [BUFSIZ / sizeof (struct acct)];

char	yes   = 1,
	no    = 0,

	user_list [N_USER][9];

time_t	expand ();

struct passwd
	*passwd,
	*getpwent ();

struct stat stat_buff;

main (argc, argv)
char **argv;
{
	char	acct_desc,
		*p;

	long	i,
		j,
		i_block,
		n_blocks,
		n_byte,
		n_entry;

	float	x;

/*
 * set up user names
 */
	while (passwd = getpwent ())
	{
		if (passwd->pw_uid > N_USER)
		{
			printf ("lastcomm:Too large of a user id number\n");
			return;
		}
		if (user_list[passwd->pw_uid][0]==0)
			move (passwd->pw_name, user_list [passwd->pw_uid]);
	}

	acct_desc = open ("/usr/adm/acct", 0);
	if (acct_desc < 0)
	{
		perror ("/usr/adm/acct");
		return;
	}
	fstat (acct_desc, &stat_buff);
	n_blocks = (stat_buff.st_size + BUFSIZ - 1) / BUFSIZ;

	/*
	 * read one block's worth
	 */
	for (i_block = n_blocks - 1; i_block >= 0; i_block--)
	{
		lseek (acct_desc, i_block * BUFSIZ, 0);
		n_byte = read (acct_desc, (char *)acct_buff, BUFSIZ);
		n_entry = n_byte / sizeof acct_buff [0];
		for (i = n_entry - 1; i >= 0; i--)
		{
			if (acct_buff [i].ac_uid >= N_USER)
			{
				printf (
				"lastcomm:Too large of a user id number\n");
				return;
			}
			if (!*user_list [acct_buff [i].ac_uid]) continue;
			/*
			 * get the times
			 */
			x =	expand (acct_buff [i].ac_utime)
				+
				expand (acct_buff [i].ac_stime);
			/*
			 * null terminate the command name
			 */
			acct_buff [i].ac_comm [10] = 0;
			/*
			 * replace missing command names with question marks
			 */
			if (!*acct_buff [i].ac_comm)
			{
				move ("?", acct_buff [i].ac_comm);
			}
			/*
			 * replace control characters with question marks
			 */
			for (p = acct_buff [i].ac_comm; *p; p++)
			{
				if (*p < '!' || '~' < *p) *p = '?';
			}
			for (j = 1; j < argc; j++)
			{
				if
				(
					equal (acct_buff [i].ac_comm, argv [j])
					||
					equal
					(
					user_list [acct_buff [i].ac_uid],
						argv [j]
					)
				)
				{
					break;
				}
			}
			if (argc == 1 || j != argc)
			{
				printf
				(
					"%-10s %-8s %6.2f     %.16s\n",
					acct_buff [i].ac_comm,
					user_list [acct_buff [i].ac_uid],
					x / 60.0,
					ctime (&acct_buff [i].ac_btime)
				);
			}
		}
	}
}

time_t
expand (t)
comp_t t;
{
	register time_t nt;

	nt = t & 017777;
	t >>= 13;
	while (t)
	{
		t--;
		nt <<= 3;
	}
	return (nt);
}

move (a, b)
register char *a, *b;
{
	while (*b++ = *a++);
}

equal (a, b)
register char *a, *b;
{
	for (;; a++, b++)
	{
		if (*a != *b) return no;
		if (!*a)      return yes;
	}
}

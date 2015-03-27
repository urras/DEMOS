# include	<stdio.h>
# include	<defines.h>
# include	<aux.h>
# include	<lock.h>
# include	<pv.h>
# include	<sccs.h>
# include	<opsys.h>
# include	<sys/dir.h>
# include       <access.h>
# include       <code.h>

SCCSID(@(#)purge.c	7.4	7/7/83)

/*
**  PURGE DATABASE
**
**	This stand-alone routine cleans up a database.  This includes:
**
**	- Destroy temporary relations, i.e., relations with names
**		beginning with "_SYS".
**	- Destroy expired relations
**	- Clean out junk files, such as core, etc.
**	- As a suggested future expansion, reformat relations which
**		have many overflow pages, or heaps with lots of old
**		deleted tuples, etc.
**
**	It may be called by the rubin superuser or by the dba of
**	a database.  There are two modes.  The first is where databases
**	to be purged are explicitly named.  If none are named, then
**	all databases owned by the particular user (or all databases
**	if the RUBIN superuser) are purged.
**
**	Flags:
**	-p	enable the purge feature, i.e., clean out expired
**		relations as well as temporary relations.
**	-s	attempt to run in superuser mode.  The user must be
**		login "rubin" for this to succeed.
**	-a	ask the user before each database.
**	-f	clean out rather than report junk files.
**
**	(8/2/82 peter@lbl-unix)
**		allow dba to destroy user relations by suitable manipulation
**		of Usercode
**		added ExitFn = nullfn() to avoid untimely aborts on syserrs.
**	(5/13/83 peter@lbl-csam)
**		folded in new directory rootines for 4.1c-bsd
*/


char		All;
char		Superuser;
char		Ask;
char		Purge;
char		Clean;
extern int	Wait_action;
extern int	Status;
extern char	*Usercode;
long		Today;
short		tTdbu[100];
extern int	(*ExitFn)();

main(argc, argv)
int	argc;
char	*argv[];
{
	register char	*db;
	register int	i;
	char		fake[256];
	extern char	*getnxtdb();
	extern int	null_fn();

	argv[argc] = NULL;
#	ifdef xTTR1
	tTrace(argv, 'T', tTdbu, 100);
#	endif

	/* set up arguments and operating modes */
	initialize(argc, argv);
	time(&Today);
#	ifdef	xTTR2
	tTfp(10, 2, "Usercode: %.2s\n", Usercode);
#	endif

	/* Assign ExitFn so syserr (e.g. from destroy) does not cause aborts */
	ExitFn = null_fn;

	while (db = getnxtdb())
	{
		purgedb(db);
	}
	printf("\npurge completed\n");
}


rubproc()
{
	unldb();
	exit(-1);
}



/*
**  PURGE DATABASE
**
**	The database is purged of temporaries, expired relations, and
**	junk.
*/

extern DESC	Reldes;

#ifndef DIRBLKSIZ
typedef FILE DIR;
#endif
extern DIR *opendir();
extern struct direct *readdir();


purgedb(db)
register char	*db;
{
	struct relation		rel, key;
	TID			rtid, rlimtid;
	register int            i;
	register char		c;
	long			l;
	DIR			*dirp;
	struct direct		*dp;
	int			darg[3];
	PARM			pv[2];
	char			pbuff[MAXNAME + 1];
	char			*fname;
#ifndef DIRBLKSIZ
	char			fnambuf[DIRSIZ + 1];
#endif

#	ifdef	xTTR2
	tTfp(11, 0, "entered purgedb(%s)\n", db);
#	endif
	printf("Database %s", db);
	if (!ask("? "))
		return;
	if (!Ask)
		printf(":\n");
	acc_init();

	/* set exclusive lock on data base */
#	ifdef	xTTR2
	tTfp(11, 1, "calling db_lock(%d)\n", M_EXCL);
#	endif
	db_lock(M_EXCL);

	/* open the relation relation for read-write */
	opencatalog("relation", 2);

	if (find(&Reldes, NOKEY, &rtid, &rlimtid))
	{
		printf("\tcannot find in %s\n", db);
		closecatalog(TRUE);	/* really close cache */
		unldb();		/* unlock the database */
		acc_close();
		return;
	}

	while (get(&Reldes, &rtid, &rlimtid, &rel, 1) == 0)
	{
		i = 0;

		/* check for temp rel */
		if (bequal(rel.relid, "_SYS", 4))
		{
			printf("\t%.14s: temporary", rel.relid);
			i = 1;
		}
		else if (rel.relsave < Today && rel.relsave != 0)
		{
			printf("\t%.14s: expired", rel.relid);
			if (Purge)
				if (ask("\n\t\tPURGE? "))
					i = 1;
		}
		else
			i = -1;

		/* if this relation should be purged -- call (local) destroy */
		if (i ==1)
		{
			char	*usave;
			printf("%d > %d \tpurging\n",i,0);

			/* allow DBA to purge other users relations */
			usave = 0;
			if (!bequal(rel.relowner, Usercode, 2)) {
				usave = Usercode;
				Usercode = rel.relowner;
			}

			/* set up parameter vector for destroy */
			bmove(rel.relid, pbuff, MAXNAME);
			pbuff[MAXNAME] = '\0';
			pv[0].pv_type = PV_STR;
			pv[0].pv_val.pv_str = pbuff;
			pv[1].pv_type = PV_EOF;
			pv[1].pv_val.pv_str = NULL;
			if (destroy(1, pv) != 0)
				syserr("cannot destroy %s\n", pbuff);
			if (usave)
				Usercode = usave;
			closecatalog(FALSE);	/* to flush */
		}
		else if (i == 0)
			printf("\t\t(not purged)\n");
	}

	/* open the directory to check for extra files */
	if ((dirp = opendir(".")) == NULL)
	{
		printf("\tcannot open .\n");
		closecatalog(TRUE);		/* really */
		unldb();		/* unlock the database */
		acc_close();
		return;
	}

	/* scan the directory */
	for (dp = readdir(dirp); dp != NULL; dp = readdir(dirp)) {

#ifdef DIRBLKSIZ
		fname = dp->d_name;
		if (dp->d_namlen <= 2)
			continue;
#else /* DIRBLKSIZ */
		strncpy(fnambuf, dp->d_name, DIRSIZ);
		fnambuf[DIRSIZ] = '\0';
		fname = fnambuf;
		if (!strcmp(fname, ".") || !strcmp(fname, ".."))
			continue;
#endif /* DIRBLKSIZ */

		/* throw out legitimate files */
		if (sequal(fname, "admin"))
			continue;

		/* always purge _SYS files */
		if (!bequal(fname, "_SYS", 4))
		{
			if (fname[13] != 0)
			{
				/* 1. Это м. быть отношение */
				clearkeys(&Reldes);
				setkey(&Reldes, &key, fname, RELID);
				setkey(&Reldes, &key, &fname[MAXNAME], RELOWNER);
				if (getequal(&Reldes, &key, &rel, &rtid) <= 0)
				{
					/* it is a relation (or should be saved) */
					continue;
				}
# ifdef NOCYRFILES
				/* 2. Отношение могло иметь русское имя */
				if (Admin.adhdr.adflags & A_CYRCNV)
				{
					register char *p;
					p = fname;
					while (*p)
					{
						*p = to8bits(*p);
						p++;
					}
					clearkeys(&Reldes);
					setkey(&Reldes, &key, fname, RELID);
					setkey(&Reldes, &key, &fname[MAXNAME], RELOWNER);
					if (getequal(&Reldes, &key, &rel, &rtid) <= 0)
					{
						/* it is a relation (or should be saved) */
						continue;
					}
				}
# endif
			}

			/* it is a funny file!!! */
			if (!Clean)
			{
				printf("\t%s: file (not unlinked)\n", fname);
				continue;
			}
		}

		/* purge the file */
		printf("\tunlinking %s\n", fname);
		if (unlink(fname))
			printf("\tcannot unlink\n");
	}
	closecatalog(TRUE);	/* close catalogs */
	unldb();		/* unlock the database */
	acc_close();
	closedir(dirp);
}

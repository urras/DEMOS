# include	<defines.h>
# include	<lock.h>
# include	<sccs.h>

SCCSID(@(#)dblocks.c	7.2	5/19/83)
#ifndef FCNTL
struct	lockreq	Lock;
/*
 *	setdbl - set data base lock for either exclusive or shared
 *		 access.
 */
setdbl(act, mod)
char	act;			/* type request */
char	mod;			/* mod of lock: = 1 for exclusive, = 2 for shared*/
{
	register char	*r;
	register int	i;
	auto	int	ret_val;

#	ifdef xATR1
	if ( tTf(28,6) )
		printf(" setdbl act=%o md=%o\n", act, mod);
#	endif
	if (Alockdes < 0)
		return (1);
	Lock.lract = act;		/* type of request */
	Lock.lrtype = T_DB;		/* data base lock */
	Lock.lrmod = mod;		/* exclusive or shared */
					/* zero out rest of key */
	r = Lock.lrel;
	for (i = 0; i < 8; i++)
		*r++ = 0;
	LOCKOP(Alockdes, &Lock,ret_val);
	return (ret_val);
}
/*
 *	unldb	- release the data base lock
 */
unldb()
{
	register char	*r;
	register int	i;
	auto	int	ret_val;

#	ifdef xATR1
	if (tTf(28, 7))
		printf(" unldb\n");
#	endif
	if (Alockdes < 0)
		return (1);
	Lock.lract = A_RLS1;		/* release 1 lock */
	Lock.lrtype = T_DB;		/* a data base lock*/
	r = Lock.lrel;
	for (i = 0; i < 8; i++)		/* zero out part of key*/
		*r++ = 0;
	LOCKOP(Alockdes, &Lock,ret_val);
	return (ret_val);
}

#else /* + FCNTL */

/*
 *	setdbl - set data base lock for either exclusive or shared
 *		 access.
 */
setdbl(act, mod)
char	act;			/* type request */
char	mod;			/* mod of lock: = 1 for exclusive, = 2 for shared*/
{
#	ifdef xATR1
    if ( tTf(28, 6) )
        printf(" setdbl act=%o md=%o\n", act, mod);
#	endif
    return(set_lock(
        Alockdes,
        (act == A_RTN ? F_SETLK : F_SETLKW),
	(mod == M_SHARE ? F_RDLCK : F_WRLCK),
        DB_OFF, 1) );
}


/*
 *	unldb	- release the data base lock
 */
unldb()
{
    return(set_lock(Alockdes, F_SETLK, F_UNLCK, DB_OFF, 1) );
}


#endif



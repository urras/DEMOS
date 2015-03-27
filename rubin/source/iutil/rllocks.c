# include	<defines.h>
# include	<lock.h>
# include	<sccs.h>
# include	<signal.h>

SCCSID(@(#)rllocks.c	7.2	5/30/83)

#ifndef FCNTL
struct lockreq	Lock;
/*
 *	setrll- set a relation lock
 */
setrll(act, rtid, mod)
char	act;
long	rtid;
char	mod;
{
	register char	*r;
	register int	i;
	auto	int	ret_val;

#	ifdef xATR1
	if ( tTf(28,4) )
	{
		printf(" setrll act=%d md=%o ", act, mod);
		dumptid(&rtid);
	}
#	endif
	if (Alockdes < 0)
		return(1);
	Lock.lract = act;	/* sleep (act = 2) or error return (act = 1)*/
	Lock.lrtype = T_REL;	/* relation lock */
	Lock.lrmod = mod;	/* exclusive (mod = 1) or shared (mod = 2)*/
	bmove(&rtid, Lock.lrel, 4);	/* copy relation id */
	r = Lock.lpage;

	/* zero out page id */
	for (i = 0; i < 4; i++)
		*r++ = 0;
	LOCKOP(Alockdes, &Lock,ret_val);
	return (ret_val);
}
/*
 *	unlrel- unlock a relation lock
 */
unlrl(rtid)
long	rtid;
{
	register char	*r;
	register int	i;
	auto	int	ret_val;

#	ifdef xATR1
	if (tTf(28, 5))
	{
		printf(" unlrl ");
		dumptid(&rtid);
	}
#	endif
	if (Alockdes < 0)
		return (1);
	Lock.lract = A_RLS1;
	Lock.lrtype = T_REL;	/* relation lock */
	bmove(&rtid, Lock.lrel, 4);	/* copy relation id */
	r = Lock.lpage;
	for (i = 0; i < 4; i++)
			/* zero out pageid */
		*r++ = 0;
	LOCKOP(Alockdes, &Lock,ret_val);
	return (ret_val);
}

#else
/*
 *	setrll- set a relation lock
 */
setrll(act, rtid, mod)
char	act;
long	rtid;
char	mod;
{
#	ifdef xATR1
    if ( tTf(28, 4) )
	{
		printf(" setrll act=%o md=%o\n", act, mod);
		dumptid( &rtid);
	}
#	endif
    return(set_lock(
	Alockdes,
	(act == A_RTN ? F_SETLK : F_SETLKW),
	(mod == M_SHARE ? F_RDLCK : F_WRLCK),
	RL_OFF(rtid), 1) );
}

/*
 *	unlrel- unlock a relation lock
 */
unlrl(rtid)
long	rtid;
{
    return(set_lock(Alockdes, F_SETLK, F_UNLCK, RL_OFF(rtid), 1) );
}
#endif /* FCNTL */

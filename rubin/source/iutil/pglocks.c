# include	<defines.h>
# include	<access.h>
# include	<lock.h>
# include	<sccs.h>
# include       <opsys.h>

SCCSID(@(#)pglocks.c	7.2	5/30/83)

#ifndef FCNTL
struct lockreq	Lock;
/*
 *	setpgl- sets a lock for the access buffer
 */
setpgl(buf)
struct	accbuf	*buf;
{
	register struct accbuf	*b;
	register int		i;
	auto	int	ret_val;

#	ifdef xATR1
	if ( tTf(28,2) )
	{
		printf(" setpgl pg=%ld rel", buf->thispage);
		dumptid(&buf->rel_tupid);
	}
#	endif
	if (Alockdes < 0)
		return(1);
	b = buf;
	Lock.lract = A_SLP;	/* wait for lock */
	Lock.lrtype = T_PAGE;	/* page lock */
	Lock.lrmod = M_EXCL;	/* exclusive lock */
	bmove(&b->rel_tupid, Lock.lrel, 4);	/* copy relation id */
	bmove(&b->thispage, Lock.lpage, 4);	/* copy page id */
	LOCKOP(Alockdes, &Lock,ret_val);
	b->bufstatus |= BUF_LOCKED;
	return (ret_val);
}

/*
 *	unlpg- releases a page lock
 */
unlpg(buf)
struct	accbuf	*buf;
{
	register struct	accbuf	*b;
	register int		i;
	auto	int	ret_val;

#	ifdef xATR1
	if (tTf(28, 3))
	{
		printf(" unlpg page %ld rel", buf->thispage);
		dumptid(&buf->rel_tupid);
	}
#	endif
	if (Alockdes < 0)
		return(1);
	b = buf;
	Lock.lract = A_RLS1;
	bmove(&b->rel_tupid, Lock.lrel, 4);	/* copy relation id */
	Lock.lrtype = T_PAGE;	/* page lock */
	bmove(&b->thispage, Lock.lpage, 4);	/* copy page id */
	b->bufstatus &= ~BUF_LOCKED;
	LOCKOP(Alockdes, &Lock,ret_val);
	return (ret_val);
}

/*
 *	unlall - release all locks held by this process
 */
unlall()
{
	register int	i;
	auto	int	ret_val;


#	ifdef xATR1
	if (tTf(28, 6))
		printf(" unlall\n");
#	endif

	Acclock = TRUE;	/* reset page lock flag just in case */
	if (Alockdes < 0)
		return(1);
	Lock.lract = A_RLSA;
	LOCKOP(Alockdes, &Lock,ret_val);
	return (ret_val);
}

#else
static char	locked[NOFILE];
setpgl(buf)
register struct accbuf *buf;
{
    register int	i;
    i = buf->filedesc;
    if (i < 0 || i >= NOFILE) 
        return( -1);
    locked[i] = 1;
    buf->bufstatus |= BUF_LOCKED;
#	ifdef xATR1
    if (tTf(28, 3)) {
        printf(" unlpg page %ld rel", buf->thispage);
        dumptid(&buf->rel_tupid);
    }
#	endif
    return(
        set_lock(i, F_SETLKW, F_WRLCK, (long)buf->thispage * (long)PGSIZE, PGSIZE)
        );
}


/*
 *	unlpg- releases a page lock
 */
unlpg(buf)
register struct accbuf *buf;
{
    register int	i;
#	ifdef xATR1
    if (tTf(28, 3)) {
        printf(" unlpg page %ld rel", buf->thispage);
        dumptid(&buf->rel_tupid);
    }
#	endif
    i = buf->filedesc;
    buf->bufstatus &= ~BUF_LOCKED;
    if (i < 0 || i >= NOFILE)
        return( 1);
    locked[i] = 0;
    return(
        set_lock(i, F_SETLKW, F_UNLCK, (long)buf->thispage * (long)PGSIZE, PGSIZE)
        );
}


/*
 *	unlall - release all locks held by this process
 */
unlall()
{
    register int	i;
#	ifdef xATR1
    if (tTf(28, 6))
        printf(" unlall\n");
#	endif
    Acclock = TRUE;	/* reset page lock flag just in case */
    for (i = 0; i < NOFILE; i++)
        if (locked[i]) {
            locked[i] = 0;
            set_lock(i, F_SETLKW, F_UNLCK, 0l, 0);
        }
    return(
        set_lock(Alockdes, F_SETLKW, F_UNLCK, DB_OFF + 1l, 0)
        );
}

#endif /* FCNTL */

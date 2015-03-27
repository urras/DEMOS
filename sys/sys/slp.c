/*
 * $Log:	slp.c,v $
 * Revision 22.3  90/11/12  19:17:48  root
 * ����� ���� ��� ��1425 � ����������� include.
 * 
 * Revision 22.2  89/07/08  18:33:52  avg
 * �������� ������ by korotaev (see diff 1.4 --> 1.5): 
 * ������ ��������� SETRQ.
 * ������ ������� ��� �������� �� 8086.
 * 
 * Revision 22.1  89/04/12  15:34:18  korotaev
 * "param.h" ==> <sys/param.h>
 *
 * Revision 22.0  89/03/30  16:32:59  korotaev
 * Begining DEMOS 2.2.
 *
 * Revision 1.9  89/03/04  14:23:07  abs
 * � sleep �������� ���� PCATCH a la systemV
 *
 * Revision 1.8  89/01/20  20:20:43  abs
 * new malloc (mexpand)
 *
 * Revision 1.8  89/01/16  11:54:26  abs
 * ������� �������� ���������� �������� data. ���� ����� ���� ���� ���������
 * ����� - ��� �������������� (� �� ���� ������������ ������� � ����� �����
 * ��� ���� ������).
 *
 * Revision 1.7  89/01/07  20:52:14  korotaev
 * ������� � ������������� ������ FIFO, +fcntl, + ������ ������ ��������
 *
 * (from VAX/UTS SV.2)
 *
 * Revision 1.6  88/11/02  12:47:27  dvolodin
 * *** empty log message ***
 *
 * Revision 1.5  88/03/23  13:36:14  korotaev
 * ��������� ����� ������� � ����, �-85 � ���������� �������������
 *
 * Revision 1.4  87/04/18  14:21:52  avg
 * ��������� ����������� by alex.
 *
 * Revision 1.3  87/01/22  08:33:13  alex
 * ������ ��� ���������� �������� text � ������ ����� ������� ��������.
 * + ������ �������� FXMETER ��� ��������� ������� �� ����� ������������.
 *
 * Revision 1.2  87/01/19  22:52:11  alex
 * ����� spl6() �� ����� �� ������� ��������� �
 * ������ ��������. ��� ����� ������, ��� ��� ���� ����
 * � ���������� wakeup �� ����� ������, ������ ���������
 * �� ��������.
 *
 * Revision 1.1  86/04/19  15:51:50  avg
 * Initial revision
 *
 */

#define IPK_MEXPAND
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/text.h>
#include "../include/map.h"
#include <sys/file.h>
#include <sys/inode.h>
#include "../include/buf.h"
#include <sys/seg.h>
#ifdef  UCB_METER
#include <sys/vm.h>
#endif
#include "../include/inline.h"

#ifdef  UCB_FRCSWAP
extern  int     idleflg ;       /* If set, allow incore forks and expands */
				/* Set before idle(), cleared in clock.c */
#endif

#ifdef  CGL_RTP
int     wantrtp;        /* Set when the real-time process is runnable */
#endif

#ifdef SCHED
#define TTIINC          6       /* tty input increment */
#define TTOINC          4       /* tty output increment */
#define OTHINC          2       /* increment for other sleeps */

short curlevel = 0;
#define QUANTUM 2       /* CPU quantum -- must be <= 7 to avoid overflow */
short quantum = QUANTUM;
#endif SCHED

#ifdef  SMALL
#define SQSIZE 010      /* Must be power of 2 */
#else
#define SQSIZE 0100     /* Must be power of 2 */
#endif

#define HASH(x) (( (int) x >> 5) & (SQSIZE-1))
struct  proc    *slpque[SQSIZE];

/*
 * Give up the processor till a wakeup occurs
 * on chan, at which time the process
 * enters the scheduling queue at priority pri.
 * The most important effect of pri is that when
 * pri<=PZERO a signal cannot disturb the sleep;
 * if pri>PZERO signals will be processed.
 * Callers of this routine must be prepared for
 * premature return, and check that the reason for
 * sleeping has gone away.
 */

/* ������� �� sleep:
 *                 sleep
 *   pri<= PZERO          pri > PZERO
 *  �� ����������� ��    ����������� ��
 *  ��������             ��������
 *  wakeup           wakeup       signal
 *   return 0        return 0       |
 *                             PCATCH   noPCATCH
 *                             return 1    resume
 */
sleep(chan, pri)
caddr_t chan;
{
	register struct proc *rp;
	register struct proc **hp;
	register s;
	register retintr;       /* �� 0, ���� ���� ���� PCATCH */

	rp = u.u_procp;
	retintr = pri & PCATCH ;   pri &= PMASK;
	s = spl6();
#ifdef SCHED
	if (chan==0)
#else SCHED
	if (chan==0 || rp->p_stat != SRUN)
#endif SCHED
		panic("sleep");
	rp->p_wchan = chan;
#ifdef  UCB_METER
	rp->p_slptime = 0;
#endif
#ifndef SCHED
	rp->p_pri = pri;
#endif  SCHED
	hp = &slpque[HASH(chan)];
	rp->p_link = *hp;
	*hp = rp;
	if(pri > PZERO) {
#ifdef SCHED
		rp->p_stat = SWAIT;
#endif SCHED
		if(ISSIG(rp)) {
			if (rp->p_wchan)
				unsleep(rp);
			rp->p_stat = SRUN;
			(void) _spl0();
			goto psig;
		}
		if (rp->p_wchan == 0)
			goto out;
#ifndef  SCHED
		rp->p_stat = SSLEEP;
#endif  SCHED
		(void) _spl0();
		if(runin != 0) {
			runin = 0;
			wakeup((caddr_t)&runin);
		}
		swtch();
		if(ISSIG(rp))
			goto psig;
	} else {
		rp->p_stat = SSLEEP;
		(void) _spl0();
		swtch();
	}
out:
	splx(s);
	return 0;       /* normal (wakeuped) */

	/*
	 * If priority was low (>PZERO) and
	 * there has been a signal,
	 * execute non-local goto to
	 * the qsav location.
	 * (see trap.c)
	 */
psig:
	if( retintr ){
		splx(s);
		return 1;       /* signalled */
	}
	/* else restart */
	resume(u.u_procp->p_addr, u.u_qsav);
	/*NOTREACHED*/
}


/*
 * Remove a process from its wait queue
 */
unsleep(p)
register struct proc *p;
{
	register struct proc **hp;
	register s;

	s = spl6();
	if (p->p_wchan) {
		hp = &slpque[HASH(p->p_wchan)];
		while (*hp != p)
			hp = &(*hp)->p_link;
		*hp = p->p_link;
		p->p_wchan = 0;
	}
	splx(s);
}


/*
 * Wake up all processes sleeping on chan.
 */

wakeup(chan)
register caddr_t chan;
{
	register struct proc *p, **q;
	struct proc **h;
	int i, s;
#ifndef NOKA5
	mapinfo map;

	/*
	 * Since we are called at interrupt time, must insure normal
	 * kernel mapping to access proc.
	 */
	savemap(map);
#endif
	s = spl6();
	h = &slpque[HASH(chan)];
restart:
	for (q = h; p = *q; ) {
		if (p->p_stat != SSLEEP && p->p_stat != SSTOP
#ifdef  SCHED
	     && p->p_stat != SWAIT  && p->p_stat != SRUN
#endif  SCHED
	       )
			panic("wakeup");
		if (p->p_wchan==chan) {
			p->p_wchan = 0;
			*q = p->p_link;
#ifdef  UCB_METER
			p->p_slptime = 0;
#endif
			if (p->p_stat == SSLEEP
#ifdef  SCHED
			 || p->p_stat == SWAIT
#endif  SCHED
			 ) {
				setrun(p);
				goto restart;
			}
		} else
			q = &p->p_link;
	}
	splx(s);
#ifndef NOKA5
	restormap(map);
#endif
}


#if defined(DIAGNOSTIC) || defined(SCHED)
#  define SETRQ(p) setrq(p)
#else
#  define SETRQ(p) ((p->p_link = runq) , (runq=p))
#endif
#ifdef SCHED
/*
 * SETRQ -- called only from within this source file, SETRQ inserts
 * a specified process into the "ready" process queue used by the
 * CPU dispatcher "swtch".  Sorting into the ready list is based on
 * the priority level p_level of the process.  The flag argument
 * determines whether the process is put at the head or the tail
 * of the processes at its level (head if nonzero).
 */

setrq(p,flag)
register struct proc *p;
int flag;       /* if nonzero, insert at head */
{
	register struct proc *q, *ip;   /* ip is "insertion point" */
	int s;

	s = spl6();

	ip = NULL;
	p->p_stat =SRUN;
	/* process p will be inserted before the first process q which */
	/* satisfies the necessary condition (q worse prio than p)     */
/*      for(q = runq; q != NULL; q = q->p_link){
*/      q = runq;
	do {
		if (q == NULL)
			break;
		if(q->p_level < p->p_level ||(flag && q->p_level == p->p_level))
			break;
		ip = q;
		q = q->p_link;
	} while (q != runq);

	if(ip == NULL){ /* insert at start of list */
		p->p_link = runq;
		runq = p;
	}
	else { /* general insertion case */
		p->p_link = ip->p_link; /* save rest of list */
		ip->p_link = p;
	}

	if(p->p_level > curlevel)
		runrun++;       /* this line implements preemptive scheduling */
	splx(s);
}
#else SCHED

/*
 * When you are sure that it
 * is impossible to get the
 * 'proc on q' diagnostic, the
 * diagnostic loop can be removed.
 */
setrq(p)
struct proc *p;
{
	register struct proc *q;
	register s;

	s = spl6();
#ifdef  DIAGNOSTIC
	for(q=runq; q!=NULL; q=q->p_link)
		if(q == p) {
			printf("proc on q\n");
			goto out;
		}
#endif
	p->p_link = runq;
	runq = p;
out:
	splx(s);
}
#endif SCHED

/*
 * Remove runnable job from run queue.
 * This is done when a runnable job is swapped
 * out so that it won't be selected in swtch().
 * It will be reinserted in the runq with setrq
 * when it is swapped back in.
 */
remrq(p)
	register struct proc *p;
{
	register struct proc *q;
	int s;

	s = spl6();
	if (p == runq)
		runq = p->p_link;
	else {
		for (q = runq; q; q = q->p_link)
			if (q->p_link == p) {
				q->p_link = p->p_link;
				goto done;
			}
		panic("remque");
done:
		;
	}
	splx(s);
}

/*
 * Set the process running;
 * arrange for it to be swapped in if necessary.
 */
setrun(p)
register struct proc *p;
{
	register s;

	s = spl6();
	switch (p->p_stat) {

#ifdef SCHED
	case SWAIT:
#endif SCHED
	case SSTOP:
	case SSLEEP:
		unsleep(p);             /* e.g. when sending signals */
		break;

	case SIDL:
		break;

	default:
		panic("setrun");
	}
	p->p_stat = SRUN;
#ifdef SCHED
	setlev(p,1);
	setrq(p,0);
#else
	if (p->p_flag & SLOAD)
		SETRQ(p);
#endif SCHED
	splx(s);

#ifdef  CGL_RTP
	if (p == rtpp) {
		wantrtp++;
		runrun++;
	}
	else
#endif
#ifndef SCHED
		if (p->p_pri < curpri)
			runrun++;
#endif SCHED
	if(runout != 0 && (p->p_flag&SLOAD) == 0) {
		runout = 0;
		wakeup((caddr_t)&runout);
	}
}

#ifdef SCHED
setlev(p,flag)
register struct proc *p;
int flag;
{
	register i;
	/* adjust level if necessary */
	if((i = p->p_baslev) < RTLEVEL){
		if(p->p_level > i)
			p->p_level--;
		if(p->p_sflag & STTYIN)
			i += TTIINC;
		else if(p->p_sflag & STTYOUT)
			i += TTOINC;
		else if (flag)
			i += OTHINC;
		if(i >= RTLEVEL)
			i = RTLEVEL - 1;
		if(i > p->p_level)
			p->p_level = i;
	}
	p->p_quant = 0;
	p->p_sflag &= ~ (STTYIN | STTYOUT);
}
#else

/*
 * Set user priority.
 * The rescheduling flag (runrun)
 * is set if the priority is better
 * than the currently running process.
 */
setpri(pp)
register struct proc *pp;
{
	register p;

	p = ((pp->p_cpu & 0377) / 16) + PUSER + pp->p_nice - NZERO;
	if(p > 127)
		p = 127;
	if(p < curpri)
		runrun++;
	pp->p_pri = p;
	return(p);
}
#endif SCHED
/*
 * The main loop of the scheduling (swapping)
 * process.
 * The basic idea is:
 *  see if anyone wants to be swapped in;
 *  swap out processes until there is room;
 *  swap him in;
 *  repeat.
 * The runout flag is set whenever someone is swapped out.
 * Sched sleeps on it awaiting work.
 *
 * Sched sleeps on runin whenever it cannot find enough
 * core (by swapping out or otherwise) to fit the
 * selected swapped process.  It is awakened when the
 * core situation changes and in any case once per second.
 */
#ifdef SCHED
sched()
{
	register struct proc *p, *rp, *q;
	int maxsize, desperate;

	for(;;){        /* keep loading processes !!! */
		/*
		 * Find process to swap in; the eligible process is the first
		 * one in the "ready" list which is not loaded at present.
		 */
		spl6();
		for (p = runq; p != NULL; p = p->p_link){
			if((p->p_stat == SRUN) && (p->p_flag & SLOAD) == 0)
				break;
		}
		if (p == NULL){
			runout++;
			sleep((caddr_t)&runout, PSWP);
			continue;
		}
		spl0();

		/* if there is enough free core, swap in the process */

		if (swapin(p)){
			continue;
		}

		/* find core by out-swapping largest eligible process */
		spl6();
		q = NULL;
		for(desperate = 0; q == NULL && desperate <= 1; desperate++){
			maxsize = -1;
			for (rp = &proc[0]; rp < maxproc; rp++) {
				if(swapok(p,rp,desperate)){
#ifdef  VIRUS_VFORK
				   if (maxsize < rp->p_dsize + rp->p_ssize) {
					q = rp;
					maxsize = rp->p_dsize + rp->p_ssize;
				   }
#else
				    if (maxsize < rp->p_size) {
					q = rp;
					maxsize = rp->p_size;
				    }
#endif
				}
			}
		}
		if(q != NULL){
			spl0();
			q->p_flag &= ~SLOAD;
#ifdef  VIRUS_VFORK
			(void) xswap(q, X_FREECORE, X_OLDSIZE, X_OLDSIZE);
#else
			(void) xswap(q, X_FREECORE, X_OLDSIZE);
#endif
		}
		else {
			runin++;
			sleep((caddr_t)&runin, PSWP);
		}
	}
}
#else
#define PRISIZE 128
sched()
{
	register struct proc *rp, *p;
	register outage, inage;
	size_t maxsize;
	size_t nmaxsize;
	/*
	 * find user to swap in;
	 * of users ready, select one out longest
	 */

loop:
	(void) _spl0();
/*      (void) _spl6();  ������ �� ��������, ��� ��������� �����
			 ��� ��������� ���������� ����������� �������
			 � ������ ������ ��� �� ��� */
	outage = -20000;
	for (rp = &proc[0]; rp <= maxproc; rp++)
#ifdef  VIRUS_VFORK
	/* always bring in parents ending a vfork, to avoid deadlock */
	if (rp->p_stat==SRUN && (rp->p_flag&SLOAD)==0 &&
	    ((rp->p_time - (rp->p_nice-NZERO)*8 > outage)
	    || (rp->p_flag & SVFPARENT)))
#else
	if (rp->p_stat==SRUN && (rp->p_flag&SLOAD)==0 &&
	    rp->p_time - (rp->p_nice-NZERO)*8 > outage)
#endif
		{
		p = rp;
		outage = rp->p_time - (rp->p_nice-NZERO)*8;
#ifdef  VIRUS_VFORK
		if (rp->p_flag & SVFPARENT)
			break;
#endif
	}
	/*
	 * If there is no one there, wait.
	 */
	if (outage == -20000) {
		_spl6();
		runout++;
		sleep((caddr_t)&runout, PSWP);
		(void) _spl0();
		goto loop;
	}

	/*
	 * See if there is core for that process;
	 * if so, swap it in.
	 */

	if (swapin(p))
		goto loop;

	/*
	 * none found.
	 * look around for core.
	 * Select the largest of those sleeping
	 * at bad priority; if none, select the oldest.
	 */
/*        (void) _spl6();       ��. ����. ���� */
	p = NULL;
	maxsize = 0;
	inage = -1;
	for (rp = &proc[1]; rp <= maxproc; rp++) {
		if (rp->p_stat==SZOMB
		 || (rp->p_flag&(SSYS|SLOCK|SULOCK|SLOAD))!=SLOAD)
			continue;
		if (rp->p_textp && rp->p_textp->x_flag&XLOCK)
			continue;
		if (rp->p_stat==SSLEEP&&rp->p_pri>=PZERO || rp->p_stat==SSTOP) {
#ifdef  VIRUS_VFORK
			if (maxsize <
			   (nmaxsize = rp->p_dsize + rp->p_ssize +
			    ((rp->p_textp && rp->p_textp->x_count == 1)?
				rp->p_textp->x_size:0)+
				rp->p_time*PRISIZE)) {
				p = rp;
				maxsize = nmaxsize;
			}
#else
			if (maxsize < rp->p_size) {
				p = rp;
				maxsize = rp->p_size;
			}
#endif
		} else if (maxsize==0 && (rp->p_stat==SRUN||rp->p_stat==SSLEEP)
#ifdef  CGL_RTP
		    /*
		     * Can't swap processes preempted in copy/clear.
		     */
		    && (rp->p_pri > PRTP + 1)
#endif
		    ) {
			if (rp->p_time+rp->p_nice-NZERO > inage) {
				p = rp;
				inage = rp->p_time+rp->p_nice-NZERO;
			}
		}
	}
	/* _spl0(); */
	/*
	 * Swap found user out if sleeping at bad pri,
	 * or if he has spent at least 2 seconds in core and
	 * the swapped-out process has spent at least 3 seconds out.
	 * Otherwise wait a bit and try again.
	 */
	if (maxsize>0 || (outage>=3 && inage>=2)) {
		(void) _spl6();
		p->p_flag &= ~SLOAD;
		if(p->p_stat == SRUN)
			remrq(p);
		(void) _spl0();
#ifdef  VIRUS_VFORK
		(void) xswap(p, X_FREECORE, X_OLDSIZE, X_OLDSIZE);
#else
		(void) xswap(p, X_FREECORE, X_OLDSIZE);
#endif
		goto loop;
	}
	(void) _spl6();
	runin++;
	sleep((caddr_t)&runin, PSWP);
	(void) _spl0();
	goto loop;
}
#endif SCHED
#ifdef SCHED
/*
 * SWAPOK is a predicate (truth-valued function) used to determine the
 * desirability of swapping process 'loser' in order to bring in process
 * 'winner'.  The 'desperate' flag captures the distinction between
 * 'easy core' and 'hard core'.
 */

int
swapok(winner,loser,desperate)
register struct proc *winner, *loser;
int desperate;
{
	register struct proc *p;

	if ((loser->p_flag & (SSYS|SLOCK|SULOCK|SLOAD)) != SLOAD)
		return(0);
	/* don't swap out a process which is at "real-time" priority level */
	if(loser->p_level >= RTLEVEL)
		return(0);
	/* avoid deadlocks -- don't swap out process with locked text */
	if(loser->p_textp && (loser->p_textp->x_flag & XLOCK))
		return(0);
	switch(loser->p_stat){
	case SSTOP:
	case SWAIT:
		break;
	case SRUN:
		if(!desperate)
			return(0);
		if(loser->p_sflag & SEXEC){
			return(0);      /* avoid swapping an EXECing proc */
		}
		/* make sure that winner is higher priority than loser */
		if(winner->p_level < loser->p_level)
			return(0);
		if(winner->p_level == loser->p_level){
			p = winner;
			while(p && p != loser && p->p_level == winner->p_level)
				p = p->p_link;
			if(p != loser)
				return(0);
		}
		break;
	case SSLEEP:
		if(!desperate)
			return(0);
		if(loser->p_sflag & SEXEC){
			return(0);      /* avoid swapping an EXECing proc */
		}
		break;
	default:
		return(0);
	}
	/* if loser has been loaded less than time-to-swap, or */
	/* loser has never been scheduled to run since being loaded */
	return(1);
}
#endif SCHED
/*
 * Swap a process in.
 * Allocate data and possible text separately.
 * It would be better to do largest first.
 */
#ifdef  VIRUS_VFORK
/*
 * Text, data, stack and u. are allocated in that order,
 * as that is likely to be in order of size.
 */
#endif
swapin(p)
register struct proc *p;
{
#ifdef  VIRUS_VFORK
	register struct text *xp;
	memaddr a[3];
	register memaddr x = NULL;
	smeter(p);
	/*
	 *  Malloc the text segment first, as it tends to be largest.
	 */
	if (xp = p->p_textp) {
		xlock(xp);
		xmeter(xp);
		if (xp->x_caddr == 0) {
			if ((x = malloc(coremap, xp->x_size)) == NULL) {
				xunlock(xp);
				return(0);
			}
		}
	}
	if (malloc3(coremap, p->p_dsize, p->p_ssize, USIZE, a) == NULL) {
		if (x)
			mfree(coremap, xp->x_size, x);
		if (xp)
			xunlock(xp);
		return(0);
	}
	if (x) {
		xp->x_caddr = x;
		if ((xp->x_flag & XLOAD) == 0)
			swap(xp->x_daddr, x, xp->x_size, B_READ);
	}
	if (xp) {
		xp->x_ccount++;
		xunlock(xp);
	}
	if (p->p_dsize) {
		swap(p->p_daddr, a[0], p->p_dsize, B_READ);
		mfree(swapmap, ctod(p->p_dsize), p->p_daddr);
	}
	if (p->p_ssize) {
		swap(p->p_saddr, a[1], p->p_ssize, B_READ);
		mfree(swapmap, ctod(p->p_ssize), p->p_saddr);
	}
	swap(p->p_addr, a[2], USIZE, B_READ);
	mfree(swapmap, ctod(USIZE), p->p_addr);
	p->p_daddr = a[0];
	p->p_saddr = a[1];
	p->p_addr = a[2];

#else   VIRUS_VFORK
	register struct text *xp;
	register int a;
	register unsigned x = 0;

	if ((a = malloc(coremap, p->p_size)) == NULL)
		return(0);
	if (xp = p->p_textp) {
		xlock(xp);
		xmeter(xp);
		if (xp->x_caddr == 0) {
			if ((x = malloc(coremap, xp->x_size)) == NULL)
				{
				xunlock(xp);
				mfree(coremap, p->p_size, a);
				return(0);
			}
			xp->x_caddr = x;
			if ((xp->x_flag & XLOAD)==0)
				swap(xp->x_daddr, x, xp->x_size, B_READ);
		}
		xp->x_ccount++;
		xunlock(xp);
	}
	swap(p->p_addr, a, p->p_size, B_READ);
	mfree(swapmap, ctod(p->p_size), p->p_addr);
	p->p_addr = a;
#endif  VIRUS_VFORK

#ifndef SCHED
	if (p->p_stat == SRUN)
		setrq(p);
#endif SCHED
	p->p_flag |= SLOAD;
#ifdef SCHED
	p->p_quant = 0;
#else  SCHED
	p->p_time = 0;
#endif SCHED
	return(1);
}

/*
 * put the current process on
 * the queue of running processes and
 * call the scheduler.
 */
qswtch()
{
#ifdef SCHED
	register struct proc *p;
	register int f = 0;     /* setrq flag */
	register int i;

	p = u.u_procp;

	if(quantum <= 0)       /* end-of-quantum processing */
		setlev(p,0);
	else {                  /* process preempted by higher level process */
		p->p_quant = quantum;
		f++;    /* insert process at head of level subqueue */
	}
	setrq(p,f);
#else
	setrq(u.u_procp);
#endif SCHED
	swtch();
}

/*
 * This routine is called to reschedule the CPU.
 * if the calling process is not in the RUN state,
 * arrangements for it to restart must have
 * been made elsewhere, usually by calling via sleep.
 * There is a race here. A process may become
 * ready after it has been examined.
 * In this case, idle() will be called and
 * will return in at most 1hz time.
 * i.e. it's not worth putting an spl() in.
 */
swtch()
{
	register n;
	register struct proc *p, *q;
	struct proc *pp, *pq;

#if defined(DIAGNOSTIC) && !defined(NOKA5)
	extern struct buf *hasmap;
	if(hasmap != (struct buf *)0)
		panic("swtch hasmap");
#endif
	/*
	 * If not the idle process, resume the idle process.
	 */
	if (u.u_procp != &proc[0]) {
		if (save(u.u_rsav)) {
			sureg();
			return;
		}
#ifndef NONFP
		if (u.u_fpsaved==0) {
			savfp(&u.u_fps);
			u.u_fpsaved = 1;
		}
#endif
		resume(proc[0].p_addr, u.u_qsav);
	}
	/*
	 * The first save returns nonzero when proc 0 is resumed
	 * by another process (above); then the second is not done
	 * and the process-search loop is entered.
	 *
	 * The first save returns 0 when swtch is called in proc 0
	 * from sched().  The second save returns 0 immediately, so
	 * in this case too the process-search loop is entered.
	 * Thus when proc 0 is awakened by being made runnable, it will
	 * find itself and resume itself at rsav, and return to sched().
	 */
	if (save(u.u_qsav)==0 && save(u.u_rsav))
		return;
#ifdef  UCB_METER
	cnt.v_swtch++;
#endif
loop:
	(void) _spl6();
	runrun = 0;
#ifdef  CGL_RTP
	/*
	 * Test for the presence of a "real time process".
	 * If there is one and it is runnable, give it top
	 * priority.
	 */
	if ((p=rtpp) && p->p_stat==SRUN && (p->p_flag&SLOAD)) {
		pq = NULL;
		for (q=runq; q!=NULL; q=q->p_link) {
			if (q == p)
				break;
			pq = q;
		}
		if (q == NULL)
			panic("rtp not found\n");       /* "cannot happen" */
		n = PRTP;
		wantrtp = 0;
		goto runem;
	}
#endif
	q = NULL;
#ifdef SCHED
	for(p = runq; p != NULL && (p->p_flag & SLOAD) == 0; p = p->p_link){
		q = p;  /* update predecessor */
	}
#else
	pp = NULL;
	n = 128;
	/*
	 * Search for highest-priority runnable process
	 */
	for(p=runq; p!=NULL; p=p->p_link) {
		if((p->p_stat==SRUN) && (p->p_flag&SLOAD)) {
			if(p->p_pri < n) {
				pp = p;
				pq = q;
				n = p->p_pri;
			}
		}
		q = p;
	}
	/*
	 * If no process is runnable, idle.
	 */
	p = pp;
#endif SCHED
	if(p == NULL) {
#ifdef  UCB_FRCSWAP
		idleflg++;
#endif
		idle();
		goto loop;
	}
#ifdef SCHED
/*      else
		if(p->p_stat != SRUN)
			panic("not ready");
*/
#else  SCHED
#ifdef  CGL_RTP
runem:
#endif
	q = pq;
#endif SCHED
	if(q == NULL)
		runq = p->p_link;
	else
		q->p_link = p->p_link;
#ifdef SCHED
	p->p_link = NULL;
	n = p->p_level;
	curlevel = n;
	if(p->p_quant > 0)      /* last quantum partially used */
		quantum = p->p_quant;
	else {
		if(n >= RTLEVEL)
			quantum = QUANTUM;
		else
			quantum = QUANTUM * (RTLEVEL - n);
	}

#else
	curpri = n;
#endif SCHED
	(void) _spl0();
	/*
	 * The rsav (ssav) contents are interpreted in the new address space
	 */
	n = p->p_flag&SSWAP;
	p->p_flag &= ~SSWAP;
	resume(p->p_addr, n? u.u_ssav: u.u_rsav);
}

/*
 * Create a new process-- the internal version of
 * sys fork.
 * It returns 1 in the new process, 0 in the old.
 */
#ifdef  VIRUS_VFORK
newproc(isvfork)
#else
newproc()
#endif
{
	int a1, a2;
	register struct proc *rpp, *rip;
	register n;
#ifdef  VIRUS_VFORK
	unsigned a[3];
#endif

	rpp = NULL;
	/*
	 * First, just locate a slot for a process
	 * and copy the useful info from this process into it.
	 * The panic "cannot happen" because fork has already
	 * checked for the existence of a slot.
	 */
retry:
	mpid++;
	if(mpid >= 30000)
		mpid = 1;
	for(rip = proc; rip < procNPROC; rip++) {
		if(rip->p_stat == NULL && rpp==NULL)
			rpp = rip;
		if (rip->p_pid==mpid || rip->p_pgrp==mpid)
			goto retry;
	}
	if (rpp == NULL)
		panic("no procs");

	/*
	 * make proc entry for new proc
	 */

	rip = u.u_procp;
	rpp->p_clktim = 0;
	rpp->p_stat = SIDL;
	rpp->p_flag = SLOAD | (rip->p_flag & (SDETACH|SNUSIG));
	rpp->p_pptr = rip;
	rpp->p_siga0 = rip->p_siga0;
	rpp->p_siga1 = rip->p_siga1;
	rpp->p_cursig = 0;
	rpp->p_wchan = 0;
	rpp->p_uid = rip->p_uid;
	rpp->p_pgrp = rip->p_pgrp;
#ifdef SCHED
	rpp->p_quant = 0;
	rpp->p_sflag = 0;
	rpp->p_baslev = rip->p_baslev;
	rpp->p_level = (rpp->p_baslev>=RTLEVEL)?rpp->p_baslev:(RTLEVEL-1);
	rpp->p_link = NULL;
#else
	rpp->p_nice = rip->p_nice;
	rpp->p_time = 0;
	rpp->p_cpu = 0;
#endif SCHED
	rpp->p_textp = rip->p_textp;
	rpp->p_pid = mpid;
	rpp->p_ppid = rip->p_pid;
#ifdef  UCB_METER
	rpp->p_slptime = 0;
#endif
	if (rpp > maxproc)
		maxproc = rpp;

	/*
	 * make duplicate entries
	 * where needed
	 */

	for(n=0; n<NOFILE; n++)
		if(u.u_ofile[n] != NULL)
			u.u_ofile[n]->f_count++;
#ifdef  VIRUS_VFORK
	if ((rip->p_textp != NULL) && !isvfork)
#else
	if(rip->p_textp != NULL)
#endif
	{
		rip->p_textp->x_count++;
		rip->p_textp->x_ccount++;
	}
	u.u_cdir->i_count++;
	if (u.u_rdir)
		u.u_rdir->i_count++;
	/*
	 * When the resume is executed for the new process,
	 * here's where it will resume.
	 */
	if (save(u.u_ssav)) {
		sureg();
		return(1);
	}
	/*
	 * Partially simulate the environment
	 * of the new process so that when it is actually
	 * created (by copying) it will look right.
	 */
	u.u_procp = rpp;

#ifdef  VIRUS_VFORK
	rpp->p_dsize = rip->p_dsize;
	rpp->p_ssize = rip->p_ssize;
	rpp->p_daddr = rip->p_daddr;
	rpp->p_saddr = rip->p_saddr;
	a1 = rip->p_addr;
	if (isvfork)
		a[2] = malloc(coremap,USIZE);
	else {
		/*
		 * malloc3() will set a[2] to NULL on failure.
		 */
#ifdef  UCB_FRCSWAP
		a[2] = NULL;
		if (idleflg)
#endif
			(void) malloc3(coremap,rip->p_dsize,rip->p_ssize,USIZE,a);
	}
	/*
	 * If there is not enough core for the
	 * new process, swap out the current process to generate the
	 * copy.
	 */
	if(a[2] == NULL) {
		rip->p_stat = SIDL;
		rpp->p_addr = a1;
		rpp->p_stat = SRUN;
		(void) xswap(rpp, X_DONTFREE, X_OLDSIZE, X_OLDSIZE);
		rip->p_stat = SRUN;
		u.u_procp = rip;
	} else {
		/*
		 * There is core, so just copy.
		 */
		rpp->p_addr = a[2];
#ifdef  CGL_RTP
		/*
		 * Copy is now a preemptable kernel process.
		 * The u. area is non-reentrant so copy it first
		 * in non-preemptable mode.
		 */
		copyu(rpp->p_addr);
#else
		copy(a1, rpp->p_addr, USIZE);
#endif
		u.u_procp = rip;
		if (isvfork == 0) {
			rpp->p_daddr = a[0];
			copy(rip->p_daddr, rpp->p_daddr, rpp->p_dsize);
			rpp->p_saddr = a[1];
			copy(rip->p_saddr, rpp->p_saddr, rpp->p_ssize);
		}
		(void) _spl6();
		rpp->p_stat = SRUN;
#ifndef SCHED
		SETRQ(rpp);
#endif SCHED
		(void) _spl0();
	}
#ifdef SCHED
	setrq(rpp, 0);
#endif SCHED
	rpp->p_flag |= SSWAP;
	if (isvfork) {
		/*
		 *  Set the parent's sizes to 0, since the child now
		 *  has the data and stack.
		 *  (If we had to swap, just free parent resources.)
		 *  Then wait for the child to finish with it.
		 */
		if (a[2] == NULL) {
			mfree(coremap,rip->p_dsize,rip->p_daddr);
			mfree(coremap,rip->p_ssize,rip->p_saddr);
		}
		rip->p_dsize = 0;
		rip->p_ssize = 0;
		rip->p_textp = NULL;
		rpp->p_flag |= SVFORK;
		rip->p_flag |= SVFPARENT;
		while (rpp->p_flag & SVFORK)
			sleep((caddr_t)rpp,PSWP+1);
		if ((rpp->p_flag & SLOAD) == 0)
			panic("newproc vfork");
		u.u_dsize = rip->p_dsize = rpp->p_dsize;
		rip->p_daddr = rpp->p_daddr;
		rpp->p_dsize = 0;
		u.u_ssize = rip->p_ssize = rpp->p_ssize;
		rip->p_saddr = rpp->p_saddr;
		rpp->p_ssize = 0;
		rip->p_textp = rpp->p_textp;
		rpp->p_textp = NULL;
		rpp->p_flag |= SVFDONE;
		wakeup((caddr_t)rip);
		/* must do estabur if dsize/ssize are different */
		estabur(u.u_tsize,u.u_dsize,u.u_ssize,u.u_sep,RO);
		rip->p_flag &= ~SVFPARENT;
	}
	return(0);

#else   VIRUS_VFORK
	rpp->p_size = n = rip->p_size;
	a1 = rip->p_addr;
#ifndef UCB_FRCSWAP
	a2 = malloc(coremap, n);
#else
	if(idleflg)
		a2 = malloc(coremap, n);
	else
		a2 = NULL;
#endif
	/*
	 * If there is not enough core for the
	 * new process, swap out the current process to generate the
	 * copy.
	 */
	if(a2 == NULL) {
		rip->p_stat = SIDL;
		rpp->p_addr = a1;
		rpp->p_stat = SRUN;
		(void) xswap(rpp, X_DONTFREE, X_OLDSIZE);
		rip->p_stat = SRUN;
#ifdef  CGL_RTP
		u.u_procp = rip; /* see comments below */
#endif
	} else {
		/*
		 * There is core, so just copy.
		 */
		rpp->p_addr = a2;
#ifdef  CGL_RTP
		/*
		 * Copy is now a preemptable kernel process.
		 * The u. area is non-reentrant so copy it first
		 * in non-preemptable mode.
		 */
		copyu(a2);
		/*
		 * If we are to be interrupted we must insure consistency;
		 * restore current process state now.
		 */
		u.u_procp = rip;
		copy(a1+USIZE, a2+USIZE, n-USIZE);
#else
		copy(a1, a2, n);
#endif
		(void) _spl6();
		rpp->p_stat = SRUN;
#ifdef SCHED
		setrq(rpp, 1);
#else
		SETRQ(rpp);
#endif SCHED
		(void) _spl0();
	}
#ifndef CGL_RTP
	u.u_procp = rip;
#endif
	rpp->p_flag |= SSWAP;
	return(0);
#endif  VIRUS_VFORK
}

#ifdef  VIRUS_VFORK
/*
 * Notify parent that vfork child is finished with parent's data.
 * Called during exit/exec(getxfile); must be called before xfree().
 * The child must be locked in core
 * so it will be in core when the parent runs.
 */
endvfork()
{
	register struct proc *rip, *rpp;

	rpp = u.u_procp;
	rip = rpp->p_pptr;
	rpp->p_flag &= ~SVFORK;
	rpp->p_flag |= SLOCK;
	wakeup((caddr_t)rpp);
	while(!(rpp->p_flag&SVFDONE))
		sleep((caddr_t)rip,PZERO-1);
	/*
	 * The parent has taken back our data+stack, set our sizes to 0.
	 */
	u.u_dsize = rpp->p_dsize = 0;
	u.u_ssize = rpp->p_ssize = 0;
	rpp->p_flag &= ~(SVFDONE | SLOCK);
}
#endif
/*
 * Change the size of the data+stack regions of the process.
 * If the size is shrinking, it's easy-- just release the extra core.
 * If it's growing, and there is core, just allocate it
 * and copy the image, taking care to reset registers to account
 * for the fact that the system's stack has moved.
 * If there is no core, arrange for the process to be swapped
 * out after adjusting the size requirement-- when it comes
 * in, enough core will be allocated.
 *
 * After the expansion, the caller will take care of copying
 * the user's stack towards or away from the data area.
 */
#ifdef  VIRUS_VFORK
/*
 * The data and stack segments are separated from each other.  The second
 * argument to expand specifies which to change.  The stack segment will
 * not have to be copied again after expansion.
 */
expand(newsize,segment)
#else
expand(newsize)
#endif
{
	register i,
		 n;     /* ������ ������ �������� */
	register struct proc *p;
	register a1,    /* ������ ����� �������� */
		 a2;    /* ����� (� ������ ���������) */

#ifdef  VIRUS_VFORK
	p = u.u_procp;
	if (segment == S_DATA) {
		n = p->p_dsize;
		p->p_dsize = newsize;
		a1 = p->p_daddr;
		/* ������� ����������� */
		if(n >= newsize) {
			n -= newsize;
			mfree(coremap, n, a1+newsize);
			return;
		}
	} else {
		n = p->p_ssize;
		p->p_ssize = newsize;
		a1 = p->p_saddr;
		if(n >= newsize) {
			n -= newsize;
			p->p_saddr += n;
			mfree(coremap, n, a1);
			/*
			 *  Since the base of stack is different,
			 *  segmentation registers must be repointed.
			 */
			sureg();
			return;
		}
	}

	/* ������� ������ */
	if (save(u.u_ssav)) {
		/*
		 * If we had to swap, the stack needs moving up.
		 */
		if (segment == S_STACK) {
			a1 = p->p_saddr;
			i = newsize - n;
			a2 = a1 + i;
			/*
			 * i is the amount of growth.  Copy i clicks
			 * at a time, from the top; do the remainder
			 * (n % i) separately.
			 */
			while (n >= i) {
				n -= i;
				copy(a1+n, a2+n, i);
			}
			copy(a1, a2, n);
		}
		sureg();
		return;
	}
#ifndef NONFP
	if (u.u_fpsaved==0) {
		savfp(&u.u_fps);
		u.u_fpsaved = 1;
	}
#endif
#ifdef  UCB_FRCSWAP
	/*
	 * Stack must be copied either way, might as well not swap.
	 */
	if(idleflg || (segment==S_STACK))
		a2 = malloc(coremap, newsize);
	else
		a2 = NULL;
#else
#ifdef IPK_MEXPAND
	if( segment == S_DATA ){
		a2 = mexpand( coremap, a1, n, newsize );
		if( a2 == a1 ){
			/* ������� ������ ����������       */
			/* ������ ������������ ��� �� ���� */
			sureg();
			return;
		}
		/* ����� mexpand �������� ������ ��� malloc */
	}else
#endif IPK_MEXPAND
	a2 = malloc(coremap, newsize);
#endif
	if(a2 == NULL) {
		if (segment == S_DATA)
			(void) xswap(p, X_FREECORE, n, X_OLDSIZE);
		else
			(void) xswap(p, X_FREECORE, X_OLDSIZE, n);
		p->p_flag |= SSWAP;
		swtch();
		/* NOTREACHED */
	}
	if (segment == S_STACK) {
		p->p_saddr = a2;
		/*
		 * Make the copy put the stack at the top of the new area.
		 */
		a2 += newsize - n;
	} else
		p->p_daddr = a2;
	copy(a1, a2, n);
	mfree(coremap, n, a1);
	sureg();
	return;

#else   VIRUS_VFORK
	p = u.u_procp;
	n = p->p_size;
	p->p_size = newsize;
	a1 = p->p_addr;
	if(n >= newsize) {
		mfree(coremap, n-newsize, a1+newsize);
		return;
	}
	if (save(u.u_ssav)) {
		sureg();
		return;
	}
#ifndef NONFP
	if (u.u_fpsaved==0) {
		savfp(&u.u_fps);
		u.u_fpsaved = 1;
	}
#endif
#ifdef  UCB_FRCSWAP
	if(idleflg)
		a2 = malloc(coremap, newsize);
	else
		a2 = NULL;
#else
	a2 = malloc(coremap, newsize);
#endif
	if(a2 == NULL) {
		(void) xswap(p, X_FREECORE, n);
		p->p_flag |= SSWAP;
		swtch();
		/*NOTREACHED*/
	}
#ifdef  CGL_RTP
	copyu(a2);      /* see comments in newproc() */
	copy(a1+USIZE, a2+USIZE, n-USIZE);
	p->p_addr = a2;
#else
	p->p_addr = a2;
	copy(a1, a2, n);
#endif
	mfree(coremap, n, a1);
	resume(a2, u.u_ssav);
#endif  VIRUS_VFORK
}
#ifdef  CGL_RTP
/*
 * Test status of the "real time process";
 * preempt the current process if runnable.
 * Use caution when calling this routine, much
 * of the kernel is non-reentrant!
 */
runrtp()
{
	register struct proc *p;

	if ((p=rtpp)==NULL || p==u.u_procp)
		return;
	if (p->p_stat==SRUN && (p->p_flag&SLOAD)!=0) {
		u.u_procp->p_pri = PRTP+1;
		qswtch();
	}
}
#endif

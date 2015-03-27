/*
 * $Log:	sys4.c,v $
 * Revision 22.3  90/11/12  19:20:44  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.2  89/06/30  16:01:51  abs
 * chown олжен сносить ISUID и ISGID биты доступа.
 * 
 * Revision 22.1  89/04/12  15:39:10  korotaev
 * "param.h" ==> <sys/param.h>
 *
 * Revision 22.0  89/03/30  16:49:17  korotaev
 * Begining DEMOS 2.2.
 *
 * Revision 2.5  89/03/02  19:00:45  abs
 * часть функций перенесена в sys5.c
 *
 * Revision 2.4  89/01/07  21:11:20  korotaev
 * Слияние с исправлениями насчет FIFO, +fcntl, + разные режимы открытия
 *
 * (from VAX/UTS SV.2)
 *
 * Revision 2.3  88/11/29  13:27:34  abs
 * style
 *
 * Revision 2.2  88/11/28  14:32:35  avg
 * Стилистические правки.
 *
 * Revision 2.1  88/11/09  11:50:08  abs
 * эксперименты над структурой каталога.
 *
 * Revision 1.4  88/11/02  21:20:26  dvolodin
 * *** empty log message ***
 *
 * Revision 1.3  88/03/23  13:39:33  korotaev
 * Состояние после слияния с АЗЛК, Э-85 и Бурковским планировщиком
 *
 * Revision 1.2  87/04/18  15:26:44  avg
 * Правлен reboot под EXMOUNT.
 *
 * Revision 1.1  86/04/19  15:52:51  avg
 * Initial revision
 *
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include "../include/reg.h"
#include <sys/inode.h>
#include <sys/proc.h>
#include <sys/timeb.h>
#include "../include/quota.h"
#include <sys/reboot.h>
#include <sys/filsys.h>

/*
 * Everything in this file is a routine implementing a system call.
 */

/*
 * return the current time (old-style entry)
 */
gtime()
{
	u.u_r.r_time = time;
}

/*
 * New time entry-- return TOD with milliseconds, timezone,
 * DST flag
 */
ftime()
{
	struct a {
		struct  timeb *tp;
	};
	struct timeb t;
	register unsigned ms;

	(void) _spl7();
	t.time = time;
	ms = lbolt;
	(void) _spl0();
	if (ms > hz) {
		ms -= hz;
		t.time++;
	}
	t.millitm = (1000*ms)/hz;
	t.timezone = timezone;
	t.dstflag = dstflag;
	if (copyout((caddr_t)&t, (caddr_t)(((struct a *) u.u_ap)->tp), sizeof(t)) < 0)
		u.u_error = EFAULT;
}

/*
 * Set the time
 */
stime()
{
	register struct a {
		time_t  time;
	} *uap;

	if (suser()) {
		uap = (struct a *)u.u_ap;
		bootime += uap->time - time;
		time = uap->time;
	}
}

setuid()
{
	register uid;
	struct a {
		int     uid;
	};

	uid = ((struct a *) u.u_ap)->uid;
	if (u.u_ruid == uid || u.u_uid == uid || suser()) {
		u.u_uid = uid;
		u.u_procp->p_uid = uid;
		u.u_ruid = uid;
	}
}

getuid()
{
	u.u_r.r_val1 = u.u_ruid;
	u.u_r.r_val2 = u.u_uid;
}

setgid()
{
	register gid;
	struct a {
		int     gid;
	};

	gid = ((struct a *) u.u_ap)->gid;
	if (u.u_rgid == gid || u.u_gid == gid || suser()) {
		u.u_gid = gid;
		u.u_rgid = gid;
	}
}

getgid()
{
	u.u_r.r_val1 = u.u_rgid;
	u.u_r.r_val2 = u.u_gid;
}

getpid()
{
	u.u_r.r_val1 = u.u_procp->p_pid;
	u.u_r.r_val2 = u.u_procp->p_ppid;
}

/* NICE system call (transplanted here from sys4.c for better modularity) */
/* emulates old NICE by mapping to setblev */
nice()
{
	register struct a {
		int     niceness;
	} *uap = (struct a *)u.u_ap;
#ifdef SCHED                          /* от -16 до -20 -> 8 */
	register int nlev;                      /* -14 -> 7 */
						/* -12 -> 6 */
	nlev = -uap->niceness/2;                /* -10 -> 5 */
	if (nlev > ULEVEL)                      /* -8  -> 4 */
		nlev = ULEVEL;                  /* -6  -> 3 */
	else if ( nlev < -ULEVEL )              /* -4  -> 2 */
		nlev = -ULEVEL;                 /* -2  -> 1 */
	if(nlev > 0 && suser() == 0)            /*  0  -> 0 */
		return;                         /*  2 -> -1 */
	nlev += u.u_procp->p_baslev;            /*  4 -> -2 */
	if(nlev > RTLEVEL - 1)                  /*   . . .  */
		nlev = RTLEVEL - 1;             /* 14 -> -7 */
	setbaslev(-1, nlev);           /* от 16 до 20 -> -8 */
#else SCHED
	register oldnice, newnice;

	oldnice = u.u_procp->p_nice;
	newnice = oldnice + uap->niceness;
	if (newnice >= 2 * NZERO)
		newnice = 2 * NZERO - 1;
	if (newnice < 0)
		newnice = 0;
	if (newnice < oldnice && !suser())      /* tsk tsk */
		newnice = oldnice;
	u.u_procp->p_nice = newnice;
#endif SCHED
}

/*
 * renice -- change the nice value of a process
 */
renice()
{
	register struct a {
		int pid;
#ifdef SCHED
		int level;
#else
		int nice;
#endif SCHED
	} *uap = (struct a *) u.u_ap;
#ifdef SCHED
	if(uap->level > RTLEVEL-1 && suser() == 0)
		return;
	setbaslev(uap->pid, uap->level);
#else SCHED
	register struct proc *p;

	for (p = &proc[2]; p <= maxproc; p++)
		if (p->p_pid == uap->pid) {
			if (suser()) {
				u.u_r.r_val1 = p->p_nice;
				p->p_nice = MAX(uap->nice, -127);
			}
			else if (p->p_uid == u.u_uid) {
				u.u_r.r_val1 = p->p_nice;
				p->p_nice = MIN(MAX(p->p_nice, uap->nice), 127);
				u.u_error = 0;
			}
			return;
		}
	u.u_error = ESRCH;
#endif SCHED
}

/*
 * Unlink system call.
 * Hard to avoid races here, especially
 * in unlinking directories.
 */
unlink()
{
	register struct inode *ip, *pp;

	pp = namei(uchar, DELETE, 0);
	if (pp == NULL)
		return;
	/*
	 * Check for unlink(".")
	 * to avoid hanging on the iget
	 */
	if (pp->i_number == u.u_dent.d_ino) {
		ip = pp;
		ip->i_count++;
	} else
		ip = iget(pp->i_dev, u.u_dent.d_ino);
	if (ip == NULL)
		goto out1;
#ifdef UCB_QUOTAS
	/*
	 * Only superuser can unlink directories or a quota
	 */
	if (((ip->i_mode & IFMT) == IFDIR || isquot(ip)) && !suser())
#else
	if ((ip->i_mode&IFMT)==IFDIR && !suser())
#endif
		goto out;
	/*
	 * Don't unlink a mounted file.
	 */
	if (ip->i_dev != pp->i_dev) {
		u.u_error = EBUSY;
		goto out;
	}
	if (ip->i_flag&ITEXT)
		xrele(ip);      /* try once to free text */
	if (ip->i_flag&ITEXT && ip->i_nlink==1) {
		u.u_error = ETXTBSY;
		goto out;
	}
	/*
	 * sticky bit on a directory means only
	 * owner or super-user may remove file - for /tmp, etc.
	 */
	if ((pp->i_mode & ISVTX) && !own(ip))
		goto out;
	dirremove( pp );   /* see sys5.c (erase current entry) */
	ip->i_nlink--;
#ifdef UCB_QUOTAS
	/*
	 * make unlinks force deallocation of blocks in quota
	 */
	if (ip->i_nlink <= 0)
		qcopy(pp, ip);
#endif
	ip->i_flag |= ICHG;

out:
	iput(ip);
out1:
	iput(pp);
}
chdir()
{
	chdirec(&u.u_cdir);
}

chroot()
{
	if (suser())
		chdirec(&u.u_rdir);
}

chdirec(ipp)
register struct inode **ipp;
{
	register struct inode *ip;

	ip = namei(uchar, LOOKUP, 1);
	if (ip == NULL)
		return;
	if ((ip->i_mode&IFMT) != IFDIR) {
		u.u_error = ENOTDIR;
		goto bad;
	}
	if (access(ip, IEXEC))
		goto bad;
	prele(ip);
	if (*ipp) {
		plock(*ipp);
		iput(*ipp);
	}
	*ipp = ip;
	return;

bad:
	iput(ip);
}

chmod()
{
	register struct inode *ip;
	register struct a {
		char    *fname;
		int     fmode;
	} *uap;

	if ((ip = owner(1)) == NULL)
		return;
	ip->i_mode &= ~07777;
	uap = (struct a *)u.u_ap;
	if (u.u_uid)
		uap->fmode &= ~ISVTX;
	ip->i_mode |= uap->fmode & 07777;
	ip->i_flag |= ICHG;
	if (ip->i_flag & ITEXT && (ip->i_mode & ISVTX)==0)
		xrele(ip);
	iput(ip);
}

chown()
{
	register struct inode *ip;
	register struct a {
		char    *fname;
		int     uid;
		int     gid;
	} *uap;

	if (!suser() || (ip = namei(uchar, LOOKUP, 0)) == NULL)
		return;
	uap = (struct a *)u.u_ap;
	ip->i_uid = uap->uid;
	ip->i_gid = uap->gid;
	ip->i_mode &= ~(ISUID|ISGID);
	ip->i_flag |= ICHG;
	iput(ip);
}

ssig()
{
	register int (*f)();
	register struct a {
		int     signo;
		int     (*fun)();
	} *uap = (struct a *)u.u_ap;
	register struct proc *p = u.u_procp;
	int a;
	long sigmask;

	a = uap->signo & SIGNUMMASK;
	f = uap->fun;
	if (a<=0 || a>=NSIG || a==SIGKILL || a==SIGSTOP ||
	    a==SIGCONT && (f == SIG_IGN || f == SIG_HOLD)) {
		u.u_error = EINVAL;
		return;
	}
	if ((uap->signo &~ SIGNUMMASK) || (f != SIG_DFL && f != SIG_IGN &&
	    SIGISDEFER(f)))
		p->p_flag |= SNUSIG;
	/*
	 * Don't clobber registers if we are to simulate
	 * a ret+rti.
	 */
	if ((uap->signo&SIGDORTI) == 0)
		u.u_r.r_val1 = (int)u.u_signal[a];
	/*
	 * Change setting atomically.
	 */
	(void) _spl6();
	sigmask = 1L << (a-1);
	if (f == SIG_IGN)
		p->p_sig &= ~sigmask;           /* never to be seen again */
	u.u_signal[a] = f;
	if (f != SIG_DFL && f != SIG_IGN && f != SIG_HOLD)
		f = SIG_CATCH;
	if ((int)f & 1)
		p->p_siga0 |= sigmask;
	else
		p->p_siga0 &= ~sigmask;
	if ((int)f & 2)
		p->p_siga1 |= sigmask;
	else
		p->p_siga1 &= ~sigmask;
	(void) _spl0();
	/*
	 * Now handle options.
	 */
	if (uap->signo & SIGDOPAUSE) {
		/*
		 * Simulate a PDP11 style wait instrution which
		 * atomically lowers priority, enables interrupts
		 * and hangs.
		 */
		pause();
		/*NOTREACHED*/
	}
	if (uap->signo & SIGDORTI)
		u.u_eosys = SIMULATERTI;
}

kill()
{
	register struct proc *p;
	register a;
	register struct a {
		int     pid;
		int     signo;
	} *uap = (struct a *)u.u_ap;
	int f, priv, sig;

	f = 0;
	priv = 0;
	a = uap->pid;
	sig = uap->signo;
	if (sig < 0)
		/*
		 * A negative signal means send to process group.
		 */
		uap->signo = -uap->signo;
	if (uap->signo == 0 || uap->signo > NSIG) {
		u.u_error = EINVAL;
		return;
	}
	if (a > 0 && sig > 0) {
		for(p = &proc[0]; p <= maxproc; p++)
			if ( p->p_pid == a)
				goto found;
		p = 0;
found:
		if (p == 0 || u.u_uid && u.u_uid != p->p_uid) {
			u.u_error = ESRCH;
			return;
		}
		psignal(p, uap->signo);
		return;
	}
	if (a==-1 && u.u_uid==0) {
		priv++;
		a = 0;
		sig = -1;               /* like sending to pgrp */
	} else if (a==0) {
		/*
		 * Zero process id means send to my process group.
		 */
		sig = -1;
		a = u.u_procp->p_pgrp;
		if (a == 0) {
			u.u_error = EINVAL;
			return;
		}
	}
	for(p = &proc[0]; p <= maxproc; p++) {
		if (p->p_stat == NULL)
			continue;
		if (sig > 0) {
			if (p->p_pid != a)
				continue;
		} else if (p->p_pgrp!=a && priv==0 || p<=&proc[1] ||
		    (priv && p==u.u_procp))
			continue;
		if (u.u_uid != 0 && u.u_uid != p->p_uid &&
		    (uap->signo != SIGCONT || !inferior(p)))
			continue;
		f++;
		psignal(p, uap->signo);
	}
	if (f == 0)
		u.u_error = ESRCH;
}

times()
{
	register struct a {
		time_t  (*times)[4];
	} *uap = (struct a *)u.u_ap;

	if (copyout((caddr_t)&u.u_utime, (caddr_t)uap->times,
	    sizeof(*uap->times)) < 0)
		u.u_error = EFAULT;
}

profil()
{
	register struct a {
		short   *bufbase;
		unsigned bufsize;
		unsigned pcoffset;
		unsigned pcscale;
	} *uap = (struct a *)u.u_ap;

	u.u_prof.pr_base = uap->bufbase;
	u.u_prof.pr_size = uap->bufsize;
	u.u_prof.pr_off = uap->pcoffset;
	u.u_prof.pr_scale = uap->pcscale;
}

/*
 * alarm clock signal
 */
alarm()
{
	register struct proc *p;
	register c;
	struct a {
		int     deltat;
	};

	p = u.u_procp;
	c = p->p_clktim;
	p->p_clktim = ((struct a *) u.u_ap)->deltat;
	u.u_r.r_val1 = c;
}

/*
 * indefinite wait.
 * no one should wakeup(&u)
 */
pause()
{
	for(;;)   sleep((caddr_t)&u, PSLEP);
}

/*
 * mode mask for creation of files
 */
umask()
{
	struct a {
		int     mask;
	};

	u.u_r.r_val1 = u.u_cmask;
	u.u_cmask = ((struct a *)u.u_ap)->mask & 0777;
}

/*
 * Set IUPD and IACC times on file.
 * Can't set ICHG.
 */
utime()
{
	struct a {
		char    *fname;
		time_t  *tptr;
	};
	register struct inode *ip;
	time_t tv[2];

	if ((ip = owner(1)) == NULL)
		return;
	if (copyin((caddr_t)((struct a *) u.u_ap)->tptr, (caddr_t)tv, sizeof(tv))) {
		u.u_error = EFAULT;
	} else {
		ip->i_flag |= IACC|IUPD|ICHG;
#ifdef UCB_FSFIX
		iupdat(ip, &tv[0], &tv[1], 0);
#else
		iupdat(ip, &tv[0], &tv[1]);
#endif
	}
	iput(ip);
}

#ifdef CGL_RTP
/*
 * make/unmake this process into a real time one.
 */
rtp()
{
	register struct proc *p;
	struct a {
		int flag;
	};

	if (u.u_gid != 0 && !suser()) {
		u.u_error = EPERM;
		return;
	}
	p = u.u_procp;
	if (((struct a *) u.u_ap)->flag) {
		if (rtpp != NULL) {
			u.u_error = EBUSY;      /* exclusive use error */
			return;
		}
		p->p_flag |= SULOCK;
		rtpp = p;
	} else {
		p->p_flag &= ~SULOCK;
		rtpp = NULL;
	}
}
#endif

/*
 * Setpgrp on specified process and its descendants.
 * Pid of zero implies current process.
 * Pgrp of zero is getpgrp system call returning
 * current process group.
 */
setpgrp()
{
	register struct proc *top;
	register struct a {
		int     pid;
		int     pgrp;
	} *uap = (struct a *)u.u_ap;

	if (uap->pid==0)
		top = u.u_procp;
	else {
		for(top = &proc[0]; top <= maxproc; top++)
			if ( uap->pid == top->p_pid)
				goto found1;
		top = 0;
found1:
		if (top == 0) {
			u.u_error = ESRCH;
			return;
		}
	}
	if (uap->pgrp <= 0) {
		u.u_r.r_val1 = top->p_pgrp;
		return;
	}
	if (top->p_uid != u.u_uid && u.u_uid && !inferior(top))
		u.u_error = EPERM;
	else
		top->p_pgrp = uap->pgrp;
}

spgrp(top, npgrp)
register struct proc *top;
{
	register struct proc *pp, *p;
	int f = 0;

	for (p = top; npgrp == -1 || u.u_uid == p->p_uid ||
	    !u.u_uid || inferior(p); p = pp) {
		if (npgrp == -1) {
#define bit(a)  (1L<<(a-1))
			p->p_sig &= ~(bit(SIGTSTP)|bit(SIGTTIN)|bit(SIGTTOU));
			p->p_flag |= SDETACH;
		} else
			p->p_pgrp = npgrp;
		f++;
		/*
		 * Search for children.
		 */
		for (pp = &proc[0]; pp <= maxproc; pp++)
			if (pp->p_pptr == p)
				goto cont;
		/*
		 * Search for siblings.
		 */
		for (; p != top; p = p->p_pptr)
			for (pp = p + 1; pp <= maxproc; pp++)
				if (pp->p_pptr == p->p_pptr)
					goto cont;
		break;
	cont:
		;
	}
	return (f);
}

/*
 * Is p an inferior of the current process?
 */
inferior(p)
register struct proc *p;
{

	for (; p != u.u_procp; p = p->p_pptr)
		if (p <= &proc[1])
			return (0);
	return (1);
}

reboot()
{
	register struct filsys *fp;
	register struct buf    *bp;
	struct a {
		dev_t   dev;
		int     opt;
	};

	if (suser()) {
		/*
		 *  Force the root filesystem's superblock to be updated,
		 *  so the date will be as current as possible after
		 *  rebooting.
		 */
#ifdef EXMOUNT
		if (bp = getfs(rootdev)) {
			fp = (struct filsys *)mapin(bp);
			fp->s_fmod = 1;
			mapout(bp);
		}
#else
		if (fp = getfs(rootdev))
			fp->s_fmod = 1;
#endif EXMOUNT
		boot((struct a *)u.u_ap->dev,(struct a *)u.u_ap->opt);
	}
}

#ifdef SCHED
/*
 * The SETBASLEV routine is the key means of manipulating process
 * levels for the new scheduler.  Our "UNIMAX" system (registered
 * trademark of Hospital Systems Study Group, of course) has a
 * "setblev" system call which simply grabs arguments from user
 * space and calls this SETBASLEV kernel subroutine.  As you can see,
 * the emulation of the NICE system call (above) also uses SETBASLEV.
 */
extern short curlevel;

setbaslev(pid,lev)
register int pid;
register int lev;
{
	register struct proc *p;

	/* normalize new base level */

	if(lev < 0)
		lev = 0;
	else if(lev > MAXLEVEL)
		lev = MAXLEVEL;


	/* locate process */

	if(pid < 0)
		p = u.u_procp;
	else {
		for(p = &proc[0]; p < &proc[nproc]; p++)
			if(p->p_pid == pid)
				break;

		if(p >= &proc[nproc]){
			u.u_error = ESRCH;
			return;
		}
	}

	/* can't increase level or zap someone else's process unless root */
	if(((lev > p->p_baslev) || (p->p_uid != u.u_uid)) && !suser())
		return;

	/* OK, set everything appropriately */
	p->p_baslev = lev;
	if(p->p_level < lev){
		p->p_level = lev;
		if(p == u.u_procp)
			curlevel = lev;
	}
}

/*       Убрал Коротаев М.В. (есть вызов renice).
 *setblev()
 *{
 *        register struct  a {
 *                int pid;
 *                int lev;
 *        } *uap;
 *        uap = (struct a *)u.u_ap;
 *        setbaslev(uap->pid,uap->lev);
 *}
 */
#endif  SCHED

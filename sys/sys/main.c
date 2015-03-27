/*
 * $Log:	main.c,v $
 * Revision 22.3  90/11/12  19:12:07  root
 * ����� ���� ��� ��1425 � ����������� include.
 * 
 * Revision 22.2  89/04/28  17:13:54  avg
 * �������� #include "h/space.h" ��� �������� SEMMNS � MSGMNI.
 *
 * Revision 22.1  89/04/12  15:23:54  korotaev
 * "param.h" ==> <sys/param.h>
 *
 * Revision 22.0  89/03/30  14:03:49  korotaev
 * Begining DEMOS 2.2.
 *
 * Revision 1.9  89/03/09  17:25:35  abs
 * messages
 *
 * Revision 1.8  89/03/03  14:18:49  abs
 * semaphores
 *
 * Revision 1.7  89/01/31  21:42:44  korotaev
 * � ����. ��� ��������� �������� include.
 *
 * Revision 1.6  89/01/31  20:55:22  korotaev
 * ��������� ���������� �����.
 *
 * Revision 1.5  88/08/30  13:51:32  korotaev
 * ��������� ��������� ����������� �������� ����������.
 *
 * Revision 1.4  88/03/23  13:34:37  korotaev
 * ��������� ����� ������� � ����, �-85 � ���������� �������������
 *
 * Revision 1.3  88/01/23  17:47:16  korotaev
 * ��������� ����� �������� � AVG-�-85.
 *
 * Revision 1.2  87/04/18  13:31:21  avg
 * ������� ������ ��� EXMOUNT.
 *
 * Revision 1.1  86/04/19  15:50:22  avg
 * Initial revision
 *
 */

/*NOXSTR*/

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/filsys.h>
#include "../include/mount.h"
#include "../include/map.h"
#include <sys/proc.h>
#include <sys/inode.h>
#include <sys/seg.h>
#include <sys/conf.h>
#include "../include/buf.h"
#include <sys/file.h>
#include "h/space.h"    /* ��� SEMMNS � MSGMNI */
#include <sys/sem.h>
#include <sys/msg.h>

#ifdef  UCB_FRCSWAP
/*
 *      If set, allow incore forks and expands.
 *      Set before idle(), cleared in clock.c.
 *      Set to 1 here because the init creation
 *      must not cause a swap.
 */
int     idleflg = 1;
#endif

/*
 * Initialization code.
 * Called from cold start routine as
 * soon as a stack and segmentation
 * have been established.
 * Functions:
 *      clear and free user core
 *      turn on clock
 *      hand craft 0th process
 *      call all initialization routines
 *      fork - process 0 to schedule
 *           - process 1 execute bootstrap
 */

/*YESXSTR*/

main()
{
	extern char version[];

	printf("\n%s", version);
	startup();
	vect_conflict();

	/*
	 * set up system process
	 */
	proc[0].p_addr = *ka6;
#ifndef VIRUS_VFORK
	proc[0].p_size = USIZE;
#endif
	proc[0].p_stat = SRUN;
	proc[0].p_flag |= SLOAD|SSYS;
#ifdef SCHED
	proc[0].p_baslev = proc[0].p_level = ULEVEL;
#else  SCHED
	proc[0].p_nice = NZERO;
#endif SCHED
	u.u_usize = USIZE;
	u.u_procp = &proc[0];
	u.u_cmask = CMASK;

	/*
	 * Initialize devices and
	 * set up 'known' i-nodes
	 */

#ifdef  UCB_IHASH
	ihinit();
#endif
	cinit();
	binit();
#if SEMMNS > 0
	seminit();
#endif
#if MSGMNI > 0
	msginit();
#endif
#ifdef  UNIBUS_MAP
	(void) ubinit();
#endif  UNIBUS_MAP
#ifdef  UCB_NET
	netinit();
#endif
	clkstart();
	iinit();
	rootdir = iget(rootdev, (ino_t)ROOTINO);
	rootdir->i_flag &= ~ILOCK;
	u.u_cdir = iget(rootdev, (ino_t)ROOTINO);
	u.u_cdir->i_flag &= ~ILOCK;

	/*
	 * make init process
	 * enter scheduling loop
	 * with system process
	 */
#ifdef  VIRUS_VFORK
	if(newproc(0))
#else
	if(newproc())
#endif
		{
#ifdef  VIRUS_VFORK
		expand((int)btoc(szicode),S_DATA);
#else
		expand(USIZE + (int)btoc(szicode));
#endif
		estabur((unsigned)0, btoc(szicode), (unsigned)0, 0, RO);
		copyout((caddr_t)icode, (caddr_t)0, szicode);
		/*
		 * Return goes to loc. 0 of user init
		 * code just copied out.
		 */
		return;
	}
	else
		sched();
}

/*
 * Iinit is called once (from main)
 * very early in initialization.
 * It reads the root's super block
 * and initializes the current date
 * from the last modified date.
 *
 * panic: iinit -- cannot read the super
 * block (usually because of an IO error).
 */
iinit()
{
	register struct buf *cp, *bp;
	register struct filsys *fp;
	register i;

	(*bdevsw[major(rootdev)].d_open)(rootdev, FREAD|FWRITE);
	(*bdevsw[major(swapdev)].d_open)(swapdev, FREAD|FWRITE);
	bp = bread(rootdev, SUPERB);
	if(u.u_error)
		panic("iinit");
#ifndef EXMOUNT
	fp = &mount[0].m_filsys;
	bcopy(mapin(bp), (caddr_t)fp, sizeof(struct filsys));
	mapout(bp);
	brelse(bp);
#else /* EXMOUNT */
	cp = geteblk();
	mount[0].m_filsys = cp;
	copybb( bp, cp, 0, 0, sizeof(struct filsys)/sizeof(int) );
	brelse(bp);
	fp = (struct filsys *)mapin(cp);
#endif EXMOUNT
	mount[0].m_inodp = (struct inode *) 1;
	mount[0].m_dev = rootdev;
	fp->s_flock = 0;
	fp->s_ilock = 0;
	fp->s_ronly = 0;
#ifdef  UCB_IHASH
	fp->s_lasti = 1;
	fp->s_nbehind = 0;
#endif
	fp->s_fsmnt[0] = '/';
	for (i = 1; i < sizeof(fp->s_fsmnt); i++)
		fp->s_fsmnt[i] = 0;
	time = fp->s_time;
	bootime = time;
#ifdef EXMOUNT
	mapout(cp);
#endif EXMOUNT
}

memaddr bpaddr;         /* physical click-address of buffers */

/*
 * Initialize the buffer I/O system by freeing
 * all buffers and setting all device buffer lists to empty.
 */
binit()
{
	register struct buf *bp;
	register struct buf *dp;
	register int i;
	struct bdevsw *bdp;
	long paddr;

	bfreelist.b_forw = bfreelist.b_back =
	    bfreelist.av_forw = bfreelist.av_back = &bfreelist;
	paddr = ((long) bpaddr) << 6;
	for (i=0; i<nbuf; i++) {
		bp = &buf[i];
		bp->b_dev = NODEV;
		bp->b_un.b_addr = loint(paddr);
		bp->b_xmem = hiint(paddr);
		paddr += bsize;
		bp->b_back = &bfreelist;
		bp->b_forw = bfreelist.b_forw;
		bfreelist.b_forw->b_back = bp;
		bfreelist.b_forw = bp;
		bp->b_flags = B_BUSY;
		brelse(bp);
	}
	for (bdp = bdevsw; bdp < bdevsw + nblkdev; bdp++) {
		dp = bdp->d_tab;
		if(dp) {
			dp->b_forw = dp;
			dp->b_back = dp;
		}
		(void) (*bdp->d_root)();
	}
}

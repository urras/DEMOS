/*
 * $Log:	pipe.c,v $
 * Revision 22.2  90/11/12  19:15:16  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.1  89/04/12  15:29:03  korotaev
 * "param.h" ==> <sys/param.h>
 * 
 * Revision 22.0  89/03/30  16:08:10  korotaev
 * Begining DEMOS 2.2.
 * 
 * Revision 1.3  89/01/07  20:48:51  korotaev
 * Слияние с исправлениями насчет FIFO, +fcntl, + разные режимы открытия
 * 
 * (from VAX/UTS SV.2)
 * 
 * Revision 1.2  88/01/23  18:57:59  korotaev
 * Состояние перед сливанием с AVG-Э-85.
 * 
 * Revision 1.1  86/04/19  15:50:51  avg
 * Initial revision
 *
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/inode.h>
#include <sys/file.h>
#include "../include/reg.h"
#include "../include/inline.h"

/*
 * Max allowable buffering per write on a pipe.
 * This is also roughly the max size of the
 * file created to implement the pipe.
 * If this size is bigger than 4096
 * (5120 without UCB_NKB),
 * pipes will be implemented with large
 * files, which is probably not good.
 */

/*
 * The sys-pipe entry.
 * Allocate an inode on the root device.
 * Allocate 2 file structures.
 * Put it all together with flags.
 */
pipe()
{
	register struct inode *ip;
	register struct file *rf, *wf;
	register int r;

	ip = ialloc(pipedev);
	if(ip == NULL) {
		/*
		 * Someone may not have mounted pipedev so we
		 * try to get an inode from rootdev.
		 * Getfs() will have already printed a message
		 * about its inability to find pipedev in the
		 * mount table, so don't complain.
		 */
		ip = ialloc(rootdev);
		if (ip == NULL)
			return;
		/*
		 * This is necessary because the ialloc(pipedev) above
		 * set u.u_error to ENOSPC.
		 */
		else
			u.u_error = 0;
	}
	rf = falloc();
	if(rf == NULL) {
		iput(ip);
		return;
	}
	r = u.u_r.r_val1;
	wf = falloc();
	if(wf == NULL) {
		rf->f_count = 0;
		u.u_ofile[r] = NULL;
		iput(ip);
		return;
	}
	u.u_r.r_val2 = u.u_r.r_val1;
	u.u_r.r_val1 = r;
	wf->f_flag = FWRITE;
	wf->f_inode = rf->f_inode = ip;
	rf->f_flag = FREAD;
	ip->i_count = 2;
	ip->i_frptr = ip->i_fwptr = 0;
	ip->i_frcnt = ip->i_fwcnt = 1;
	ip->i_mode = IFIFO;
	ip->i_flag = IACC|IUPD|ICHG;
	prele(ip);
}

/*
 *      FROM SVR2
 */

/*
 * Open a pipe
 * Check read and write counts, delay as necessary
 */

openp(ip, mode)
register struct inode *ip;
register mode;
{
	if (mode&FREAD) {
		if( (ip->i_frcnt++ == 0) && (ip->i_flag&IFIR) ){
			ip->i_flag &= ~IFIR;
			wakeup((caddr_t)&ip->i_frcnt);
		}
	}
	if (mode&FWRITE) {
		if (mode&FNDELAY && ip->i_frcnt == 0) {
			u.u_error = ENXIO;
			return;
		}
		if( (ip->i_fwcnt++ == 0) && (ip->i_flag&IFIW) ){
			ip->i_flag &= ~IFIW;
			wakeup((caddr_t)&ip->i_fwcnt);
		}
	}
	if (mode&FREAD) {
		while (ip->i_fwcnt == 0) {
			if (mode&FNDELAY || ip->i_size)
				return;
			ip->i_flag |= IFIW;
			sleep(&ip->i_fwcnt, PPIPE);
		}
	}
	if (mode&FWRITE) {
		while (ip->i_frcnt == 0){
			ip->i_flag |= IFIR;
			sleep(&ip->i_frcnt, PPIPE);
		}
	}
}

/*
 * Close a pipe
 * Update counts and cleanup
 */
closep(ip, mode)
register struct inode *ip;
register mode;
{
	register i;
	daddr_t bn;

	if (mode&FREAD) {
		if ((--ip->i_frcnt == 0) && (ip->i_flag&IFIW)) {
			ip->i_flag &= ~IFIW;
			wakeup((caddr_t)&ip->i_fwcnt);
		}
	}
	if (mode&FWRITE) {
		if ((--ip->i_fwcnt == 0) && (ip->i_flag&IFIR)) {
			ip->i_flag &= ~IFIR;
			wakeup((caddr_t)&ip->i_frcnt);
		}
	}
	if ((ip->i_frcnt == 0) && (ip->i_fwcnt == 0)) {
		for( i = NADDR-4 ; i >= 0 ; i-- ){
			bn = ip->i_un.i_addr[i];
			if( bn == (daddr_t)0 )
				continue;
			ip->i_un.i_addr[i] = (daddr_t)0;
#ifdef  UCB_QUOTAS
			qfree(ip, bn);
#else
			free(ip->i_dev, bn);
#endif
		}
		ip->i_frptr = ip->i_fwptr = 0;
		ip->i_size = 0;
		ip->i_flag |= IUPD|ICHG;
	}
}

#ifdef  plock
#undef  plock
#endif
#ifdef  prele
#undef  prele
#endif
/*
 * Lock a pipe.
 * If its already locked,
 * set the WANT bit and sleep.
 */
plock(ip)
register struct inode *ip;
{
	register s = spl6();

	while(ip->i_flag&ILOCK) {
		ip->i_flag |= IWANT;
		sleep((caddr_t)ip, PINOD);
	}
	ip->i_flag |= ILOCK;
	splx(s);
}

/*
 * Unlock a pipe.
 * If WANT bit is on,
 * wakeup.
 * This routine is also used
 * to unlock inodes in general.
 */
prele(ip)
register struct inode *ip;
{

	ip->i_flag &= ~ILOCK;
	if(ip->i_flag&IWANT) {
		ip->i_flag &= ~IWANT;
		wakeup((caddr_t)ip);
	}
}

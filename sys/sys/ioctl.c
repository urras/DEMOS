/*
 * Ioctl.
 *
 * $Log:	ioctl.c,v $
 * Revision 22.3  90/11/12  19:11:04  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.2  89/06/22  13:37:51  korotaev
 * *** empty log message ***
 * 
 * Revision 22.1  89/04/12  15:21:17  korotaev
 * "param.h" ==> <sys/param.h>
 * 
 * Revision 22.0  89/03/30  13:49:50  korotaev
 * Begining DEMOS 2.2.
 * 
 * Revision 1.4  89/01/07  20:41:42  korotaev
 * Слияние с исправлениями насчет FIFO, +fcntl, + разные режимы открытия
 * 
 * (from VAX/UTS SV.2)
 * 
 * Revision 1.3  88/06/06  14:39:51  korotaev
 * Вставлен IPK_SELECT (from TCP/IP).
 * 
 * Revision 1.2  88/03/23  13:33:48  korotaev
 * Состояние после слияния с АЗЛК, Э-85 и Бурковским планировщиком
 * 
 * Revision 1.1  86/04/19  15:50:03  avg
 * Initial revision
 * 
 */
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/tty.h>
#include <sys/proc.h>
#include <sys/inode.h>
#include <sys/file.h>
#include "../include/reg.h"
#include <sys/conf.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>

/*
 * stty/gtty writearound
 */
stty()
{
	u.u_arg[2] = u.u_arg[1];
	u.u_arg[1] = TIOCSETP;
	ioctl();
}

gtty()
{
	u.u_arg[2] = u.u_arg[1];
	u.u_arg[1] = TIOCGETP;
	ioctl();
}

/*
 * ioctl system call
 * Check legality, execute common code, and switch out to individual
 * device routine.
 */
ioctl()
{
	register struct file *fp;
	register struct inode *ip;
	register struct a {
		int     fdes;
		int     cmd;
		caddr_t cmarg;
	} *uap = (struct a *)u.u_ap;
	extern int spcnum;
	dev_t dev;
	int fmt;

	if( (uap->cmd>>8)=='?' ){
		if (copyout((caddr_t)&spcnum, uap->cmarg, sizeof(int)))
			u.u_error = EFAULT;
		return;
	}
	if ((fp = getf(uap->fdes)) == NULL)
		return;
	if ((fp->f_flag & (FREAD|FWRITE)) == 0) {
		u.u_error = EBADF;
		return;
	}
	if (uap->cmd==FIOCLEX) {
		u.u_pofile[uap->fdes] |= EXCLOSE;
		return;
	}
	if (uap->cmd==FIONCLEX) {
		u.u_pofile[uap->fdes] &= ~EXCLOSE;
		return;
	}
#ifdef  UCB_NET
	if (fp->f_flag & FSOCKET) {
		soioctl(fp->f_socket, uap->cmd, uap->cmarg);
		return;
	}
#endif
	if( (uap->cmd >> 8) == FCNTL ){
		fcntl(uap->fdes, fp, uap->cmd, (caddr_t)uap->cmarg);
		return;
	}
	ip = fp->f_inode;
	fmt = ip->i_mode & IFMT;
	if (fmt != IFCHR) {
		if (uap->cmd==FIONREAD && (fmt == IFREG || fmt == IFDIR)) {
			off_t nread = ip->i_size - fp->f_un.f_offset;

			if (copyout((caddr_t)&nread, uap->cmarg, sizeof(off_t)))
				u.u_error = EFAULT;
		} else
		       if (uap->cmd == FIONBIO || uap->cmd == FIOASYNC)
			return;
		else
			u.u_error = ENOTTY;
		return;
	}
	dev = ip->i_un.i_rdev;
	u.u_r.r_val1 = 0;
	if ((u.u_procp->p_flag&SNUSIG) && save(u.u_qsav)) {
		u.u_eosys = RESTARTSYS;
		return;
	}
	(*cdevsw[major(dev)].d_ioctl)(dev, uap->cmd, uap->cmarg, fp->f_flag);
}

/*
 * Do nothing specific version of line
 * discipline specific ioctl command.
 */
/*ARGSUSED*/
nullioctl(tp, cmd, addr, flag)
struct tty *tp;
caddr_t addr;
{
	return (cmd);
}

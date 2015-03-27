/*
 * $Log:	fio.c,v $
 * Revision 22.3  90/11/12  19:10:12  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.2  89/07/24  19:26:03  korotaev
 * Исправлен flock.
 * 
 * Revision 22.1  89/04/12  15:18:55  korotaev
 * "param.h" ==> <sys/param.h>
 * 
 * Revision 22.0  89/03/30  13:41:58  korotaev
 * Begining DEMOS 2.2.
 * 
 * Revision 1.7  89/01/07  20:39:52  korotaev
 * Слияние с исправлениями насчет FIFO, +fcntl, + разные режимы открытия
 * 
 * (from VAX/UTS SV.2)
 * 
 * Revision 1.6  88/11/17  19:14:32  dvolodin
 * *** empty log message ***
 * 
 * Revision 1.5  88/07/15  19:05:52  korotaev
 * Добавлены новые устройства: root(255,0) и swap(255,1).
 * При работе с ними используются текущие rootdev и swapdev.
 * 
 * Revision 1.4  88/06/29  15:44:55  korotaev
 * Исправлено поле в <sys/inode.h>
 * 
 * Revision 1.3  87/12/12  18:12:52  korotaev
 * Вставлен системный вызов flock.
 * 
 * Revision 1.2  87/04/18  13:28:41  avg
 * Сделаны правки под EXMOUNT.
 *
 * Revision 1.1  86/04/19  15:49:43  avg
 * Initial revision
 *
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/fcntl.h>
#include <sys/seg.h>
#include <sys/filsys.h>
#include <sys/file.h>
#include <sys/conf.h>
#include <sys/inode.h>
#include "../include/reg.h"
#include <sys/acct.h>
#include <sys/proc.h>

/*
 * Convert a user supplied
 * file descriptor into a pointer
 * to a file structure.
 * Only task is to check range
 * of the descriptor.
 */
struct file *
getf(f)
register int f;
{
	register struct file *fp;

	if(0 <= f && f < NOFILE) {
		fp = u.u_ofile[f];
		if(fp != NULL)
			return(fp);
	}
	u.u_error = EBADF;
	return(NULL);
}

/*
 * Internal form of close.
 * Decrement reference count on
 * file structure.
 * Also make sure the pipe protocol
 * does not constipate.
 *
 * Decrement reference count on the inode following
 * removal to the referencing file structure.
 * Call device handler on last close.
 */
#ifdef  UCB_NET
closef(fp, nouser)
#else
closef(fp)
#endif
register struct file *fp;
{
	register struct inode *ip;
	int flag;
	dev_t dev;
	register int (*cfunc)();
	segm    saveregs;

	if(fp == (struct file *) NULL)
		return;
	ip = fp->f_inode;
	if( ip->i_pflock != NULL ){
		saveseg5(saveregs);
		mapseg5(flock_base, (flock_size << 8) | RW);
		unflock(ip,(long)0,(long)0,(u.u_procp)->p_pid);
		restorseg5(saveregs);
	}
	if (fp->f_count > 1) {
		fp->f_count--;
		return;
	}
	flag = fp->f_flag;
	if( flag & (FSHLOCK|FEXLOCK) )
		ino_unlock(fp, FSHLOCK|FEXLOCK);
#ifdef UCB_NET
	if (flag & FSOCKET) {
		u.u_error = 0;                  /* XXX */
		soclose(fp->f_socket, nouser);
		if (nouser == 0 && u.u_error)
			return;
		fp->f_socket = 0;
		fp->f_count = 0;
		u.u_error = 0;
		return;
	}
#endif
	fp->f_count = 0;
	plock(ip);
	switch(ip->i_mode&IFMT) {
	case IFCHR:
		dev = ip->i_un.i_rdev;
		cfunc = cdevsw[major(dev)].d_close;
		break;
	case IFBLK:
		dev = ip->i_un.i_rdev;
		if( major(dev) == 255 )
			dev = (minor(dev)==0)?rootdev:swapdev;
		cfunc = bdevsw[major(dev)].d_close;
		break;
	case IFIFO:
		closep(ip,flag);
	default:
		iput(ip);
		return;
	}
	iput(ip);
	for(fp = file; fp < fileNFILE; fp++) {
#ifdef  UCB_NET
		if (fp->f_flag & FSOCKET)
			continue;
#endif
		if (fp->f_count && fp->f_inode==ip)
			return;
	}
	(*cfunc)(dev, flag);
}

/*
 * openi called to allow handler
 * of special files to initialize and
 * validate before actual IO.
 */
openi(ip, rw)
register struct inode *ip;
{
	register dev_t dev;
	register unsigned int maj;

	dev = ip->i_un.i_rdev;
	maj = major(dev);
	switch(ip->i_mode&IFMT) {

	case IFCHR:
		if(maj >= nchrdev)
			goto bad;
		(*cdevsw[maj].d_open)(dev, rw & FWRITE);
		break;

	case IFBLK:
		if( maj == 255 ){
			dev = (minor(dev)==0)?rootdev:swapdev;
			maj = major(dev);
		} else if(maj >= nblkdev)
			goto bad;
		(*bdevsw[maj].d_open)(dev, rw & FWRITE);
		break;

	case IFIFO:
		openp(ip, rw);
		break;
	}
	return;

bad:
	u.u_error = ENXIO;
}

/*
 * Check mode permission on inode pointer.
 * Mode is READ, WRITE or EXEC.
 * In the case of WRITE, the
 * read-only status of the file
 * system is checked.
 * Also in WRITE, prototype text
 * segments cannot be written.
 * The mode is shifted to select
 * the owner/group/other fields.
 * The super user is granted all
 * permissions.
 */
access(ip, mode)
register mode;
register struct inode *ip;
{
#ifdef EXMOUNT
	register struct buf *fp;
	int i;
#else
	register struct filsys *fp;
#endif

	if(mode == IWRITE) {
		if ((fp = getfs(ip->i_dev)) == NULL) {
			u.u_error = ENOENT;
			return(1);
		} else {
#ifdef EXMOUNT
			i = ((struct filsys *)mapin(fp))->s_ronly;
			mapout(fp);
			if (i != 0) {
				u.u_error = EROFS;
				return(1);
			}
#else
			if (fp->s_ronly != 0) {
				u.u_error = EROFS;
				return(1);
			}
#endif EXMOUNT
		}
		if (ip->i_flag&ITEXT)           /* try to free text */
			xrele(ip);
		if(ip->i_flag & ITEXT) {
			u.u_error = ETXTBSY;
			return(1);
		}
	}
	if(u.u_uid == 0)
		return(0);
#ifdef  UCB_GRPMAST
	if(u.u_uid != ip->i_uid && !(grpmast() && u.u_gid == ip->i_gid))
#else
	if(u.u_uid != ip->i_uid)
#endif
		{
		mode >>= 3;
		if(u.u_gid != ip->i_gid)
			mode >>= 3;
	}
	if((ip->i_mode&mode) != 0)
		return(0);
bad:
	u.u_error = EACCES;
	return(1);
}

/*
 * Look up a pathname and test if
 * the resultant inode is owned by the
 * current user.
 * If not, try for super-user.
 * If permission is granted,
 * return inode pointer.
 */
struct inode *
owner(follow)
int follow;
{
	register struct inode *ip;

	ip = namei(uchar, LOOKUP, follow);
	if(ip == (struct inode *) NULL)
		return((struct inode *) NULL);
#ifdef  UCB_GRPMAST
	if(own(ip))
		return(ip);
#else
	if(u.u_uid == ip->i_uid)
		return(ip);
	if(suser())
		return(ip);
#endif
	iput(ip);
	return((struct inode *) NULL);
}

own(ip)
register struct inode *ip;
{
	if(ip->i_uid == u.u_uid)
		return(1);
#ifdef  UCB_GRPMAST
	if(grpmast() && u.u_gid == ip->i_gid)
		return(1);
#endif
	if(suser())
		return(1);
	return(0);
}

/*
 * Test if the current user is the
 * super user.
 */
suser()
{

	if(u.u_uid == 0) {
#ifdef  ACCT
		u.u_acflag |= ASU;
#endif
		return(1);
	}
	u.u_error = EPERM;
	return(0);
}

/*
 * Allocate a user file descriptor.
 */
ufalloc(i)
register i;
{
	for(; i<NOFILE; i++)
		if(u.u_ofile[i] == NULL) {
			u.u_r.r_val1 = i;
			u.u_pofile[i] = 0;
			return(i);
		}
	u.u_error = EMFILE;
	return(-1);
}

/*
 * Allocate a user file descriptor
 * and a file structure.
 * Initialize the descriptor
 * to point at the file structure.
 */
struct file *
falloc()
{
	register struct file *fp;
	register i;
	static struct  file *lastf = file;

	i = ufalloc(0);
	if (i < 0)
		return((struct file *) NULL);
	for (fp = lastf; fp < fileNFILE; fp++)
		if (fp->f_count == 0)
			goto slot;
	for (fp = file; fp < lastf; fp++)
		if (fp->f_count == 0)
			goto slot;
	tablefull("file");
	u.u_error = ENFILE;
	return((struct file *) NULL);
slot:
	u.u_ofile[i] = fp;
	fp->f_count++;
	fp->f_un.f_offset = 0;
	lastf = fp + 1;
	return (fp);
}

/*
 *      F R O M   B S D 4.2
 */

/*
 * Unlock a file.
 */
ino_unlock(fp, kind)
register struct file *fp;
int kind;
{
	register struct inode *ip = fp->f_inode;
	register int flags;

	kind &= fp->f_flag;
	if( kind == 0 )
		return;
	flags = ip->i_flag;
	if( kind & FSHLOCK ){
		if( (flags & ISHLOCK) == 0 )
			panic("i_unlock: SH");
		if( --ip->i_lockc == 0 ){
			ip->i_flag &= ~ISHLOCK;
			if( flags & ILWAIT)
				wakeup((caddr_t)&ip->i_lockc);
		}
		fp->f_flag &= ~FSHLOCK;
	}
	if( kind & FEXLOCK ){
		if( (flags & IEXLOCK) == 0 )
			panic("i_unlock: EX");
		if( --ip->i_lockc == 0 ){
			ip->i_flag &= ~(IEXLOCK|ILWAIT);
			if( flags & ILWAIT)
				wakeup((caddr_t)&ip->i_lockc);
		}
		fp->f_flag &= ~FEXLOCK;
	}
}

/*
 * Place an advisory lock on an inode.
 */
ino_lock(fp, cmd)
register struct file *fp;
register int cmd;
{
	int priority = PLOCK;
	register struct inode *ip = fp->f_inode;

	if( cmd & LOCK_EX )
		priority++;
again:
	while( ip->i_flag & (IEXLOCK|ILWAIT) ){
		if( cmd & LOCK_NB )
			return(EWOULDBLOCK);
		if( fp->f_flag & FEXLOCK ){
			ino_unlock(fp, FEXLOCK);
			goto again;
		}
		ip->i_flag |= ILWAIT;
		sleep((caddr_t)&ip->i_lockc, priority);
	}
	if( cmd & LOCK_EX ){
		cmd &= ~LOCK_SH;
		while( ip->i_flag & ISHLOCK ){
			if( cmd & LOCK_NB )
				return(EWOULDBLOCK);
			if( fp->f_flag & FSHLOCK ){
				ino_unlock(fp, FSHLOCK);
				goto again;
			}
			ip->i_flag |= ILWAIT;
			sleep((caddr_t)&ip->i_lockc, PLOCK);
		}
	}
	if( fp->f_flag & (FSHLOCK|FEXLOCK) )
		panic("i_lock");
	if( cmd & LOCK_SH ){
		ip->i_lockc++;
		ip->i_flag |= ISHLOCK;
		fp->f_flag |= FSHLOCK;
	}
	if( cmd & LOCK_EX ){
		ip->i_lockc++;
		ip->i_flag |= IEXLOCK;
		fp->f_flag |= FEXLOCK;
	}
	return(0);
}


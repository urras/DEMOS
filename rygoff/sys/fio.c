/*
 * $Log:	fio.c,v $
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

#include "param.h"
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/filsys.h>
#include <sys/file.h>
#include <sys/conf.h>
#include <sys/inode.h>
#include <sys/reg.h>
#include <sys/acct.h>

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
	int flag, mode;
	dev_t dev;
	register int (*cfunc)();
#ifdef  MPX_FILS
	struct chan *cp;
#endif

	if(fp == (struct file *) NULL)
		return;
	if (fp->f_count > 1) {
		fp->f_count--;
		return;
	}
#ifdef IPK_FLOCK
	if( fp->f_flag & (FSHLOCK|FEXLOCK) )
		ino_unlock(fp, FSHLOCK|FEXLOCK);
#endif IPK_FLOCK
	flag = fp->f_flag;
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
	ip = fp->f_inode;
#ifdef  MPX_FILS
	cp = fp->f_un.f_chan;
#endif
	dev = ip->i_un.i_rdev;
	mode = ip->i_mode;

	plock(ip);
	fp->f_count = 0;
	if(flag & FPIPE) {
		ip->i_mode &= ~(IREAD|IWRITE);
		wakeup((caddr_t)ip+1);
		wakeup((caddr_t)ip+2);
	}
	iput(ip);

	switch(mode&IFMT) {

	case IFCHR:
#ifdef  MPX_FILS
	case IFMPC:
#endif
		cfunc = cdevsw[major(dev)].d_close;
		break;

	case IFBLK:
#ifdef  MPX_FILS
	case IFMPB:
#endif
		if( major(dev) == 255 )
			dev = (minor(dev)==0)?rootdev:swapdev;
		cfunc = bdevsw[major(dev)].d_close;
		break;
	default:
		return;
	}

#ifdef  MPX_FILS
	if ((flag & FMP) == 0)
		for(fp = file; fp < fileNFILE; fp++)
			if (fp->f_count && fp->f_inode==ip)
				return;
	(*cfunc)(dev, flag, cp);
#else
	for(fp = file; fp < fileNFILE; fp++) {
#ifdef  UCB_NET
		if (fp->f_flag & FSOCKET)
			continue;
#endif
		if (fp->f_count && fp->f_inode==ip)
			return;
	}
	(*cfunc)(dev, flag);
#endif
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
#ifdef  MPX_FILS
	case IFMPC:
#endif
		if(maj >= nchrdev)
			goto bad;
		(*cdevsw[maj].d_open)(dev, rw);
		break;

	case IFBLK:
#ifdef  MPX_FILS
	case IFMPB:
#endif
		if( maj == 255 ){
			dev = (minor(dev)==0)?rootdev:swapdev;
			maj = major(dev);
		} else if(maj >= nblkdev)
			goto bad;
		(*bdevsw[maj].d_open)(dev, rw);
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
#ifndef UCB_SYMLINKS
owner()
#else
owner(follow)
int follow;
#endif
{
	register struct inode *ip;

#ifndef UCB_SYMLINKS
	ip = namei(uchar, LOOKUP);
#else
	ip = namei(uchar, LOOKUP, follow);
#endif
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
ufalloc()
{
	register i;

	for(i=0; i<NOFILE; i++)
		if(u.u_ofile[i] == NULL) {
			u.u_r.r_val1 = i;
			u.u_pofile[i] = 0;
			return(i);
		}
	u.u_error = EMFILE;
	return(-1);
}

struct  file *lastf;
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

	i = ufalloc();
	if (i < 0)
		return(NULL);
	if (lastf == (struct file *) NULL)
		lastf = file;
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

#ifdef IPK_FLOCK

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
		/* if( (flags & ISHLOCK) == 0 )
			panic("unlock: SHLOCK"); */
		fp->f_flag &= ~FSHLOCK;
		if( --ip->i_lockc == 0 )
			ip->i_flag &= ~(ISHLOCK|ILWAIT);
	} else if( kind & FEXLOCK ){
		/* if( (flags & IEXLOCK) == 0 )
			panic("unlock: EXLOCK"); */
		fp->f_flag &= ~FEXLOCK;
		if( --ip->i_lockc == 0 )
			ip->i_flag &= ~(IEXLOCK|ILWAIT);
	}
	if( flags & ILWAIT)
		wakeup((caddr_t)&ip->i_lockc);
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

	if( cmd == LOCK_EX )
		priority++;
	else if( cmd == LOCK_NB ){
		return((ip->i_flag&(IEXLOCK|ISHLOCK))?EWOULDBLOCK:0);
	}
again:
	while( ip->i_flag & IEXLOCK ){
		if( fp->f_flag & FEXLOCK ){
			ino_unlock(fp, FEXLOCK);
		} else {
			ip->i_flag |= ILWAIT;
			sleep((caddr_t)&ip->i_lockc, priority);
		}
	}
	if( cmd == LOCK_EX ){
		if( ip->i_flag & ISHLOCK ){
			if( fp->f_flag & FSHLOCK ){
				ino_unlock(fp, FSHLOCK);
			} else {
				ip->i_flag |= ILWAIT;
				sleep((caddr_t)&ip->i_lockc, PLOCK);
			}
			goto again;
		}
		ip->i_lockc++;
		ip->i_flag |= IEXLOCK;
		fp->f_flag |= FEXLOCK;
	} else if( cmd == LOCK_SH ){
		ip->i_lockc++;
		ip->i_flag |= ISHLOCK;
		fp->f_flag |= FSHLOCK;
	}
	return(0);
}

#endif IPK_FLOCK

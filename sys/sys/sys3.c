/*
 * $Log:	sys3.c,v $
 * Revision 22.4  90/11/12  19:20:18  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.3  89/06/19  21:26:34  abs
 * поле f_fpack в структуре filsys.
 * используется ustat-ом.
 * 
 * Revision 22.2  89/05/23  14:10:39  abs
 * новый сисвызов ustat
 *
 * Revision 22.1  89/04/12  15:38:26  korotaev
 * "param.h" ==> <sys/param.h>
 *
 * Revision 22.0  89/03/30  16:44:12  korotaev
 * Begining DEMOS 2.2.
 *
 * Revision 1.7  89/01/10  14:07:02  abs
 * Стилистика.
 *
 * Revision 1.6  89/01/07  21:09:32  korotaev
 * Слияние с исправлениями насчет FIFO, +fcntl, + разные режимы открытия
 *
 * (from VAX/UTS SV.2)
 *
 * Revision 1.5  88/11/02  21:19:01  dvolodin
 * *** empty log message ***
 *
 * Revision 1.4  88/01/23  20:11:22  korotaev
 * Слияние с весией AVG-Э-85.
 *
 * Revision 1.3  88/01/23  20:02:01  korotaev
 * Состояние перед слиянием с AVG-Э-85.
 *
 * Revision 1.2  87/04/18  15:26:10  avg
 * Изменения под EXMOUNT.
 *
 * Revision 1.1  86/04/19  15:52:47  avg
 * Initial revision
 *
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/ino.h>
#include "../include/reg.h"
#include "../include/buf.h"
#include <sys/filsys.h>
#include "../include/mount.h"
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/inode.h>
#include <sys/file.h>
#include <sys/conf.h>
#include <sys/stat.h>
#include "../include/inline.h"
#include <sys/ustat.h>

/*
 * the fstat system call.
 */
fstat()
{
	register struct file *fp;
	register struct a {
		int     fdes;
		struct stat *sb;
	} *uap = (struct a *)u.u_ap;

	fp = getf(uap->fdes);
	if (fp == NULL)
		return;
#ifdef  UCB_NET
	if (fp->f_flag & FSOCKET)
		u.u_error = sostat(fp->f_socket, uap->sb);
	else
#endif
		stat1(fp->f_inode, uap->sb);
}

/*
 * the stat system call.
 */
stat()
{
	register struct inode *ip;
	register struct a {
		char    *fname;
		struct stat *sb;
	} *uap;

	uap = (struct a *)u.u_ap;
	ip = namei(uchar, LOOKUP, 1);
	if (ip == NULL)
		return;
	stat1(ip, uap->sb);
	iput(ip);
}

/*
 * Lstat system call; like stat but doesn't follow links.
 */
lstat()
{
	register struct inode *ip;
	register struct a {
		char    *fname;
		struct stat *sb;
	} *uap;

	uap = (struct a *)u.u_ap;
	ip = namei(uchar, 0, 0);
	if (ip == NULL)
		return;
	stat1(ip, uap->sb);
	iput(ip);
}

/*
 * The basic routine for fstat and stat:
 * get the inode and pass appropriate parts back.
 */
stat1(ip, ub)
register struct inode *ip;
struct stat *ub;
{
	register struct dinode *dp;
	register struct buf *bp;
	struct stat ds;
	dev_t   dev;

#ifdef UCB_FSFIX
	IUPDAT(ip, &time, &time, 0);
#else
	IUPDAT(ip, &time, &time);
#endif
	/*
	 * first copy from inode table
	 */
	ds.st_dev = ip->i_dev;
	ds.st_ino = ip->i_number;
	ds.st_mode = ip->i_mode;
	ds.st_nlink = ip->i_nlink;
	ds.st_uid = ip->i_uid;
	ds.st_gid = ip->i_gid;
	ds.st_rdev = (major(ip->i_un.i_rdev)==255) ?
	((minor(ip->i_un.i_rdev)==0) ? rootdev : swapdev) : ip->i_un.i_rdev;
	ds.st_size = ip->i_size;
	/*
	 * next the dates in the disk
	 */
	bp = bread(ip->i_dev, itod(ip->i_number));
	dp = (struct dinode *) mapin(bp);
	dp += itoo(ip->i_number);
	ds.st_atime = dp->di_atime;
	ds.st_mtime = dp->di_mtime;
	ds.st_ctime = dp->di_ctime;
	mapout(bp);
	brelse(bp);
	if (copyout((caddr_t)&ds, (caddr_t)ub, sizeof(ds)) < 0)
		u.u_error = EFAULT;
}

/*
 * the dup system call.
 */
dup()
{
	register struct file *fp;
	register struct a {
		int     fdes;
		int     fdes2;
	} *uap = (struct a *)u.u_ap;
	register i;

	i = uap->fdes & ~077;
	uap->fdes &= 077;
	fp = getf(uap->fdes);
	if (fp == NULL)
		return;
	if ((i&0100) == 0) {
		if ((i = ufalloc(0)) < 0)
			return;
	} else {
		i = uap->fdes2;
		if (i<0 || i>=NOFILE) {
			u.u_error = EBADF;
			return;
		}
		u.u_r.r_val1 = i;
	}
	if (i!=uap->fdes) {
		if (u.u_ofile[i]!=NULL)
#ifndef UCB_NET
			closef(u.u_ofile[i]);
#else
			closef(u.u_ofile[i],0);
#endif
		u.u_ofile[i] = fp;
		fp->f_count++;
	}
}

/*
 * the mount system call.
 */
smount()
{
	dev_t dev;
	char *cp;
	register struct inode *ip;
	register struct mount *mp;
	struct mount *smp;
	register struct filsys *fp;
	struct buf *bp;
#ifdef EXMOUNT
	struct buf *bpf;
#endif
	struct a {
		char    *fspec;
		char    *freg;
		int     ronly;
	} *uap = (struct a *)u.u_ap;

	dev = getmdev();
	if (u.u_error || !suser())
		return;
	u.u_dirp = (caddr_t)uap->freg;
	ip = namei(uchar, LOOKUP, 1);
	if (ip == NULL)
		return;
	if( ip->i_count != 1 )
		goto out;
	if( (ip->i_mode & IFMT) != IFDIR ){
		u.u_error = ENOTDIR;
		goto out1;
	}
	smp = NULL;
	for(mp = mount; mp < mountNMOUNT; mp++) {
		if (mp->m_inodp != NULL) {
			if (dev == mp->m_dev)
				goto out;
		} else
			if (smp == NULL)
				smp = mp;
	}
	mp = smp;
	if (mp == NULL){
		u.u_error = ENFILE;
		goto out1;
	}
	(*bdevsw[major(dev)].d_open)(dev, !uap->ronly);
	if (u.u_error)
		goto out1;
	bp = bread(dev, SUPERB);
	if (u.u_error) {
		brelse(bp);
		goto out1;
	}
	mp->m_inodp = ip;
	mp->m_dev = dev;
#ifndef EXMOUNT
	fp = &mp->m_filsys;
	bcopy((caddr_t) mapin(bp), (caddr_t)fp, sizeof(struct filsys));
	mapout(bp);
#else
	bpf = geteblk();
	mp->m_filsys = bpf;
	copybb( bp, bpf, 0, 0, sizeof(struct filsys)/sizeof(int) );
	fp = (struct filsys *)mapin(bpf);
#endif EXMOUNT
	fp->s_ilock = 0;
	fp->s_flock = 0;
	fp->s_ronly = uap->ronly & 1;
#ifdef  UCB_IHASH
	fp->s_nbehind = 0;
	fp->s_lasti = 1;
#endif
	u.u_dirp = uap->freg;
	for (cp = fp->s_fsmnt; cp < &fp->s_fsmnt[sizeof(fp->s_fsmnt) - 1];)
		if ((*cp++ = uchar()) == 0)
			u.u_dirp--;             /* get 0 again */
	*cp = 0;
#ifdef EXMOUNT
	mapout(bpf);
#endif EXMOUNT
	brelse(bp);
	ip->i_flag |= IMOUNT;
	prele(ip);
	return;
out:
	u.u_error = EBUSY;
out1:
	iput(ip);
}

/*
 * the umount system call.
 */
sumount()
{
	dev_t dev;
	register struct inode *ip;
	register struct mount *mp;
	register struct buf *bp;
	struct buf *dp;
	extern short lbolt;
	int busyf;

	dev = getmdev();
	if (u.u_error || !suser())
		return;
	xumount(dev);   /* remove unused sticky files from text table */
	update();
	for (mp = mount; mp < mountNMOUNT; mp++)
		if (mp->m_inodp != NULL && dev == mp->m_dev) {
			for(ip = inode; ip < inodeNINODE; ip++)
				if (ip->i_number != 0 && dev == ip->i_dev){
				       if(ip->i_count == 0 )
						ip->i_number =0;
					else {
							u.u_error = EBUSY;
							return;
						}
				       }
			dp = bdevsw[major(dev)].d_tab;
	WaitLoop:
			busyf = 0;
			for (bp = dp->b_forw; bp != dp; bp = bp->b_forw) {
				(void) _spl6();
				if (bp->b_dev == dev) {
					if( bp->b_flags & B_BUSY )
						busyf++;
					else {
#ifdef UCB_BHASH
						bunhash(bp);
#endif
						bp->b_dev = NODEV;
					}
				}
				(void) _spl0();
			}
			if( busyf ) {
				sleep( &lbolt, PRIBIO );
				goto WaitLoop;
			}
			(*bdevsw[major(dev)].d_close)(dev, 0);
			ip = mp->m_inodp;
			ip->i_flag &= ~IMOUNT;
			plock(ip);
			iput(ip);
			mp->m_inodp = NULL;
#ifdef EXMOUNT
			brelse(mp->m_filsys);
#endif
			return;
		}

	u.u_error = EINVAL;
}

/*
 * Common code for mount and umount.
 * Check that the user's argument is a reasonable
 * thing on which to mount, and return the device number if so.
 */
dev_t
getmdev()
{
	register dev_t dev;
	register struct inode *ip;

	ip = namei(uchar, LOOKUP, 1);
	if (ip == NULL)
		return(NODEV);
	if ((ip->i_mode&IFMT) != IFBLK)
		u.u_error = ENOTBLK;
	dev = ip->i_un.i_rdev;
	if (major(dev) >= nblkdev)
		u.u_error = ENXIO;
	iput(ip);
	return(dev);
}

/*
 * Return target name of a symbolic link
 * (In case of AVG_CALLS this may be a server pathname).
 */
readlink()
{
	register struct inode *ip;
	register struct a {
		char    *name;
		char    *buf;
		int     count;
	} *uap = (struct a *)u.u_ap;

	ip = namei(uchar, LOOKUP, 0);
	if (ip == NULL)
		return;
	if ((ip->i_mode&IFMT) != IFLNK) {
#ifdef AVG_CALLS
	if ((ip->i_mode&IFMT) != IFLNK && (ip->i_mode&IFMT) != IFCALL) {
#endif
		u.u_error = ENXIO;
		goto out;
	}
	u.u_offset = 0;
	u.u_base = uap->buf;
	u.u_count = uap->count;
	u.u_segflg = 0;
	readi(ip);
out:
	iput(ip);
	u.u_r.r_val1 = uap->count - u.u_count;
}

/*
 * symlink -- make a symbolic link
 */
symlink()
{
#ifdef AVG_CALLS
	mkSymlink(IFLNK);
}

/*
 * mkcall - make a call entry for server
 */
mkcall()
{
	mkSymlink(IFCALL);
}

/*
 * Common code for symlink and mkcall
 */
mkSymlink(type)
int type;
{
#endif AVG_CALLS
	register struct a {
		char    *target;
		char    *linkname;
	} *uap = (struct a *)u.u_ap;
	register struct inode *ip;
	register char *tp;
	register c, nc;

	tp = uap->target;
	nc = 0;
	while (c = fubyte(tp)) {
		if (c < 0) {
			u.u_error = EFAULT;
			return;
		}
		tp++;
		nc++;
	}
	u.u_dirp = uap->linkname;
	ip = namei(uchar, CREATE, 0);
	if (ip) {
		iput(ip);
		u.u_error = EEXIST;
		return;
	}
	if (u.u_error)
		return;
#ifndef AVG_CALLS
	ip = maknode(IFLNK | 0777);
#else
	ip = maknode(type | 0777);
#endif AVG_CALLS
	if (ip == NULL)
		return;
	u.u_base = uap->target;
	u.u_count = nc;
	u.u_offset = 0;
	u.u_segflg = 0;
	writei(ip);
	iput(ip);
}

#define FsLTOP(dev, b) (b)
/* логич. блоки - в однокилобайтные */

/* выдача информации про свободное место в файловой системе */
ustat(){
	register struct a{
		dev_t dev;
		struct ustat *ubuf;
	} *uap;
	register struct mount *mp;
	register struct filsys *fp;
	struct ustat ust;
#ifdef EXMOUNT
	struct buf *bpf;
#endif
	uap = (struct a *) u.u_ap;
	for( mp=mount; mp<mountNMOUNT; mp++ )
		if( mp->m_inodp != NULL && uap->dev == mp->m_dev ){
#ifndef EXMOUNT
			fp = & mp->m_filsys;
#else
			bpf = mp->m_filsys;
			fp = (struct filsys *) mapin(bpf);
#endif
			ust.f_tfree = FsLTOP(mp->m_dev, fp->s_tfree );
			ust.f_tinode= fp->s_tinode;
			bcopy((caddr_t)fp->s_fsmnt, (caddr_t)&ust.f_fname,
				sizeof( ust.f_fname ));
			bcopy((caddr_t)fp->s_fpack, (caddr_t)&ust.f_fpack,
				sizeof( ust.f_fpack ));
#ifdef EXMOUNT
			mapout( bpf );
#endif
			if( copyout( (caddr_t) &ust , (caddr_t) uap->ubuf,
			    sizeof(ust)))u.u_error = EFAULT;
			return;
		}
	u.u_error = EINVAL;
	return;
}

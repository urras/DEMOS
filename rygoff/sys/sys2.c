/*
 * $Log:	sys2.c,v $
 * Revision 1.4  88/06/29  15:46:26  korotaev
 * Исправлено поле в <sys/inode.h>
 * 
 * Revision 1.3  87/12/12  18:12:11  korotaev
 * Вставлен системный вызов flock.
 * 
 * Revision 1.2  87/09/23  16:04:38  root
 * Изменения связанные с truncate
 *
 * Revision 1.1  86/04/19  15:52:36  avg
 * Initial revision
 *
 */

#include "param.h"
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/file.h>
#include <sys/inode.h>
#include <sys/quota.h>
#ifdef  MENLO_JCL
#include <sys/proc.h>
#endif
#include <sys/inline.h>


/*
 * read system call
 */
read()
{
	rdwr(FREAD);
}

/*
 * write system call
 */
write()
{
	rdwr(FWRITE);
}

/*
 * common code for read and write calls:
 * check permissions, set base, count, and offset,
 * and switch out to readi, writei, or pipe code.
 */
rdwr(mode)
register mode;
{
	register struct file *fp;
	register struct inode *ip;
	register struct a {
		int     fdes;
		char    *cbuf;
		unsigned count;
	} *uap;

	uap = (struct a *)u.u_ap;
#ifdef  UNFAST
	if ((fp = getf(uap->fdes)) == NULL)
		return;
#else
	if (GETF(fp,uap->fdes)) {
		u.u_error = EBADF;
		return;
	}
#endif
	if((fp->f_flag & mode) == 0) {
		u.u_error = EBADF;
		return;
	}
	u.u_base = (caddr_t)uap->cbuf;
	u.u_count = uap->count;
	u.u_segflg = 0;
#ifdef  MENLO_JCL
	if ((u.u_procp->p_flag & SNUSIG) && save(u.u_qsav)) {
		if (u.u_count == uap->count)
			u.u_eosys = RESTARTSYS;
	} else
#endif
#ifdef  UCB_NET
	if (fp->f_flag & FSOCKET) {
		if (mode == FREAD)
			u.u_error = soreceive(fp->f_socket, (struct sockaddr *)0);
		else
			u.u_error = sosend(fp->f_socket, (struct sockaddr *)0);
	} else
#endif
		if((fp->f_flag & FPIPE) != 0) {
			if(mode == FREAD)
				readp(fp);
			else
				writep(fp);
		} else {
			ip = fp->f_inode;
#ifdef  MPX_FILS
			if (fp->f_flag & FMP)
				u.u_offset = 0;
			else
#endif
				u.u_offset = fp->f_un.f_offset;
			if((ip->i_mode & (IFCHR & IFBLK)) == 0)
				plock(ip);
			if(mode == FREAD)
				readi(ip);
			else
				writei(ip);
			if((ip->i_mode & (IFCHR & IFBLK)) == 0)
				prele(ip);
#ifdef  MPX_FILS
			if ((fp->f_flag & FMP) == 0)
#endif
				fp->f_un.f_offset += uap->count-u.u_count;
		}
	u.u_r.r_val1 = uap->count-u.u_count;
}

/*
 * open system call
 */
open()
{
	register struct inode *ip;
	register struct a {
		char    *fname;
		int     rwmode;
	} *uap;

	uap = (struct a *)u.u_ap;
#ifndef UCB_SYMLINKS
	ip = namei(uchar, LOOKUP);
#else
	ip = namei(uchar, LOOKUP, 1);
#endif
	if(ip == NULL)
		return;
	open1(ip, ++uap->rwmode, 0);
}

/*
 * creat system call
 */
creat()
{
	register struct inode *ip;
	register struct a {
		char    *fname;
		int     fmode;
	} *uap;

	uap = (struct a *)u.u_ap;
#ifndef UCB_SYMLINKS
	ip = namei(uchar, CREATE);
#else
	ip = namei(uchar, CREATE, 1);
#endif
	if(ip == NULL) {
		if(u.u_error)
			return;
		ip = maknode(uap->fmode & 07777 & (~ISVTX));
		if (ip==NULL)
			return;
		open1(ip, FWRITE, 2);
	} else
		open1(ip, FWRITE, 1);
}

/*
 * common code for open and creat.
 * Check permissions, allocate an open file structure,
 * and call the device open routine if any.
 */
open1(ip, mode, trf)
register struct inode *ip;
register mode;
{
	register struct file *fp;
	int i;
	dev_t ird;

	if(trf != 2) {
		if(mode & FREAD)
			access(ip, IREAD);
		if(mode & FWRITE) {
			access(ip, IWRITE);
			if((ip->i_mode & IFMT) == IFDIR)
				u.u_error = EISDIR;
		}
	}
	if(u.u_error)
		goto out;
	if(trf == 1)
#ifdef IPK_TRUNCATE
		itrunc(ip,(off_t)0);
#else IPK_TRUNCATE
		itrunc(ip);
#endif IPK_TRUNCATE
	prele(ip);
	if ((fp = falloc()) == NULL)
		goto out;
	fp->f_flag = mode & (FREAD|FWRITE);
	fp->f_inode = ip;
	i = u.u_r.r_val1;
	openi(ip, mode & FWRITE);
	if(u.u_error == 0)
		return;
	u.u_ofile[i] = NULL;
	fp->f_count--;

out:
	iput(ip);
}

/*
 * close system call
 */
close()
{
	register struct file *fp;
	register struct a {
		int     fdes;
	} *uap;

	uap = (struct a *)u.u_ap;
#ifdef  UNFAST
	if ((fp = getf(uap->fdes)) == NULL)
		return;
#else
	if (GETF(fp,uap->fdes)) {
		u.u_error = EBADF;
		return;
	}
#endif
	u.u_ofile[uap->fdes] = NULL;
#ifndef UCB_NET
	closef(fp);
#else
	closef(fp,0);
#endif
}

/*
 * seek system call
 */
seek()
{
	register struct file *fp;
	register struct a {
		int     fdes;
		off_t   off;
		int     sbase;
	} *uap;

	uap = (struct a *)u.u_ap;
#ifdef  UNFAST
	if ((fp = getf(uap->fdes)) == NULL)
		return;
#else
	if (GETF(fp,uap->fdes)) {
		u.u_error = EBADF;
		return;
	}
#endif
#ifdef  UCB_NET
	if (fp->f_flag&FSOCKET) {
		u.u_error = ESPIPE;
		return;
	}
#endif
#ifdef  MPX_FILS
	if(fp->f_flag & (FPIPE|FMP))
#else
	if(fp->f_flag & FPIPE)
#endif
		{
		u.u_error = ESPIPE;
		return;
	}
	if(uap->sbase == FSEEK_RELATIVE)
		uap->off += fp->f_un.f_offset;
	else if(uap->sbase == FSEEK_EOF)
		uap->off += fp->f_inode->i_size;
	fp->f_un.f_offset = uap->off;
	u.u_r.r_off = uap->off;
}

/*
 * link system call
 */
link()
{
	register struct inode *ip, *xp;
	register struct a {
		char    *target;
		char    *linkname;
	} *uap;

	uap = (struct a *)u.u_ap;
#ifndef UCB_SYMLINKS
	ip = namei(uchar, LOOKUP);
#else
	ip = namei(uchar, LOOKUP, 1);
#endif
	if(ip == NULL)
		return;
#ifdef UCB_QUOTAS
	if (((ip->i_mode & IFMT)==IFDIR || isquot(ip)) && !suser())
#else
	if((ip->i_mode & IFMT)==IFDIR && !suser())
#endif
		goto out;

	ip->i_nlink++;
	ip->i_flag |= ICHG;
#ifdef UCB_FSFIX
	iupdat(ip, &time, &time, 1);
#else
	iupdat(ip, &time, &time);
#endif
	prele(ip);
	u.u_dirp = (caddr_t)uap->linkname;
#ifndef UCB_SYMLINKS
	xp = namei(uchar, CREATE);
#else
	xp = namei(uchar, CREATE, 0);
#endif
	if(xp != NULL) {
		u.u_error = EEXIST;
		iput(xp);
	} else {
		if (u.u_error)
			goto err;
		if (u.u_pdir->i_dev != ip->i_dev) {
			iput(u.u_pdir);
			u.u_error = EXDEV;
		} else
			wdir(ip);
	}

	if (u.u_error) {
err:
		ip->i_nlink--;
		ip->i_flag |= ICHG;
	}
out:
	iput(ip);
}

/*
 * mknod system call
 */
mknod()
{
	register struct inode *ip;
	register struct a {
		char    *fname;
		int     fmode;
		dev_t     dev;
	} *uap;

	if(suser()) {
		uap = (struct a *)u.u_ap;
#ifndef UCB_SYMLINKS
		ip = namei(uchar, CREATE);
#else
		ip = namei(uchar, CREATE, 0);
#endif
		if(ip != NULL) {
			u.u_error = EEXIST;
			goto out;
		}
		ip = maknode(uap->fmode);
		if (ip == NULL)
			return;
#ifdef  UCB_FSFIX
		if (uap->dev) {
			ip->i_un.i_rdev = uap->dev;
			ip->i_flag |= IACC|IUPD|ICHG;
		}
#else
		ip->i_un.i_rdev = uap->dev;
#endif
out:
		iput(ip);
	}
}

/*
 * access system call
 */
saccess()
{
	register struct inode *ip;
	register struct a {
		char    *fname;
		int     fmode;
	} *uap;
	register svuid, svgid;

	uap = (struct a *)u.u_ap;
	svuid = u.u_uid;
	svgid = u.u_gid;
	u.u_uid = u.u_ruid;
	u.u_gid = u.u_rgid;
#ifndef UCB_SYMLINKS
	ip = namei(uchar, LOOKUP);
#else
	ip = namei(uchar, LOOKUP, 1);
#endif
	if (ip != NULL) {
		if ((uap->fmode & FACCESS_READ) && access(ip, IREAD))
			goto done;
		if ((uap->fmode & FACCESS_WRITE) && access(ip, IWRITE))
			goto done;
		if ((uap->fmode & FACCESS_EXECUTE) && access(ip, IEXEC))
			goto done;
done:
		iput(ip);
	}
	u.u_uid = svuid;
	u.u_gid = svgid;
}

#ifdef IPK_TRUNCATE

/*
 * Системный вызов ftruncate(fd,length).
 *      обрезает открытый файл.
 */

ftruncate()
{
	register struct file  *fp;
	register struct inode *ip;
	register struct a {
		int fdes;
		off_t length;
	} *uap;

	uap = (struct a * )u.u_ap;
#ifdef UNFAST
	if( (fp =getf(uap->fdes)) == NULL ){
#else
	if( GETF(fp,uap->fdes) ){
#endif
		u.u_error = EBADF;
		return;
	}
	if( (fp->f_flag & FWRITE) == 0 ){
		u.u_error = EINVAL;
		return;
	}
	ip = fp->f_inode;
	plock(ip);
	itrunc(ip,uap->length);
	prele(ip);
}

/*
 * Системный вызов truncate(path,length).
 *      обрезает файл по имени.
 */

truncate()
{
	register struct inode *ip;
	register struct a {
		char *fname;
		off_t length;
	} *uap;

	uap = (struct a * )u.u_ap;
#ifdef UCB_SYMLINKS
	if( (ip =namei(uchar,LOOKUP)) == NULL )
#else
	if( (ip =namei(uchar,LOOKUP,1)) == NULL )
#endif
		return;
	if( access(ip,IWRITE) )
		goto out;
	if( (ip->i_mode & IFMT) != IFREG ){
		u.u_error = EINVAL;
		goto out;
	}
	itrunc(ip,uap->length);
out:
	iput(ip);
}

#endif IPK_TRUNCATE

#ifdef IPK_FLOCK

/*
 * Системный вызов flock - блокирование файла (from BSD4.2).
 */
flock()
{
	register struct file *fp;
	register struct a {
		int fd;
		int how;
	} *uap = (struct a *)u.u_ap;

#ifdef UNFAST
	if( (fp = getf(uap->fd)) == NULL ){
#else
	if( GETF(fp,uap->fd) ){
#endif
		u.u_error = EBADF;
		return;
	}
	if( (fp->f_flag & FWRITE) == 0 ){
		u.u_error = EINVAL;
		return;
	}
	if( uap->how & LOCK_UN ){
		ino_unlock(fp, FSHLOCK|FEXLOCK);
		return;
	}
	if( ((fp->f_flag & FEXLOCK) && (uap->how == LOCK_EX)) ||
	    ((fp->f_flag & FSHLOCK) && (uap->how == LOCK_SH)) )
		return;
	u.u_error = ino_lock(fp, uap->how);
}

#endif IPK_FLOCK

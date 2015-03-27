/*
 * $Log:	sys2.c,v $
 * Revision 22.6  90/11/12  19:19:59  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.5  89/10/11  18:25:34  korotaev
 * Исправлена плюха для mknod'а.
 * 
 * Revision 22.4  89/07/24  17:38:42  korotaev
 * Исправлен flock - аргументом может быть комбинация битов.
 * 
 * Revision 22.3  89/05/03  12:32:26  korotaev
 * Теперь при создании существующего файла с длиной 0 меняется время
 * 
 * Revision 22.2  89/04/12  15:37:35  korotaev
 * "param.h" ==> <sys/param.h>
 * 
 * Revision 22.0  89/03/30  16:43:08  korotaev
 * Begining DEMOS 2.2.
 * 
 * Revision 1.7  89/01/10  13:29:52  korotaev
 * Исправлена ошибка в lseek (ESPIPE).
 * 
 * Revision 1.6  89/01/07  20:59:27  korotaev
 * Слияние с исправлениями насчет FIFO, +fcntl, + разные режимы открытия
 * 
 * (from VAX/UTS SV.2)
 * 
 * Revision 1.5  88/11/02  21:13:47  dvolodin
 * *** empty log message ***
 * 
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

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include "../include/reg.h"
#include <sys/file.h>
#include <sys/inode.h>
#include "../include/quota.h"
#include <sys/proc.h>
#include "../include/inline.h"
#include <sys/fcntl.h>

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
 * set base, count, and offset,
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
	} *uap = (struct a *)u.u_ap;
	int type;

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
	u.u_fmode = fp->f_flag;
	if ((u.u_procp->p_flag & SNUSIG) && save(u.u_qsav)) {
		if (u.u_count == uap->count)
			u.u_eosys = RESTARTSYS;
	} else {
#ifdef  UCB_NET
	if (fp->f_flag & FSOCKET) {
		if (mode == FREAD)
			u.u_error = soreceive(fp->f_socket, (struct sockaddr *)0);
		else
			u.u_error = sosend(fp->f_socket, (struct sockaddr *)0);
	} else {
#endif
		ip = fp->f_inode;
		type = ip->i_mode&IFMT;
		if( type != IFCHR && type != IFBLK ){
			plock(ip);
			if( type == IFIFO )
				fp->f_un.f_offset = 0;
			else if( (u.u_fmode&FAPPEND) && (mode == FWRITE) )
				fp->f_un.f_offset = ip->i_size;
		}
		u.u_offset = fp->f_un.f_offset;
		if(mode == FREAD)
			readi(ip);
		else
			writei(ip);
		if( type != IFCHR && type != IFBLK )
			prele(ip);
		fp->f_un.f_offset += uap->count-u.u_count;
	}
	u.u_r.r_val1 = uap->count-u.u_count;
}

/*
 * open system call
 */
open()
{
	register struct a {
		char	*fname;
		int	mode;
		int	crtmode;
	} *uap = (struct a *)u.u_ap;

	copen(++uap->mode, uap->crtmode);
}

/*
 * creat system call
 */
creat()
{
	register struct a {
		char	*fname;
		int	fmode;
	} *uap = (struct a *)u.u_ap;

	copen(FWRITE|O_CREAT|O_TRUNC, uap->fmode);
}

/*
 * common code for open and creat.
 * Check permissions, allocate an open file structure,
 * and call the device open routine if any.
 */
copen(mode, arg)
register mode;
{
	register struct inode *ip;
	register struct file *fp;
	int i;

	if ((mode&(FREAD|FWRITE)) == 0) {
		u.u_error = EINVAL;
		return;
	}
	if (mode&O_CREAT) {
		ip = namei(uchar, CREATE, 1);
		if (ip == NULL) {
			if (u.u_error)
				return;
			ip = maknode(arg&07777&(~ISVTX));
			if (ip == NULL)
				return;
			mode &= ~O_TRUNC;
		} else {
			if (mode&O_EXCL) {
				u.u_error = EEXIST;
				goto out;
			}
			mode &= ~O_CREAT;
		}
	} else {
		ip = namei(uchar, LOOKUP, 1);
		if (ip == NULL)
			return;
	}
	if (!(mode&O_CREAT)) {
		if (mode&FREAD)
			access(ip, IREAD);
		if (mode&(FWRITE|O_TRUNC)) {
			access(ip, IWRITE);
			if ((ip->i_mode&IFMT) == IFDIR)
				u.u_error = EISDIR;
		}
	}
	if( u.u_error )
		goto out;
	if (mode&O_TRUNC)
		itrunc(ip,(off_t)0);
	prele(ip);
	if( (fp = falloc()) == NULL )
		goto out;
	fp->f_inode = ip;
	fp->f_flag = mode&FMASK;
	i = u.u_r.r_val1;
	openi(ip, mode);
	if (u.u_error == 0)
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
	} *uap = (struct a *)u.u_ap;

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
	} *uap = (struct a *)u.u_ap;
	register struct inode *ip;

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
	ip = fp->f_inode;
	if( (ip->i_mode&IFMT) == IFIFO ){
		u.u_error = ESPIPE;
		return;
	}
	if(uap->sbase == FSEEK_RELATIVE)
		uap->off += fp->f_un.f_offset;
	else if(uap->sbase == FSEEK_EOF)
		uap->off += ip->i_size;
	if( uap->off < 0 )
		u.u_error = EINVAL;
	else {
		fp->f_un.f_offset = uap->off;
		u.u_r.r_off = uap->off;
	}
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
	} *uap = (struct a *)u.u_ap;

	ip = namei(uchar, LOOKUP, 1);
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
	xp = namei(uchar, CREATE, 0);
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
	} *uap = (struct a *)u.u_ap;

	if( (uap->fmode & IFMT) != IFIFO && !suser() )
		return;
	ip = namei(uchar, CREATE, 0);
	if(ip != NULL){
		u.u_error = EEXIST;
		goto out;
	}
	ip = maknode(uap->fmode);
	if (ip == NULL)
		return;
	switch (ip->i_mode&IFMT){
	case IFREG:
	       /*
		* maknode преобразует нулевой тип в регулярный файл.
		* Для нормальной работы badsect надо вписать major и
		* minor. Для данного случая стоит эта проверка.
		*/
		if( uap->fmode != 0 )
			break;
	case IFBLK:
	case IFCHR:
		/*
		 *  Используется badsect'ом.
		 *  (поля i_un.i_rdev и i_un.i_addr[0] совпадают)
		 */
		ip->i_un.i_rdev = (dev_t)uap->dev;
#ifdef  UCB_FSFIX
		ip->i_flag |= IACC|IUPD|ICHG;
#else
		ip->i_flag |= ICHG;
#endif
	}
out:
	iput(ip);
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
	} *uap = (struct a *)u.u_ap;
	int    svuid, svgid;

	svuid = u.u_uid;
	svgid = u.u_gid;
	u.u_uid = u.u_ruid;
	u.u_gid = u.u_rgid;
	ip = namei(uchar, LOOKUP, 1);
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
	} *uap = (struct a * )u.u_ap;

	if( (fp =getf(uap->fdes)) == NULL ){
		u.u_error = EBADF;
		return;
	}
	ip = fp->f_inode;
	if( (fp->f_flag & FWRITE) && (ip->i_mode&IFMT) == IFREG ){
		plock(ip);
		itrunc(ip,uap->length);
		prele(ip);
	} else
		u.u_error = EINVAL;
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
	} *uap = (struct a * )u.u_ap;

	if( (ip = namei(uchar,LOOKUP,1)) == NULL )
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
	/* avoid work... */
	if( ((fp->f_flag & FEXLOCK) && (uap->how & LOCK_EX)) ||
	    ((fp->f_flag & FSHLOCK) && (uap->how & LOCK_SH)) )
		return;
	u.u_error = ino_lock(fp, uap->how);
}

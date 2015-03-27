/*
 * $Log:	rdwri.c,v $
 * Revision 22.3  90/11/12  19:16:11  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.2  89/04/12  15:31:32  korotaev
 * "param.h" ==> <sys/param.h>
 * 
 * Revision 22.0  89/03/30  16:26:19  korotaev
 * Begining DEMOS 2.2.
 * 
 * Revision 1.5  89/01/22  20:54:38  korotaev
 * Теперь O_NDELAY работает на char'ые уст-ва. Перед чтением/записью
 * вызывается cdevsw[].d_select, который говорит о наличии либо чего-то
 * во входной очереди или места в выходной. Обмен разрешается при наличии
 * соотв. возможностей.
 * Ошибка в том, что при записи записывается весь указанный буфер. И если
 * в очереди меньше места, чем указано в write'е, то все равно ожидание
 * будет. Это (в принципе) можно исправить в драйверах.
 * 
 * Revision 1.4  89/01/07  20:50:10  korotaev
 * Слияние с исправлениями насчет FIFO, +fcntl, + разные режимы открытия
 * 
 * (from VAX/UTS SV.2)
 * 
 * Revision 1.3  88/07/15  19:07:28  korotaev
 * Добавлены новые устройства: root(255,0) и swap(255,1).
 * При работе с ними используются текущие rootdev и swapdev.
 * 
 * Revision 1.2  88/06/29  15:50:22  korotaev
 * Изменено поле в inode.h
 * 
 * Revision 1.1  86/04/19  15:51:19  avg
 * Initial revision
 * 
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/inode.h>
#include <sys/dir.h>
#include <sys/user.h>
#include "../include/buf.h"
#include <sys/conf.h>
#include <sys/file.h>

/*
 * Read the file corresponding to
 * the inode pointed at by the argument.
 * The actual read arguments are found
 * in the variables:
 *      u_base          core address for destination
 *      u_offset        byte offset in file
 *      u_count         number of bytes to read
 *      u_segflg        read to kernel/user/user I
 *
 * Правлено на счет FIFO файлов + select.
 */
readi (ip)
register struct inode  *ip;
{
	struct buf     *bp;
	dev_t dev;
	daddr_t lbn, bn;
	off_t diff;
	register        on,
	                n;
	int     type;

	if (u.u_offset < 0) {
		u.u_error = EINVAL;
		return;
	}
	ip -> i_flag |= IACC;
	if (u.u_count == 0)
		return;
	type = ip -> i_mode & IFMT;
	switch (type) {
		case IFCHR: 
			dev = ip -> i_un.i_rdev;
			if (u.u_fmode & FNDELAY &&
			    (*cdevsw[major(dev)].d_select)(dev,FREAD) == 0)
				return;
			(*cdevsw[major (dev)].d_read) (dev);
			break;
		case IFIFO: 
			while (ip -> i_size == 0) {
				if (ip -> i_fwcnt == 0)
					return;
				if (u.u_fmode & FNDELAY)
					return;
				ip -> i_flag |= IFIR;
				prele (ip);
				sleep ((caddr_t) & ip -> i_frcnt, PPIPE);
				plock (ip);
			}
			u.u_offset = ip -> i_frptr;
		case IFBLK: 
		case IFREG: 
		case IFDIR: 
		case IFLNK: 
			if (type == IFBLK) {
				dev = ip -> i_un.i_rdev;
				if (major (dev) == 255)
					dev = (minor (dev) == 0) ? rootdev : swapdev;
			}
			else
				dev = ip -> i_dev;
			do {
				bn = u.u_offset >> BSHIFT;
				on = u.u_offset & BMASK;
				n = MIN ((unsigned) (BSIZE - on), u.u_count);
				if (type != IFBLK) {
					if (type == IFIFO) {
						diff = ip -> i_size;
						lbn = 0;
					}
					else {
						diff = ip -> i_size - u.u_offset;
						lbn = bn;
					}
					if (diff <= 0)
						break;
					if (diff < n)
						n = diff;
					bn = bmap (ip, bn, B_READ);
					if (u.u_error)
						break;
				}
				else
					rablock = bn + 1;
				if (bn < 0) {
					bp = geteblk ();
					clrbuf (bp);
				}
				else
					if (ip -> i_un.i_lastr + 1 == lbn)
						bp = breada (dev, bn, rablock);
					else
						bp = bread (dev, bn);
				ip -> i_un.i_lastr = lbn;
				n = MIN ((unsigned) n, BSIZE - bp -> b_resid);
				if (n != 0) {
					iomove (mapin (bp) + on, n, B_READ);
					mapout (bp);
				}
				if (type == IFIFO) {
					ip -> i_size -= n;
					if (u.u_offset >= PIPSIZ)
						u.u_offset = 0;
					if ((n + on) == BSIZE && ip -> i_size < (PIPSIZ - BSIZE)) {
						bp -> b_flags &= ~B_DELWRI;
						bp -> b_flags |= B_AGE;
					}
				}
				else {
					if ((n + on) == BSIZE || u.u_offset == ip -> i_size)
						bp -> b_flags |= B_AGE;
				}
				brelse (bp);
				if (type == IFIFO && ip -> i_wsel) {
					selwakeup (ip -> i_wsel, ip -> i_flag & ISWCOLL);
					ip -> i_wsel = 0;
					ip -> i_flag &= ~ISWCOLL;
				}
			} while (u.u_error == 0 && u.u_count != 0 && n > 0);
			if (type == IFIFO) {
				if (ip -> i_size)
					ip -> i_frptr = u.u_offset;
				else
					ip -> i_frptr = ip -> i_fwptr = 0;
				if (ip -> i_flag & IFIW) {
					ip -> i_flag &= ~IFIW;
					wakeup ((caddr_t) & ip -> i_fwcnt);
				}
			}
			break;
		default: 
			u.u_error = ENODEV;
	}
}

/*
 * Write the file corresponding to
 * the inode pointed at by the argument.
 * The actual write arguments are found
 * in the variables:
 *	u_base		core address for source
 *	u_offset	byte offset in file
 *	u_count		number of bytes to write
 *	u_segflg	write to kernel/user/user I
 */
writei (ip)
register struct inode  *ip;
{
	struct buf     *bp;
	dev_t dev;
	daddr_t bn;
	register        n,
	                on;
	register        type;
	u_short usave;
#ifdef	UCB_FSFIX
	struct direct  *dp;
#endif

	if (u.u_offset < 0) {
		u.u_error = EINVAL;
		return;
	}
#ifndef	INSECURE
 /* 
  * clear set-uid/gid on any write
  */
	ip -> i_mode &= ~(ISUID | ISGID);
#endif
	ip -> i_flag |= IACC;
	type = ip -> i_mode & IFMT;
	switch (type) {
		case IFCHR: 
			dev = ip -> i_un.i_rdev;
			if (u.u_fmode & FNDELAY &&
			    (*cdevsw[major(dev)].d_select)(dev,FWRITE) == 0)
				return;
			ip -> i_flag |= IUPD | ICHG;
			(*cdevsw[major (dev)].d_write) (dev);
			break;
		case IFIFO: 
	floop: 
			usave = 0;
			while ((u.u_count + ip -> i_size) > PIPSIZ) {
				if (ip -> i_frcnt == 0)
					break;
				if ((u.u_count > PIPSIZ) && (ip -> i_size < PIPSIZ)) {
					usave = u.u_count;
					u.u_count = PIPSIZ - ip -> i_size;
					usave -= u.u_count;
					break;
				}
				if (u.u_fmode & FNDELAY)
					return;
				ip -> i_flag |= IFIW;
				prele (ip);
				sleep ((caddr_t) & ip -> i_fwcnt, PPIPE);
				plock (ip);
			}
			if (ip -> i_frcnt == 0) {
				u.u_error = EPIPE;
				psignal (u.u_procp, SIGPIPE);
				break;
			}
			u.u_offset = ip -> i_fwptr;
		case IFBLK: 
		case IFREG: 
		case IFDIR: 
		case IFLNK: 
			if (type == IFBLK) {
				dev = ip -> i_un.i_rdev;
				if (major (dev) == 255)
					dev = (minor (dev) == 0) ? rootdev : swapdev;
			}
			else
				dev = ip -> i_dev;
			while (u.u_error == 0 && u.u_count != 0) {
				bn = u.u_offset >> BSHIFT;
				on = u.u_offset & BMASK;
				n = MIN ((unsigned) (BSIZE - on), u.u_count);
				if (type != IFBLK) {
#ifdef UCB_QUOTAS
					if ((bn = bmap (ip, bn, B_WRITE)) == 0)
						break;
#else
					bn = bmap (ip, bn, B_WRITE);
#endif
					if (bn < 0)
						break;
				}
				if (n == BSIZE || (type == IFIFO && on == 0 && ip -> i_size < (PIPSIZ - BSIZE)))
					bp = getblk (dev, bn);
				else {
					bp = bread (dev, bn);
				/* 
				 * Tape drivers don't clear buffers on end-of-tape
				 * any longer (clrbuf can't be called from interrupt).
				 */
					if (bp -> b_resid == BSIZE)
						clrbuf (bp);
				}
#ifdef	UCB_FSFIX
				iomove ((dp = (struct direct   *) ((unsigned) mapin (bp) + on)), n, B_WRITE);
#else
				iomove (mapin (bp) + on, n, B_WRITE);
				mapout (bp);
#endif
				if (u.u_error != 0) {
#ifdef	UCB_FSFIX
					mapout (bp);
#endif
					brelse (bp);
				}
				else
					if (u.u_fmode & FSYNC) {
#ifdef	UCB_FSFIX
						mapout (bp);
#endif
						bwrite (bp);
					}
					else {
#ifdef	UCB_FSFIX
						if (type == IFDIR && (dp -> d_ino == 0)) {
							mapout (bp);
						/* 
						 * Writing to clear a directory entry.
						 * Must insure the write occurs before
						 * the inode is freed, or may end up
						 * pointing at a new (different) file
						 * if inode is quickly allocated again
						 * and system crashes.
						 */
							bwrite (bp);
						}
						else {
							mapout (bp);
							if ((u.u_offset & BMASK) == 0 && (type != IFIFO)) {
								bp -> b_flags |= B_AGE;
								bawrite (bp);
							}
							else
								bdwrite (bp);
						}
#else	UCB_FSFIX
						if ((u.u_offset & BMASK) == 0 && type != IFIFO)
							bp -> b_flags |= B_AGE;
						bdwrite (bp);
#endif	UCB_FSFIX
					}
				if (type == IFREG || type == IFDIR || type == IFLNK) {
					if (u.u_offset > ip -> i_size)
						ip -> i_size = u.u_offset;
				}
				else
					if (type == IFIFO) {
						ip -> i_size += n;
						if (u.u_offset >= PIPSIZ)
							u.u_offset = 0;
						if (ip -> i_rsel) {
							selwakeup (ip -> i_rsel, ip -> i_flag & ISRCOLL);
							ip -> i_rsel = 0;
							ip -> i_flag &= ~ISRCOLL;
						}
					}
				ip -> i_flag |= IUPD | ICHG;
			}
			if (type == IFIFO) {
				ip -> i_fwptr = u.u_offset;
				if (ip -> i_flag & IFIR) {
					ip -> i_flag &= ~IFIR;
					wakeup ((caddr_t) & ip -> i_frcnt);
				}
				if (u.u_error == 0 && usave != 0) {
					u.u_count = usave;
					goto floop;
				}
			}
			break;
		default: 
			u.u_error = ENODEV;
	}
}

/*
 * Move n bytes at byte location cp
 * to/from (flag) the user/kernel (u.segflg)
 * area starting at u.base.
 * Update all the arguments by the number
 * of bytes moved.
 *
 * There are 2 algorithms,
 * if source address, dest address and count
 * are all even in a user copy,
 * then the machine language copyin/copyout
 * is called.
 * If not, its done byte by byte with
 * cpass and passc.
 */
iomove (cp, n, flag)
register        caddr_t cp;
register        n;
{
	register        t;

	if (n == 0)
		return;
	if (u.u_segflg != 1 && ((n | (int) cp | (int) u.u_base) & (NBPW - 1)) == 0) {
		if (flag == B_WRITE)
			if (u.u_segflg == 0)
				t = copyin (u.u_base, (caddr_t) cp, n);
			else
				t = copyiin (u.u_base, (caddr_t) cp, n);
		else
			if (u.u_segflg == 0)
				t = copyout ((caddr_t) cp, u.u_base, n);
			else
				t = copyiout ((caddr_t) cp, u.u_base, n);
		if (t) {
			u.u_error = EFAULT;
			return;
		}
		u.u_base += n;
		u.u_offset += n;
		u.u_count -= n;
		return;
	}
	if (flag == B_WRITE) {
		do {
			if ((t = cpass ()) < 0)
				return;
			*cp++ = t;
		} while (--n);
	}
	else
		do {
			if (passc (*cp++) < 0)
				return;
		} while (--n);
}

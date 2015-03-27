/*
 * $Log:	nami.c,v $
 * Revision 22.4  90/11/12  19:14:41  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 22.3  89/05/22  13:18:44  abs
 * вместо ENOENT - EINVAL для имен ""
 * 
 * Revision 22.2  89/05/19  22:08:36  abs
 *
 * Revision 22.1  89/05/19  22:05:43  abs
 * теперь пустое имя файла "" приводит к ошибке NOENT.
 * Раньше зачем-то находился каталог "."
 * Приведено к sV.
 *
 * Revision 22.0  89/04/12  19:42:37  korotaev
 * Begining DEMOS 2.2.
 *
 * Revision 1.9  89/04/12  15:28:21  korotaev
 * "param.h" ==> <sys/param.h>
 *
 * Revision 1.8  89/03/02  18:07:49  abs
 * *** empty log message ***
 *
 * Revision 1.7  89/01/31  20:33:10  abs
 * добавлен флажок LOCKPARENT.
 *
 * Revision 1.6  89/01/10  14:09:39  abs
 * вынес макс. число симлинков в param.h
 *
 * Revision 1.5  89/01/07  20:48:09  korotaev
 * Слияние с исправлениями насчет FIFO, +fcntl, + разные режимы открытия
 *
 * (from VAX/UTS SV.2)
 *
 * Revision 1.4  87/11/03  17:01:17  avg
 * Правлен symchar на предмет русских букв.
 *
 * Revision 1.3  87/05/18  21:17:27  alex
 * исправлена ошибка в работе с i-узлами by @AVG.
 *
 * Revision 1.2  86/06/09  10:33:43  avg
 * Сделана правка для работы с 8-битными символами в именах файлов.
 *
 * Revision 1.1  86/04/19  15:50:45  avg
 * Initial revision
 *
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/inode.h>
#include <sys/filsys.h>
#include "../include/mount.h"
#include <sys/dir.h>
#include <sys/user.h>
#include "../include/buf.h"
#include "../include/quota.h"

#ifndef saveseg5
#include <sys/seg.h>
#endif saveseg5

/*
 * Convert a pathname into a pointer to
 * an inode. Note that the inode is locked.
 *
 * func = function called to get next char of name
 *      &uchar if name is in user space
 *      &schar if name is in system space
 *      flag =  LOOKUP if name is sought
 *              CREATE if name is to be created
 *              DELETE if name is to be deleted
 * follow = 1 if to follow links at end of name
 */

/*
 * Convert a pathname into a pointer to a locked inode,
 * with side effects usable in creating and removing files.
 * This is a very central and rather complicated routine.
 *
 * The func argument gives the routine which returns successive
 * characters of the name to be translated.
 *
 * The flag argument is (LOOKUP, CREATE, DELETE) depending on whether
 * the name is to be (looked up, created, deleted).  If flag has
 * LOCKPARENT or'ed into it and the target of the pathname exists,
 * namei returns both the target and its parent directory locked.
 * If the file system is not maintained in a strict tree hierarchy,
 * this can result in a deadlock situation.  When creating and
 * LOCKPARENT is specified, the target may not be ".".  When deleting
 * and LOCKPARENT is specified, the target may be ".", but the caller
 * must check to insure it does an irele and iput instead of two iputs.
 * Это поскольку первый же iput сделает I-узел unlocked.
 * А нужно это только ПОСЛЕ второго iput-а.
 * Именно поэтому используем irele (see sys5.c), которая
 * уменьшает счетчик ссылок не снимая lock.
 *
 * The follow argument is 1 when symbolic links are to be followed
 * when they occur at the end of the name translation process.
 */

/* Из всего вышенаписанного про LOCKPARENT пока задействовано
 * лишь с DELETE
 */

struct inode *
namei(func, flag, follow)
int (*func)();
{
	register struct direct *dirp;
	struct inode *dp;
	register c;
	register char *cp;
	struct buf *bp;
	struct buf *temp;
	int nlink;
	int i;
	dev_t d;
	off_t eo;
	int lockparent;

	nlink = 0;
	u.u_sbuf = 0;

	lockparent = flag & LOCKPARENT;
	flag &= ~LOCKPARENT;

	c = (*func)();

	/* вызовы  open( "", rw ), stat("") должны отсылать */
	if( !c ){
		u.u_error = EINVAL;
		return;
	}

	/*
	 * If name starts with '/' start from
	 * root; otherwise start from current dir.
	 */

	/* get starting directory */
	dp = u.u_cdir;
	if(c == '/')
		if ((dp = u.u_rdir) == NULL)
			dp = rootdir;
	iget(dp->i_dev, dp->i_number);
	/* или plock( dp );  dp-> i_count++; */
	while(c == '/')
		c = (*func)();

	/* попытка удалить или создать "/" */
	if( !c && flag != LOOKUP )
		u.u_error = EINVAL;

/* в 4.2 BSD эта метка называется dirloop,
 * а метка out    - bad
 */

cloop:
	/*
	 * Here dp contains pointer
	 * to last component matched.
	 */

	if(u.u_error)
		goto out;
	if(c == '\0'){
		/* возврат результата из LOOKUP и CREATE */
		if( flag == DELETE || lockparent ){
			u.u_error = EISDIR;
			goto out;
		}
		return(dp);
	}

	/*
	 * If there is another component,
	 * Gather up name into
	 * users' dir buffer.
	 */

	cp = &u.u_dbuf[0];
	while (c != '/' && c != '\0' && u.u_error == 0 ) {
		if(cp < &u.u_dbuf[DIRSIZ])
			*cp++ = c;
		c = (*func)();
	}
	while(cp < &u.u_dbuf[DIRSIZ])
		*cp++ = '\0';
	while(c == '/')
		c = (*func)();

	/*
	 * dp must be a directory and
	 * must have X permission.
	 */

	access(dp, IEXEC);
seloop:
	if((dp->i_mode&IFMT) != IFDIR)
		u.u_error = ENOTDIR;
	if(u.u_error)
		goto out;

	/*
	 * set up to search a directory
	 */
	u.u_offset = 0;
	u.u_segflg = 1;
	eo = 0;
	bp = NULL;

	if (dp == u.u_rdir && u.u_dbuf[0] == '.' &&
	    u.u_dbuf[1] == '.' && u.u_dbuf[2] == 0)
		goto cloop;
eloop:

	/*
	 * If at the end of the directory,
	 * the search failed. Report what
	 * is appropriate as per flag.
	 */

	if(u.u_offset >= dp->i_size) {
		if(bp != NULL) {
			mapout(bp);
			brelse(bp);
		}
		if(flag==CREATE && c=='\0') {
			if(access(dp, IWRITE))
				goto out;
			u.u_pdir = dp;
			/* в каталоге не было такого имени -
			 * в u_pdir возвращаем locked inode
			 * каталога.
			 *   return NULL;
			 */
			if(eo)
				u.u_offset = eo-sizeof(struct direct);
			else
				dp->i_flag |= IUPD|ICHG;
			goto out1;
		}
		u.u_error = ENOENT;
		goto out;
	}

	/*
	 * If offset is on a block boundary,
	 * read the next directory block.
	 * Release previous if it exists.
	 */

	if((u.u_offset&BMASK) == 0) {
		if(bp != NULL) {
			mapout(bp);
			brelse(bp);
		}
		bp = bread(dp->i_dev,
			bmap(dp, (daddr_t)(u.u_offset>>BSHIFT), B_READ));
		if (bp->b_flags & B_ERROR) {
			brelse(bp);
			goto out;
		}
		dirp = (struct direct *)mapin(bp);
	}

	/*
	 * Note first empty directory slot
	 * in eo for possible creat.
	 * String compare the directory entry
	 * and the current component.
	 * If they do not match, go back to eloop.
	 */

	u.u_offset += sizeof(struct direct);
	if(dirp->d_ino == 0) {
		dirp++;
		if(eo == 0)
			eo = u.u_offset;
		goto eloop;
	}
#ifdef UCB_QUOTAS
	/*
	 * See if this could be a quota node.
	 */
	if((dirp->d_name[0] == '.') &&
	   (dirp->d_name[1] == 'q') &&
	   (dirp->d_name[2] == '\0'))
	{
		cp = dp->i_quot;
		/*
		 * If no quota is associated yet or a new quot is
		 * around, then . . .
		 */
		if (cp == NULL || cp->i_number != dirp->d_ino) {
			u.u_dent.d_ino = dirp->d_ino;
			mapout(bp);
			cp = iget(dp->i_dev, u.u_dent.d_ino);
			if (cp != NULL) {
				prele(cp);
				/*
				 * If not really a quota node then just put away
				 */
				if (!isquot(cp)) {
					iput(cp);
					cp = NULL;
				}
			}
			/*
			 * The value of dirp is still valid because
			 * the buffer can not have been released
			 * between the mapout() above and here,
			 * and there is a static relationship between
			 * buffer headers and the buffers proper.
			 */
			mapin(bp);
			if (cp != NULL) {
				/*
				 * set up hierarchical inode chains
				 * NOTE: this is done wrong since this may
				 *       overwrite an inode which has not
				 *       been put away yet
				 */
				cp->i_quot = dp->i_quot;
				dp->i_quot = cp;
			}
		}
		if (cp != NULL) {
			/*
			 * Mark the directory as being the original
			 * owner of the quota.  This is necessary so
			 * that quotas do not get copied up the tree.
			 */
			dp->i_flag |= IQUOT;

		}
	}
#endif UCB_QUOTAS
	for(i=0; i<DIRSIZ; i++) {
		if(u.u_dbuf[i] != dirp->d_name[i])
		{
			dirp++;
			goto eloop;
		}
		if (u.u_dbuf[i] == '\0')
			break;
	}
	u.u_dent = *dirp;
	/*
	 * Here a component matched in a directory.
	 * If there is more pathname, go back to
	 * cloop, otherwise return.
	 */

	if(bp != NULL) {
		mapout(bp);
		brelse(bp);
	}
	if(flag==DELETE && c=='\0') {
		if(access(dp, IWRITE))
			goto out;
		u.u_pdir = dp;  /* locked inode каталога */
		if( lockparent ){
			if( dp-> i_number == u.u_dent.d_ino )
			/* каталог "." */
				dp-> i_count ++;
			else{
				dp= iget( dp-> i_dev, u.u_dent.d_ino );
				if( dp == NULL ){
					iput( u.u_pdir );
					goto out;
				}
			}
		}
		return(dp);
	}
	d = dp->i_dev;
	if ((u.u_dent.d_ino == ROOTINO) && (dp->i_number == ROOTINO)
	    && (u.u_dent.d_name[1] == '.'))
		for(i=1; i<nmount; i++)
			if ((mount[i].m_inodp != NULL)
			    && (mount[i].m_dev == d)) {
				iput(dp);
				dp = mount[i].m_inodp;
				dp->i_count++;
				plock(dp);
				/*
				 * Note: permission for ROOTINO already checked.
				 */
				goto seloop;
			}
	prele(dp);
	temp = cp = iget(d, u.u_dent.d_ino);
	if (cp == NULL) {
		if (dp->i_flag & ILOCK)
			dp->i_count--;
		else
			iput(dp);
		goto out1;
	}
	if ((((struct inode *)temp)->i_mode&IFMT)==IFLNK && (follow || c)) {
		struct inode *pdp;

		pdp = (struct inode *)temp;
		if (pdp->i_size >= BSIZE-2 || ++nlink > NSYMLNK || u.u_sbuf || !pdp->i_size) {
			u.u_error = ELOOP;
			iput(pdp);
			goto out;
		}
		u.u_sbuf = bread(pdp->i_dev, bmap(pdp, (daddr_t)0, B_READ));
		if (u.u_sbuf->b_flags & B_ERROR) {
			brelse(u.u_sbuf);
			iput(pdp);
			u.u_sbuf = 0;
			goto out;
		}
		/* Save our readahead chars at end of buffer, get first */
		/* symbolic link character */
		{
			segm save5;
			char *cp;

			if (c)          /* space for readahead chars */
				u.u_slength = pdp->i_size+2;
			else    u.u_slength = pdp->i_size+1;
			u.u_soffset = 0;
			saveseg5(save5);
			mapin(u.u_sbuf);
			cp = (char *)SEG5;
			if (c)
				cp[u.u_slength-2] = '/';
			cp[u.u_slength-1] = c;
			c = cp[u.u_soffset++];
			mapout(u.u_sbuf);
			restorseg5(save5);
		}

		/* Grab the top-level inode for the new path */
		iput(pdp);
		if (c == '/') {
			iput(dp);
			if ((dp = u.u_rdir) == NULL)
				dp = rootdir;
			while (c == '/')
				c = (*func)();
			iget(dp->i_dev, dp->i_number);
		}
		else    plock(dp);
		goto cloop;
	}
#ifndef UCB_QUOTAS
	else {
		if (dp->i_flag & ILOCK)
			dp->i_count--;
		else
			iput(dp);
		dp = (struct inode *)temp;
	}
#else UCB_QUOTAS
	/*
	 * Make sure not to copy the quota node up the tree past
	 * the original height.
	 */
	if ((dp->i_flag & IQUOT) && u.u_dent.d_name[0] == '.'
	   && u.u_dent.d_name[1] == '.' && u.u_dent.d_name[2] == '\0')
		cp = dp->i_quot;
	/*
	 * Copy quota to new inode
	 */
	qcopy(dp, cp);
	if (dp->i_flag & ILOCK)
		dp->i_count--;
	else
		iput(dp);
	dp = temp;
#endif UCB_QUOTAS
	goto cloop;

out:     /* облом */
	if( dp ) iput(dp);
out1:    /* сюда посылает CREATE, если имя не существовало в каталоге */
	if (u.u_sbuf) {
		brelse(u.u_sbuf);
		u.u_sbuf = u.u_slength = u.u_soffset = 0;
	}
	return(NULL);
}

#ifdef UCB_QUOTAS
/*
 * Copy quota from dp to ip if certain conditions hold.
 */
qcopy(dp, ip)
register struct inode *dp, *ip;
{
	register struct inode *qp;

	qp = dp->i_quot;
	if (qp == NULL || qp == ip)
		return;
	if (ip->i_quot != NULL)
		return;
	ip->i_quot = qp;
	if (++(qp->i_count) == 0)
		panic ("qcopy");
}
#endif

/*
 * Return the next character from the
 * kernel string pointed at by dirp.
 */
schar()
{
	register c;

	if (u.u_sbuf) {
		c = symchar();
		if (c >= 0)
			return(c);
	}
	return(*u.u_dirp++ & 0377);
}

/*
 * Return the next character from the
 * user string pointed at by dirp.
 */
uchar()
{
	register c;

	if (u.u_sbuf) {
		c = symchar();
		if (c >= 0)
			return(c);
	}
	c = fubyte(u.u_dirp++);
	if(c == -1)
		u.u_error = EFAULT;
	else if ( (c&0300) == 0200 )    /* for U-code file names : @VG */
		u.u_error = EINVAL;
	return(c);
}

/*
 *      Get a character from the symbolic name buffer
 */
symchar()
{
	segm save5;
	register c;
	register char *cp;

	if (!u.u_sbuf)          /* Protect ourselves */
		return(-1);
	if (u.u_soffset > u.u_slength) {
		brelse(u.u_sbuf);
		u.u_soffset = u.u_slength = u.u_sbuf = 0;
		return(-1);
	}

	/* Get next character from symbolic link buffer */
	saveseg5(save5);
	mapin(u.u_sbuf);
	cp = (char *)SEG5;
	c = cp[u.u_soffset++] & 0377;   /* because of sign extention */
	mapout(u.u_sbuf);
	restorseg5(save5);
	if (u.u_soffset >= u.u_slength) {
		brelse(u.u_sbuf);
		u.u_soffset = u.u_slength = u.u_sbuf = 0;
	}
	return(c);
};      /* end of symchar */

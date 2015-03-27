/*
 * $Log:	alloc.c,v $
 * Revision 1.4  88/01/21  12:06:48  korotaev
 * При передаче адреса не быд поставлен '&'.
 * 
 * Revision 1.3  87/04/26  17:32:20  avg
 * Правлена ошибка, вызывавшая ALLOCATED INODE(S) IN FREE LIST.
 *
 * Revision 1.2  87/04/18  13:25:49  avg
 * Сделан EXMOUNT.
 *
 * Revision 1.1  86/04/19  15:49:13  avg
 * Initial revision
 *
 */

#include "param.h"
#include <sys/systm.h>
#include <sys/filsys.h>
#include <sys/mount.h>
#include <sys/fblk.h>
#include <sys/conf.h>
#include <sys/buf.h>
#include <sys/inode.h>
#include <sys/ino.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/quota.h>

typedef struct fblk *FBLKP;

#ifdef UCB_QUOTAS
/*
 * Bookkeeping for quota system interfaces to alloc() and free():
 * count one block against the quota of the given inode.
 * If the quota has been exceeded, return NULL without allocating block.
 */
struct buf *
qalloc(ip, d)
register struct inode *ip;
dev_t d;
{
	register struct inode *qp, *rp;
	struct buf *ret;

	for (qp = ip->i_quot; qp != (struct inode *) NULL; qp = qp->i_quot) {
		if (qp->i_un.i_qused >= qp->i_un.i_qmax) {
			/*
			 * A quota was exceeded. Undo the counts.
			 */
			for (rp = ip->i_quot; rp != qp; rp = rp->i_quot)
				rp->i_un.i_qused -= QCOUNT;
			u.u_error = EQUOT;
			return((struct buf *) NULL);
		}
		qp->i_un.i_qused += QCOUNT;
		qp->i_flag |= IUPD;
	}
	if((ret = alloc(d)) != NULL)
		return(ret);
	/*
	 * The block could not be allocated.
	 * Undo the quota count.
	 */
	for (qp = ip->i_quot; qp != (struct inode *) NULL; qp = qp->i_quot)
		if(qp->i_un.i_qused != 0)
			qp->i_un.i_qused -= QCOUNT;
	return((struct buf *) NULL);
}
#endif

/*
 * alloc will obtain the next available
 * free disk block from the free list of
 * the specified device.
 * The super block has up to NICFREE remembered
 * free blocks; the last of these is read to
 * obtain NICFREE more . . .
 *
 * no space on dev x/y -- when
 * the free list is exhausted.
 */
struct buf *
alloc(dev)
dev_t dev;
{
	daddr_t bno;
	register struct filsys *fp;
	register struct buf *bp;
	int     i;
	register struct fblk *fbp;
	struct filsys *fps;
#ifdef EXMOUNT
	struct buf *bpf;

	if ((bpf = getfs(dev)) == NULL)
		goto nofs;
	fp = (struct filsys *)mapin(bpf);
	while(fp->s_flock) {
		mapout(bpf);
		sleep((caddr_t)bpf+1, PINOD);
		fp = (struct filsys *)mapin(bpf);
	}
#else
	if ((fp = getfs(dev)) == NULL)
		goto nofs;
	while(fp->s_flock)
		sleep((caddr_t)&fp->s_flock, PINOD);
#endif EXMOUNT
	do {
		if(fp->s_nfree <= 0)
			goto nospace;
		if (fp->s_nfree > NICFREE) {
			fserr(fp, "bad free count");
			goto nospace;
		}
		bno = fp->s_free[--fp->s_nfree];
		if(bno == 0)
			goto nospace;
	} while (badblock(fp, bno));
	if(fp->s_nfree <= 0) {
		fp->s_flock++;
#ifdef EXMOUNT
		mapout(bpf);
#endif
		bp = bread(dev, bno);
		if (((bp->b_flags&B_ERROR) == 0) && (bp->b_resid==0)) {
#ifndef EXMOUNT
			fbp = (FBLKP) mapin(bp);
			*((FBLKP)&fp->s_nfree) = *fbp;
			mapout(bp);
#else
			copybb( bp, bpf, 0, (char*)&(fp->s_nfree) - (char*)fp,
					    sizeof(struct fblk)/sizeof(int) );
#endif EXMOUNT
		}
		brelse(bp);
#ifdef  UCB_FSFIX
		/*
		 * Write the superblock back, synchronously,
		 * so that the free list pointer won't point at garbage.
		 * We can still end up with dups in free if we then
		 * use some of the blocks in this freeblock, then crash
		 * without a sync.
		 */
#ifdef EXMOUNT
		fp = (struct filsys *)mapin(bpf);
		fp->s_fmod = 0;
		fp->s_time = time;
		mapout(bpf);
		bp = getblk(dev, SUPERB);
		copybb( bpf, bp, 0, 0, sizeof(struct filsys)/sizeof(int) );
#else
		fp->s_fmod = 0;
		fp->s_time = time;
		bp = getblk(dev, SUPERB);
		fps = (struct filsys *) mapin(bp);
		*fps = *fp;
		mapout(bp);
#endif EXMOUNT
		bwrite(bp);
#endif UCB_FSFIX

#ifdef EXMOUNT
		fp = (struct filsys *)mapin(bpf);
		fp->s_flock = 0;
		wakeup((caddr_t)bpf+1);
#else
		fp->s_flock = 0;
		wakeup((caddr_t)&fp->s_flock);
#endif EXMOUNT
		if (fp->s_nfree <=0)
			goto nospace;
	}
#ifdef EXMOUNT
	mapout(bpf);
#endif
	bp = getblk(dev, bno);
	clrbuf(bp);
#ifdef EXMOUNT
	fp = (struct filsys *)mapin(bpf);
#endif EXMOUNT
	fp->s_fmod = 1;
	fp->s_tfree--;
#ifdef EXMOUNT
	mapout(bpf);
#endif
	return(bp);

nospace:
	fp->s_nfree = 0;
	fp->s_tfree = 0;
	fserr(fp, "file system full");
	/* THIS IS A KLUDGE... */
	/* SHOULD RATHER SEND A SIGNAL AND SUSPEND THE PROCESS IN A */
	/* STATE FROM WHICH THE SYSTEM CALL WILL RESTART */
#ifdef  UCB_UPRINTF
#ifdef EXMOUNT
	{       char fsnm[34], *q, *p;
		p = fp->s_fsmnt;
		q = fsnm;
		while( *q++ = *p++ );
		mapout(bpf);
		uprintf("\n%s: write failed, file system is full\n", fsnm);
	}
#else
	uprintf("\n%s: write failed, file system is full\n", fp->s_fsmnt);
#endif EXMOUNT
#else
#ifdef EXMOUNT
	mapout(bpf);
#endif
#endif  UCB_UPRINTF

	for (i = 0; i < 5; i++)
		sleep((caddr_t)&lbolt, PRIBIO);
nofs:
	u.u_error = ENOSPC;
	return(NULL);
}

#ifdef UCB_QUOTAS
/*
 *      decrement the count for quotas
 */
qfree(ip, b)
register struct inode *ip;
daddr_t b;
{
	register struct inode *qp;

	for (qp = ip->i_quot; qp != NULL; qp = qp->i_quot)
		if (qp->i_un.i_qused) {
			qp->i_un.i_qused -= QCOUNT;
			qp->i_flag |= IUPD;
		}
	free(ip->i_dev, b);
}
#endif

/*
 * place the specified disk block
 * back on the free list of the
 * specified device.
 */
free(dev, bno)
dev_t dev;
daddr_t bno;
{
	register struct filsys *fp;
	register struct buf *bp;
	register struct fblk *fbp;
#ifdef EXMOUNT
	struct  buf *bpf;

	if ((bpf = getfs(dev)) == NULL)
		return;
	fp = (struct filsys *)mapin(bpf);
	if (badblock(fp, bno)) {
		mapout(bpf);
		return;
	}
	while(fp->s_flock) {
		mapout(bpf);
		sleep((caddr_t)bpf+1, PINOD);
		fp = (struct filsys *)mapin(bpf);
	}
#else
	if ((fp = getfs(dev)) == NULL)
		return;
	if (badblock(fp, bno))
		return;
	while(fp->s_flock)
		sleep((caddr_t)&fp->s_flock, PINOD);
#endif EXMOUNT
	if(fp->s_nfree <= 0) {
		fp->s_nfree = 1;
		fp->s_free[0] = 0;
	}
	if(fp->s_nfree >= NICFREE) {
		fp->s_flock++;
#ifndef EXMOUNT
		bp = getblk(dev, bno);
		fbp = (FBLKP) mapin(bp);
		*fbp = *((FBLKP) &fp->s_nfree);
		mapout(bp);
		fp->s_nfree = 0;
		bwrite(bp);
		fp->s_flock = 0;
		wakeup((caddr_t)&fp->s_flock);
#else
		mapout(bpf);
		bp = getblk(dev, bno);
		copybb( bpf, bp, (char*)&(fp->s_nfree) - (char*)fp,  0,
				  sizeof(struct fblk)/sizeof(int) );
		fp = (struct filsys *)mapin(bpf);
		fp->s_nfree = 0;
		mapout(bpf);    /* bwrite can sleep */
		bwrite(bp);
		fp = (struct filsys *)mapin(bpf);
		fp->s_flock = 0;
		wakeup((caddr_t)bpf+1);
#endif EXMOUNT
	}
	fp->s_free[fp->s_nfree++] = bno;
	fp->s_tfree++;
	fp->s_fmod = 1;
#ifdef EXMOUNT
	mapout(bpf);
#endif
}

/*
 * Check that a block number is in the
 * range between the I list and the size
 * of the device.
 * This is used mainly to check that a
 * garbage file system has not been mounted.
 *
 * bad block on dev x/y -- not in range
 */
badblock(fp, bn)
register struct filsys *fp;
daddr_t bn;
{

	if (bn < fp->s_isize || bn >= fp->s_fsize) {
		fserr(fp, "bad block");
		return(1);
	}
	return(0);
}

/*
 * Allocate an unused I node
 * on the specified device.
 * Used with file creation.
 * The algorithm keeps up to
 * NICINOD spare I nodes in the
 * super block. When this runs out,
 * a linear search through the
 * I list is instituted to pick
 * up NICINOD more.
 */
struct inode *
ialloc(dev)
dev_t dev;
{
	register struct filsys *fp;
	register struct buf *bp;
	register struct inode *ip;
	int i;
	struct dinode *dp;
	ino_t ino;
	daddr_t adr, Isiz;
	ino_t inobas;
	int first;
	int         j;
#ifdef EXMOUNT
	struct buf *bpf;

	if ((bpf = getfs(dev)) == NULL)
		goto nofs;
	fp = (struct filsys *)mapin(bpf);
Wloop:
	while(fp->s_ilock) {
		mapout(bpf);
		sleep((caddr_t)bpf+3, PINOD);
		fp = (struct filsys *)mapin(bpf);
	}
#else
	if ((fp = getfs(dev)) == NULL)
		goto nofs;
Wloop:
	while(fp->s_ilock)
		sleep((caddr_t)&fp->s_ilock, PINOD);
#endif EXMOUNT
loop:
	if(fp->s_ninode > 0) {
		ino = fp->s_inode[--fp->s_ninode];
		if (ino <= ROOTINO)
			goto loop;
#ifdef EXMOUNT
		mapout(bpf);
#endif
		ip = iget(dev, ino);

		if(ip == NULL)
			return(NULL);
		if(ip->i_mode == 0) {
			for (i=0; i<NADDR; i++)
				ip->i_un.i_addr[i] = 0;
#ifdef EXMOUNT
			fp = (struct filsys *)mapin(bpf);
#endif
			fp->s_fmod = 1;
			fp->s_tinode--;
#ifdef EXMOUNT
			mapout(bpf);
#endif
			return(ip);
		}
		/*
		 * Inode was allocated after all.
		 * Look some more.
		 */
		iput(ip);
#ifdef EXMOUNT
		fp = (struct filsys *)mapin(bpf);
#endif
		goto Wloop;
	}
	fp->s_ilock++;
	if (fp->s_nbehind < 4 * NICINOD) {
		first = 1;
		ino = fp->s_lasti;
#ifdef  DIAGNOSTIC
		if (itoo(ino))
			panic("ialloc");
#endif
		adr = itod(ino);
	} else {
fromtop:
		first = 0;
		ino = 1;
		adr = SUPERB+1;
		fp->s_nbehind = 0;
	}
	Isiz = fp->s_isize;
	j = fp->s_ninode;
#ifdef EXMOUNT
	mapout(bpf);
#endif
	for(; adr < Isiz; adr++) {
		inobas = ino;
		bp = bread(dev, adr);
		if ((bp->b_flags & B_ERROR) || bp->b_resid) {
			brelse(bp);
			ino += INOPB;
			continue;
		}
		dp = (struct dinode *) mapin(bp);
		for(i=0; i<INOPB; i++) {
			if(dp->di_mode != 0)
				goto cont;
#ifdef  UCB_IHASH
			if(ifind(dev, ino))
				goto cont;
#else
			for(ip = inode; ip < inodeNINODE; ip++)
				if(dev==ip->i_dev && ino==ip->i_number)
					goto cont;
#endif
#ifndef EXMOUNT
			fp->s_inode[fp->s_ninode++] = ino;
			if(fp->s_ninode >= NICINOD)
				break;
#else EXMOUNT
			mapout(bp);
			fp = (struct filsys *)mapin(bpf);
			fp->s_inode[fp->s_ninode++] = ino;
			j = fp->s_ninode;
			mapout(bpf);
			(void) mapin(bp); /* использует то, что dp уже указывает куда надо */
			if(j >= NICINOD)
				break;
#endif EXMOUNT
		cont:
			ino++;
			dp++;
		}
		mapout(bp);
		brelse(bp);
#ifdef EXMOUNT
		if(j >= NICINOD)
#else
		if(fp->s_ninode >= NICINOD)
#endif
			break;
	}
#ifdef EXMOUNT
	fp = (struct filsys *)mapin(bpf);
#endif
	if (fp->s_ninode < NICINOD && first) {
		goto fromtop;
	}
	fp->s_lasti = inobas;
	fp->s_ilock = 0;
#ifdef EXMOUNT
	wakeup((caddr_t)bpf+3);
#else
	wakeup((caddr_t)&fp->s_ilock);
#endif
	if(fp->s_ninode > 0)
		goto loop;
	fserr(fp, "out of inodes");
#ifdef UCB_UPRINTF
#ifdef EXMOUNT
	{       char fsnm[34], *q, *p;
		p = fp->s_fsmnt;
		q = fsnm;
		while( *q++ = *p++ );
		mapout(bpf);
		uprintf("\n%s: create failed, no inodes free\n", fsnm);
	}
#else
	uprintf("\n%s: create failed, no inodes free\n", fp->s_fsmnt);
#endif EXMOUNT
#else
#ifdef EXMOUNT
	mapout(bpf);
#endif
#endif UCB_UPRINTF

nofs:
	u.u_error = ENOSPC;
	return(NULL);
}

/*
 * Free the specified I node
 * on the specified device.
 * The algorithm stores up
 * to NICINOD I nodes in the super
 * block and throws away any more.
 */
ifree(dev, ino)
dev_t dev;
ino_t ino;
{
	register struct filsys *fp;
#ifdef EXMOUNT
	register struct buf *bpf;

	if ((bpf = getfs(dev)) == NULL)
		return;
	fp = (struct filsys *)mapin(bpf);
#else
	if ((fp = getfs(dev)) == NULL)
		return;
#endif EXMOUNT
	fp->s_tinode++;
	if(fp->s_ilock)
		goto ret;
	if(fp->s_ninode >= NICINOD) {
		if (fp->s_lasti > ino)
			fp->s_nbehind++;
		goto ret;
	}
	fp->s_inode[fp->s_ninode++] = ino;
	fp->s_fmod = 1;
ret:
	;
#ifdef EXMOUNT
	mapout(bpf);
#endif
}

/*
 * getfs maps a device number into
 * a pointer to the incore super
 * block.
 * Если в параметрах генерации был указан EXMOUNT,
 * getfs возвращает указатель на buf, по которому
 * необходимо выполнить mapin.
 * The algorithm is a linear
 * search through the mount table.
 * A consistency check of the
 * in core free-block and i-node
 * counts.
 *
 * bad count on dev x/y -- the count
 *      check failed. At this point, all
 *      the counts are zeroed which will
 *      almost certainly lead to "no space"
 *      diagnostic
 * The previous ``panic:  no fs'', which occurred
 * when a device was not found in the mount table,
 * has been replaced with a warning message.  This
 * is because a panic would occur if one forgot to
 * mount the pipedev.
 * NULL is now returned instead, and routines which
 * previously thought that they were guaranteed a
 * valid pointer from getfs (alloc, free, ialloc,
 * ifree, access, iupdat) have been modified to
 * detect this.
 */
#ifndef EXMOUNT
struct filsys *
#else
struct buf *
#endif EXMOUNT
getfs(dev)
dev_t dev;
{
	register struct mount  *mp;
	register struct filsys *fp;
	register struct buf    *bp;

	for(mp = mount; mp < mountNMOUNT; mp++)
	if(mp->m_inodp != NULL && mp->m_dev == dev) {
#ifdef EXMOUNT
		bp = mp->m_filsys;
		fp = (struct filsys *)mapin(bp);
#else
		fp = &mp->m_filsys;
#endif EXMOUNT
		if(fp->s_nfree > NICFREE || fp->s_ninode > NICINOD) {
			fserr(fp, "bad count");
			fp->s_nfree = 0;
			fp->s_ninode = 0;
		}
#ifdef EXMOUNT
		mapout(bp);
		return(bp);
#else
		return(fp);
#endif EXMOUNT
	}
	printf("no fs on dev %u/%u\n", major(dev), minor(dev));
#ifdef EXMOUNT
	return((struct buf *) NULL);
#else
	return((struct filsys *) NULL);
#endif EXMOUNT
}

/*
 * Update is the internal name of
 * 'sync'. It goes through the disk
 * queues to initiate sandbagged IO;
 * goes through the I nodes to write
 * modified nodes; and it goes through
 * the mount table to initiate modified
 * super blocks.
 */
update()
{
	register struct inode *ip;
	register struct mount *mp;
	register struct buf *bpf;
	struct buf *bp;
	struct filsys *fp, *fpdst;

	if(updlock)
		return;
	updlock++;
	for(mp = mount; mp < mountNMOUNT; mp++)
		if(mp->m_inodp != NULL) {
#ifndef EXMOUNT
			fp = &mp->m_filsys;
#else
			bpf = mp->m_filsys;
			fp = (struct filsys *)mapin(bpf);
#endif EXMOUNT
			if(fp->s_fmod==0 || fp->s_ilock!=0 ||
			   fp->s_flock!=0 || fp->s_ronly!=0) {
#ifdef EXMOUNT
				mapout(bpf);
#endif
				continue;
			}
#ifdef EXMOUNT
			mapout(bpf);
#endif
			bp = getblk(mp->m_dev, SUPERB);
			if (bp->b_flags & B_ERROR)
				continue;
#ifndef EXMOUNT
			fp->s_fmod = 0;
			fp->s_time = time;
			fpdst = (struct filsys *) mapin(bp);
			*fpdst = *fp;
			mapout(bp);
#else
			fp = (struct filsys *)mapin(bpf);
			fp->s_time = time;
			fp->s_fmod = 0;
			mapout(bpf);
			copybb( bpf, bp, 0, 0, sizeof(struct filsys)/sizeof(int) );
#endif EXMOUNT
			bwrite(bp);
		}
	for(ip = inode; ip < inodeNINODE; ip++)
		if((ip->i_flag&ILOCK)==0 && ip->i_count) {
			ip->i_flag |= ILOCK;
			ip->i_count++;
#ifdef  UCB_FSFIX
			iupdat(ip, &time, &time, 0);
#else
			iupdat(ip, &time, &time);
#endif
			iput(ip);
		}
	updlock = 0;
	bflush(NODEV);
}

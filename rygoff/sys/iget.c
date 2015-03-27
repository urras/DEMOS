/*
 * $Log:	iget.c,v $
 * Revision 1.9  88/03/25  19:45:52  korotaev
 * Исправлена ошибка в itrunc при включенном IPK_TRUNCATE.
 * Ошибка заключалась в том, что при освобождении i-узла занятого
 * под PIPE блоки этого i-узла не освобождались т.к. длина в i-узле
 * указана нулевая.
 * 
 * Revision 1.8  88/01/21  18:14:37  korotaev
 * Изменения с Э-85.
 * 
 * Revision 1.7  87/09/23  16:01:17  root
 * Изменения связанные с truncate
 * 
 * Revision 1.6  87/06/19  17:47:11  avg
 * Поправлена плюшка of alex.
 *
 * Revision 1.5  87/04/18  13:29:31  avg
 * Правки с кэш-ем по i-узлам by alex.
 *
 * Revision 1.4  87/03/30  10:22:11  alex
 * Таблица i-узлов в режиме UCB_IHASH используется как КЭШ,
 * то есть i-узел вычищается только если нужно место в таблице.
 *
 * Revision 1.3  87/01/22  08:32:04  alex
 * Введен параметр NOACCI - не обновлять и-узел на диске, если
 * файл только читали.
 *
 * Revision 1.2  86/05/27  17:44:56  avg
 * Если установлен режим IPK_SDADDR в ядре в I-узлах
 * в ядре будут храниться только младшие 2 байта дискового
 * адреса ( это годится для дисков до 64 Mбайт, если UCB_NKB==1 ).
 *
 * Revision 1.1  86/04/19  15:49:51  avg
 * Initial revision
 *
 */

#include "param.h"
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/inode.h>
#include <sys/ino.h>
#include <sys/filsys.h>
#include <sys/mount.h>
#include <sys/conf.h>
#include <sys/buf.h>
#ifdef  UCB_QUOTAS
#include <sys/quota.h>
#endif
#include <sys/inline.h>


#ifdef  IPK_TRUNCATE

#ifdef  UCB_QUOTAS
#define IFREE(dev, ip, bn)              qfree(ip, bn)
#define INDIRTRUNC(dev,bn,lbn,lvl,ip)   indirtrunc(dev,bn,lbn,lvl,ip)
#else
#define IFREE(dev, ip, bn)              free(dev, bn)
#define INDIRTRUNC(dev,bn,lbn,lvl,ip)   indirtrunc(dev,bn,lbn,lvl)
#endif

#else

#ifdef  UCB_QUOTAS
#define IFREE(dev, ip, bn)              qfree(ip, bn)
#define TLOOP(dev, bn, f1, f2, ip)      tloop(dev, bn, f1, f2, ip)
#else
#define IFREE(dev, ip, bn)              free(dev, bn)
#define TLOOP(dev, bn, f1, f2, ip)      tloop(dev, bn, f1, f2)
#endif

#endif IPK_TRUNCATE

#ifdef  UCB_IHASH

#ifdef  SMALL
#define INOHSZ  16              /* must be power of two */
#else
#define INOHSZ  64              /* must be power of two */
#endif
#define INOHASH(dev,ino)        (((dev) + (ino)) & (INOHSZ - 1))
struct  inode   *ihash[INOHSZ];
struct  inode   *ifreelist, **nextfree;
/*
 * TOFREE(ук-на-иузел) TOHASH(ук-на-ссылку-на-иузел,ук-на-иузел,куда-вклеить)
 */
#define TOFREE(ip) { *nextfree=(ip);(ip)->i_link=NULL;nextfree= &((ip)->i_link);}
#define TOHASH(pip,ip,npip) {*(pip)=(ip)->i_link;(ip)->i_link=(npip);(npip)=(ip);if(nextfree== &((ip)->i_link))nextfree=pip;}


/*
 * Initialize hash links for inodes
 * and build inode free list.
 */
ihinit()
{
	register struct inode *ip;
/* Культурный вариант
	nextfree= &ifreelist; ifreelist=NULL;
	for(ip = inode; ip < inodeNINODE; ip++)
		TOFREE(ip);
* Некультурный вариант  */
	ifreelist= &inode[0];
	for(ip=inode; ip < inodeNINODE-1;ip++)
		ip->i_link = ip+1;
	nextfree = &(inodeNINODE[-1].i_link);
}

/*
 * Find an inode if it is incore.
 * This is the equivalent, for inodes,
 * of ``incore'' in bio.c or ``pfind'' in subr.c.
 */
struct inode *
ifind(dev, ino)
dev_t dev;
ino_t ino;
{
	register struct inode *ip;

	for (ip = ihash[INOHASH(dev,ino)]; ip != NULL;
	    ip = ip->i_link)
		if (ino==ip->i_number && dev==ip->i_dev)
			return (ip);
	return ((struct inode *)NULL);
}

#endif  UCB_IHASH

/*
 * Look up an inode by device,inumber.
 * If it is in core (in the inode structure),
 * honor the locking protocol.
 * If it is not in core, read it in from the
 * specified device.
 * If the inode is mounted on, perform
 * the indicated indirection.
 * In all cases, a pointer to a locked
 * inode structure is returned.
 *
 * printf warning: no inodes -- if the inode
 *      structure is full
 * panic: no imt -- if the mounted file
 *      system is not in the mount table.
 *      "cannot happen"
 */
struct inode *
iget(dev, ino)
dev_t dev;
ino_t ino;
{
	register struct inode *ip;
	register struct mount *mp;
#ifdef  UCB_IHASH
	int slot;
	short in_free;
	register struct inode **pip; /* Адрес ссылки в старой цепи */
#else
	register struct inode *oip;
#endif
	struct buf *bp;
	struct dinode *dp;
loop:
#ifdef  UCB_IHASH
	in_free = 0;
	slot = INOHASH(dev, ino);
	pip = &ihash[slot];
	ip = *pip;
	while (ip != NULL ||
	       in_free++ == 0 && ((pip = &ifreelist),(ip = *pip)!=NULL) )
#else
	oip = NULL;
	for(ip = inode; ip < inodeNINODE; ip++)
#endif
	{
		if(ino == ip->i_number && dev == ip->i_dev) {
	again:
			if((ip->i_flag&ILOCK) != 0) {
				ip->i_flag |= IWANT;
				sleep((caddr_t)ip, PINOD);
				if(ino == ip->i_number && dev == ip->i_dev
					&& *pip == ip )
					goto again;
				goto loop;
			}
			if((ip->i_flag&IMOUNT) != 0) {
				for(mp = mount; mp < mountNMOUNT; mp++)
				if(mp->m_inodp == ip) {
					dev = mp->m_dev;
					ino = ROOTINO;
					goto loop;
				}
				panic("no imt");
			}
			ip->i_count++;
			ip->i_flag |= ILOCK;
#ifdef UCB_IHASH
			TOHASH(pip,ip,ihash[slot]);
#endif
			return(ip);
		}
#ifdef  UCB_IHASH
		pip = &ip->i_link;
		ip = *pip;
#else
		if(oip==NULL && ip->i_count==0)
			oip = ip;
#endif
	}
#ifdef  UCB_IHASH
	if (ifreelist == NULL)
#else
	ip = oip;
	if(ip == NULL)
#endif
	{
		tablefull("inode");
		u.u_error = ENFILE;
		return(NULL);
	}
#ifdef  UCB_IHASH
	ip = ifreelist;
	TOHASH( &ifreelist, ip, ihash[slot]);
#endif
	ip->i_dev = dev;
	ip->i_number = ino;
	ip->i_flag = ILOCK;
	ip->i_count = 1;
	ip->i_un.i_lastr = 0;
#ifdef  UCB_QUOTAS
	ip->i_quot = NULL;
#endif

	bp = bread(dev, itod(ino));

	/*
	 * Check I/O errors
	 */
	if (((bp->b_flags&B_ERROR) != 0) || bp->b_resid) {
		brelse(bp);
		/*
		 * This was an iput(ip), but if i_nlink happens to be 0,
		 * the itrunc() in iput() might be successful.
		 */
		ip->i_count = 0;
		ip->i_number = 0;
		ip->i_flag = 0;
#ifdef  UCB_IHASH
		ihash[slot] = ip->i_link;
		TOFREE(ip);
#endif
		return(NULL);
	}
	dp = (struct dinode *) mapin(bp);
	dp += itoo(ino);
	iexpand(ip, dp);
	mapout(bp);
	brelse(bp);
	return(ip);
}

iexpand(ip, dp)
register struct inode *ip;
register struct dinode *dp;
{
	register char *p1;
	char *p2;
	int i;

	ip->i_mode = dp->di_mode;
	ip->i_nlink = dp->di_nlink;
	ip->i_uid = dp->di_uid;
	ip->i_gid = dp->di_gid;
	ip->i_size = dp->di_size;
	p1 = (char *)ip->i_un.i_addr;
	p2 = (char *)dp->di_addr;
	for(i=0; i<NADDR; i++) {
#ifndef IPK_SDADDR
		*p1++ = *p2++;
		*p1++ = 0;
#else IPK_SDADDR
#ifdef UCB_QUOTAS
		if( (ip->i_mode & IFMT) == IFQUOT ) {
			if( i++ >= NADDR-1 )
				break;
			*p1++ = *p2;
			*p1++ = 0;
		}
#endif UCB_QUOTAS
		p2++;
#endif IPK_SDADDR       /*@VG chunk*/
		*p1++ = *p2++;
		*p1++ = *p2++;
	}
}

/*
 * Decrement reference count of
 * an inode structure.
 * On the last reference,
 * write the inode out and if necessary,
 * truncate and deallocate the file.
 */
iput(ip)
register struct inode *ip;
{
#ifdef  UCB_QUOTAS
	register struct inode *qp;
#endif
#ifdef  UCB_IHASH
	register struct inode *jp;
	register int i;
# endif
#ifdef  UCB_QUOTAS

	/*
	 * Berkeley's quota system is hierarchical
	 * so this loop is necessary to de-reference all
	 * quota nodes still hanging around
	 */
    do {
	qp = NULL;
#endif
	if(ip->i_count == 1) {
		ip->i_flag |= ILOCK;
		if(ip->i_nlink <= 0) {
#ifdef IPK_TRUNCATE
			itrunc(ip,(off_t)0);
#else IPK_TRUNCATE
			itrunc(ip);
#endif IPK_TRUNCATE
			ip->i_mode = 0;
			ip->i_flag |= IUPD|ICHG;
			ifree(ip->i_dev, ip->i_number);
		}
#ifdef  UCB_QUOTAS
		qp = ip->i_quot;
		if (qp != NULL)
			ip->i_quot = NULL;
#endif
#ifdef UCB_FSFIX
		IUPDAT(ip, &time, &time, 0);
#else
		IUPDAT(ip, &time, &time);
#endif
		prele(ip);
#ifdef  UCB_IHASH
		i = INOHASH(ip->i_dev, ip->i_number);
		if (ihash[i] == ip)
			ihash[i] = ip->i_link;
		else {
			for (jp = ihash[i]; jp != NULL; jp = jp->i_link)
				if (jp->i_link == ip) {
					jp->i_link = ip->i_link;
					goto done;
				}
			/* panic("iput");       */
			printf("iput: not in ihash i=%o mod=%o\n",ip->i_number,
				ip->i_mode);
		}
done:
		TOFREE(ip);/* Оставляем в памяти для повт. поиска */
		ip->i_flag &= ~(IUPD|ICHG|IACC|ILOCK);
		if(ip->i_mode == 0) ip->i_number = 0;
#else
		ip->i_flag = 0;
		ip->i_number = 0;
#endif
	}
	else
		prele(ip);
	ip->i_count--;
#ifdef  UCB_QUOTAS
	} while ((ip = qp) != NULL);
#endif
}

/*
 * Check accessed and update flags on
 * an inode structure.
 * If any are on, update the inode
 * with the current time.
 */
#ifdef UCB_FSFIX
/*
 * If waitfor set, then must insure
 * i/o order by waiting for the write
 * to complete.
 */

iupdat(ip, ta, tm, waitfor)
#else
iupdat(ip, ta, tm)
#endif
register struct inode *ip;
time_t *ta, *tm;
#ifdef UCB_FSFIX
int waitfor;
#endif
{
	register struct buf *bp;
	register struct dinode *dp;
	struct filsys *fp;
	char *p1, *p2;
	int i;

	if((ip->i_flag&(IUPD
#ifndef NOACCI
			    |IACC
#endif
			    |ICHG)) != 0) {
#ifdef EXMOUNT
		if ((bp = getfs(ip->i_dev)) == NULL )
			return;
		fp = (struct filsys *)mapin(bp);
		i = fp->s_ronly;
		mapout(bp);
		if(i)
			return;
#else
		if ((fp = getfs(ip->i_dev)) == NULL || fp->s_ronly)
			return;
#endif EXMOUNT
		bp = bread(ip->i_dev, itod(ip->i_number));
		if ((bp->b_flags & B_ERROR) || bp->b_resid) {
			brelse(bp);
			return;
		}
		dp = (struct dinode *) mapin(bp);
		dp += itoo(ip->i_number);
		dp->di_mode = ip->i_mode;
		dp->di_nlink = ip->i_nlink;
		dp->di_uid = ip->i_uid;
		dp->di_gid = ip->i_gid;
		dp->di_size = ip->i_size;
		p1 = (char *)dp->di_addr;
		p2 = (char *)ip->i_un.i_addr;
		for(i=0; i<NADDR; i++) {
#ifndef IPK_SDADDR
			*p1++ = *p2++;
#ifdef  MPX_FILS
			if(*p2++ != 0 && (ip->i_mode&IFMT)!=IFMPC
			   && (ip->i_mode&IFMT)!=IFMPB)
#else
			if(*p2++ != 0)
#endif
			   printf("iaddr[%d] > 2^24(%D), inum = %d, dev = %d\n",
				i, ip->i_un.i_addr[i], ip->i_number, ip->i_dev);
#else IPK_SDADDR
#ifdef UCB_QUOTAS
			if( (ip->i_mode & IFMT) == IFQUOT ) {
				if( i++ >= NADDR-1 )
					break;
				*p1++ = *p2++;
				p2++;
			} else
#endif UCB_QUOTAS
			*p1++ = 0;
#endif IPK_SDADDR
			*p1++ = *p2++;
			*p1++ = *p2++;
		}
		if(ip->i_flag&IACC)
			dp->di_atime = *ta;
		if(ip->i_flag&IUPD)
			dp->di_mtime = *tm;
		if(ip->i_flag&ICHG)
			dp->di_ctime = time;
		ip->i_flag &= ~(IUPD|IACC|ICHG);
		mapout(bp);
#ifdef UCB_FSFIX
		if (waitfor)
			bwrite(bp);
		else
#endif
			bdwrite(bp);
	}
}

/*
 * Free all the disk blocks associated
 * with the specified inode structure.
 * The blocks of the file are removed
 * in reverse order. This FILO
 * algorithm will tend to maintain
 * a contiguous free list much longer
 * than FIFO.
 */

#ifdef IPK_TRUNCATE

#define SINGLE 0        /* индексы косвенных блоков */
#define DOUBLE 1
#define TRIPLE 2

itrunc(ip,len)
register struct inode *ip;
	 off_t         len;
{
	register int          i,j;
		 dev_t        dev;
		 daddr_t      bn, lastblock, lastiblock[3];
#ifdef UCB_FSFIX
		struct inode itmp;
#endif

	if( ip->i_size <= len && (ip->i_flag & IPIPE) == 0 ){
		ip->i_flag |= IACC;
		iupdat(ip, &time, &time, 1);
		return;
	}
	i = ip->i_mode & IFMT;
#ifndef UCB_SYMLINKS
	if(i != IFREG && i != IFDIR)
#else
	if(i != IFREG && i != IFDIR && i != IFLNK)
#endif
		return;
       /*
	* Вычисление остатков блоков в списке i-узла
	* и в косвенных списках.
	* Если lastblock < 0 (== -1), то очищаем все блоки.
	*/
	lastblock = ((len+BSIZE-1)>>BSHIFT) - 1;
	lastiblock[SINGLE] = lastblock - (NADDR-3);
	lastiblock[DOUBLE] = lastiblock[SINGLE] - NINDIR;
	lastiblock[TRIPLE] = lastiblock[DOUBLE] - NINDIR*NINDIR;
	lastblock = MIN(lastblock,NADDR-4);
#ifdef UCB_FSFIX
       /*
	* Очищение i-узла перед освобождением блоков.
	* В случае краха системы могут появиться missing'и.
	*/
	itmp = *ip;
	for( i = NADDR-1 ; i > lastblock ; i-- ){
		if( i < NADDR-3 )
			itmp.i_un.i_addr[i] = (IDADDR)0;
		else if( lastiblock[i-(NADDR-3)] < 0 )
			itmp.i_un.i_addr[i] = (IDADDR)0;
	}
	itmp.i_size = len;
	itmp.i_flag |= ICHG | IUPD;
	iupdat(&itmp, &time, &time, 1);
	ip->i_flag &= ~(ICHG | IUPD | IACC);
#endif
	dev = ip->i_dev;
       /*
	* Освобождение блоков.
	*/
	for( i = NADDR-1 ; i > lastblock ; i-- ){
		bn = (daddr_t)(ip->i_un.i_addr[i]);
		if( bn != (daddr_t)0 ){
			if( i < NADDR-3 ){
				ip->i_un.i_addr[i] = (IDADDR)0;
				IFREE(dev,ip,bn);
			} else {
				j = i-(NADDR-3);
				if( lastiblock[j] < 0 )
					lastiblock[j] = (daddr_t)-1;
				INDIRTRUNC(dev,bn,lastiblock[j],j,ip);
				if( lastiblock[j] < 0 ){
					ip->i_un.i_addr[i] = (IDADDR)0;
					IFREE(dev,ip,bn);
				}
			}
		}
		if( i >= NADDR-3 && lastiblock[i-(NADDR-3)] >= 0 )
			break;
	}
	ip->i_size = len;
#ifndef UCB_FSFIX
	ip.i_flag |= ICHG | IUPD;
#endif
}

#ifdef UCB_QUOTAS
indirtrunc(dev,bn,lastbn,level,ip)
struct inode *ip;
#else
indirtrunc(dev,bn,lastbn,level)
#endif
dev_t         dev;
daddr_t       bn,lastbn;
int           level;
{
	register int         i;
	register struct buf *bp;
#ifdef UCB_FSFIX
		 struct buf *bp1;
#endif
	register daddr_t    *bap;
		 daddr_t     nb;
		 long        factor;
		 int         last,goflg;

	factor = 1;
	for( i = SINGLE ; i < level ; i++ )
		factor = factor * NINDIR;
	last = (lastbn>=(daddr_t)0) ? lastbn/factor : -1;
	bp = NULL;
	goflg = 0;
#ifdef UCB_FSFIX
       /*
	* Сначала очистить косвенный блок и сбросить его,
	* запомнив его в другом КЭШ'e (bp1).
	*/
	if( lastbn >= (daddr_t)0 ){
		bp1 = geteblk();
		goflg = 2;
		goto read;
use3:           copybb(bp,bp1,last*sizeof(daddr_t),last*sizeof(daddr_t),
		       (NINDIR-last)*sizeof(daddr_t)/sizeof(int));
		bap = (daddr_t *)mapin(bp);
		for( i = NINDIR-1 ; i > last ; i-- )
			bap[i] = (daddr_t)0;
		mapout(bp);
		bwrite(bp);
		bp = bp1;
	}
#endif
       /*
	* Освобождение всех ненужных блоков.
	*/
	for( i = NINDIR-1 ; i > last ; i-- ){
		if( bp == NULL )
			goto read;
use1:           bap = (daddr_t *)mapin(bp);
		nb = bap[i];
		mapout(bp);
		if( nb == (daddr_t)0 )
			continue;
		if( level > SINGLE ){
#ifdef UCB_FSFIX
			if( goflg == 0 ){
#endif
				brelse(bp);
				bp = NULL;
#ifdef UCB_FSFIX
			}
#endif
			INDIRTRUNC(dev,nb,(daddr_t)-1,level-1,ip);
		}
		IFREE(dev,ip,nb);
	}
       /*
	* Освобождение ненужных подблоков в последнем блоке.
	*/
#ifdef UCB_FSFIX
	if( lastbn >= (daddr_t)0 && level > SINGLE ){
#else
	if( lastbn >= (daddr_t)0 ){
#endif
		if( bp == NULL ){
			goflg++;
			goto read;
		}
use2:           bap = (daddr_t *)mapin(bp);
#ifndef UCB_FSFIX
		for( i = NINDIR-1 ; i > last ; i-- )
			bap[i] = (daddr_t)0;
#endif
		nb = bap[last];
		mapout(bp);
#ifdef UCB_FSFIX
		brelse(bp);
#else
		bwrite(bp);
#endif
		bp = NULL;
#ifndef UCB_FSFIX
		if( level > SINGLE )
#endif
		INDIRTRUNC(dev,nb,(daddr_t)(lastbn%factor),level-1,ip);
	}
	if( bp != NULL )
		brelse(bp);
	return;
read:
	bp = bread(dev,bn);
	if( (bp->b_flags & B_ERROR) || bp->b_resid ){
		brelse(bp);
		return;
	}
#ifdef UCB_FSFIX
	switch (goflg){
		case (2):
			goto use3;
		case (1):
			goto use2;
		default:
			goto use1;
	}
#else
	if( goflg )
		goto use2;
	goto use1;
#endif
}

#else IPK_TRUNCATE

itrunc(ip)
register struct inode *ip;
{
	register i;
	register dev_t dev;
	daddr_t bn;
#ifdef UCB_FSFIX
	struct inode itmp;
#endif

	i = ip->i_mode & IFMT;
#ifndef UCB_SYMLINKS
	if (i!=IFREG && i!=IFDIR)
#else
	if (i!=IFREG && i!=IFDIR && i!=IFLNK)
#endif
		return;

#ifdef UCB_FSFIX
	/*
	 * Clean inode on disk before freeing blocks
	 * to insure no duplicates if system crashes.
	 */
	itmp = *ip;
	itmp.i_size = 0;
	for (i = 0; i < NADDR; i++)
		itmp.i_un.i_addr[i] = 0;
	itmp.i_flag |= ICHG|IUPD;
	iupdat(&itmp, &time, &time, 1);
	ip->i_flag &= ~(IUPD|IACC|ICHG);

	/*
	 * Now return blocks to free list... if machine
	 * crashes, they will be harmless MISSING blocks.
	 */
#endif

	dev = ip->i_dev;
	for(i=NADDR-1; i>=0; i--) {
		bn = (daddr_t)(ip->i_un.i_addr[i]);
		if(bn == (daddr_t)0)
			continue;
		ip->i_un.i_addr[i] = (IDADDR)0;
		switch(i) {

		default:
			IFREE(dev, ip, bn);
			break;

		case NADDR-3:
			TLOOP(dev, bn, 0, 0, ip);
			break;

		case NADDR-2:
			TLOOP(dev, bn, 1, 0, ip);
			break;

		case NADDR-1:
			TLOOP(dev, bn, 1, 1, ip);
		}
	}
	ip->i_size = 0;
#ifndef UCB_FSFIX
	ip->i_flag |= ICHG|IUPD;
#else
	/* Inode was written and flags updated above.
	 * No need to modify flags here.
	 */
#endif

}

#ifdef  UCB_QUOTAS
tloop(dev, bn, f1, f2, ip)
#else
tloop(dev, bn, f1, f2)
#endif
dev_t dev;
daddr_t bn;
#ifdef  UCB_QUOTAS
struct inode    *ip;
#endif
{
	register i;
	register struct buf *bp;
	register daddr_t *bap;
	daddr_t nb;

	bp = NULL;
	for(i=NINDIR-1; i>=0; i--) {
		if(bp == NULL) {
			bp = bread(dev, bn);
			if ((bp->b_flags & B_ERROR) || bp->b_resid) {
				brelse(bp);
				return;
			}
		}
		bap = (daddr_t *) mapin(bp);
		nb = bap[i];
		mapout(bp);
		if(nb == (daddr_t)0)
			continue;
		if(f1) {
			brelse(bp);
			bp = NULL;
			TLOOP(dev, nb, f2, 0, ip);
		} else
			IFREE(dev, ip, nb);
	}
	if(bp != NULL)
		brelse(bp);
	IFREE(dev, ip, bn);
}

#endif IPK_TRUNCATE

/*
 * Make a new file.
 */
struct inode *
maknode(mode)
{
	register struct inode *ip;

	ip = ialloc(u.u_pdir->i_dev);
	if(ip == NULL) {
		iput(u.u_pdir);
		return(NULL);
	}
	ip->i_flag |= IACC|IUPD|ICHG;
	if((mode&IFMT) == 0)
		mode |= IFREG;
	ip->i_mode = mode & ~u.u_cmask;
	ip->i_nlink = 1;
	ip->i_uid = u.u_uid;
	ip->i_gid = u.u_gid;

#ifdef UCB_FSFIX
	/*
	 * Make sure inode goes to disk before directory entry.
	 */
	iupdat(ip, &time, &time, 1);
#endif

	wdir(ip);
	return(ip);
}

/*
 * Write a directory entry with
 * parameters left as side effects
 * to a call to namei.
 */
wdir(ip)
struct inode *ip;
{

	if (u.u_pdir->i_nlink <= 0) {
		u.u_error = ENOTDIR;
		goto out;
	}
	u.u_dent.d_ino = ip->i_number;
	bcopy((caddr_t)u.u_dbuf, (caddr_t)u.u_dent.d_name, DIRSIZ);
	u.u_count = sizeof(struct direct);
	u.u_segflg = 1;
	u.u_base = (caddr_t)&u.u_dent;
	writei(u.u_pdir);
#ifdef  UCB_QUOTAS
	/*
	 * Copy quota for new file
	 */
	if (!u.u_error)
		qcopy(u.u_pdir, ip);
#endif
out:
	iput(u.u_pdir);
}

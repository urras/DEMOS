/*
 * System parameters.
 *
 * This file is copied into each directory where we compile
 * the kernel; it should be modified there to suit local taste
 * if necessary.
 *
 * $Log:	param.c,v $
 * Revision 1.6  90/11/12  19:15:08  root
 * Новые вещи для СМ1425 и перемещение include.
 * 
 * Revision 1.5  89/04/27  14:24:44  korotaev
 * Изменения в связи с передвижением каталогов и файлов
 * 
 * Revision 1.4  89/04/18  17:22:41  korotaev
 * Перед делением на space.h и param.c и перед перенесением 
 * param.c в ../sys
 * 
 * Revision 1.3  89/03/04  14:59:39  abs
 * *** empty log message ***
 * 
 * Revision 1.2  88/03/23  14:12:21  korotaev
 * Состояние после слияния с АЗЛК, Э-85 и Бурковским планировщиком
 * 
 * Revision 1.1  86/04/19  15:02:35  avg
 * Initial revision
 * 
 */
#include        <sys/param.h>
#include        <sys/systm.h>
#include        "../include/buf.h"
#include        <sys/tty.h>
#include        <sys/conf.h>
#include        <sys/proc.h>
#include        <sys/text.h>
#include        <sys/dir.h>
#include        <sys/user.h>
#include        <sys/file.h>
#include        <sys/inode.h>
#include        <sys/filsys.h>
#include        "../include/mount.h"
#include        "../include/callout.h"
#include        <sys/acct.h>
#include        "../include/map.h"
#include        <sys/seg.h>
#include        "h/space.h"

int	maxusers = MAXUSERS;
int	hz	= HZ;
int	timezone = TIMEZONE;
bool_t	dstflag	= DSTFLAG;
int	nmount	= NMOUNT;
int	nfile	= NFILE;
int	ninode	= NINODE;
int	nproc	= NPROC;
int	ntext	= NTEXT;
int	nbuf	= NBUF;
int	nclist	= NCLIST;
int	ncallout = NCALL;
int	ndisk	= NDISK;
int	cmapsiz	= CMAPSIZ;
int	smapsiz	= SMAPSIZ;
int     nfslot  = NFLOCK;

struct	mount	mount[NMOUNT];
struct	inode	inode[NINODE];
struct	buf	buf[NBUF];
struct	callout	callout[NCALL + 1];	/* last one used as a delimiter */
struct	buf	bfreelist;
#ifndef	UCB_CLIST
struct	cblock	cfree[NCLIST];
#else
unsigned clstdesc = ((((btoc(NCLIST*sizeof(struct cblock)))-1) << 8) | RW);
#endif
long	dk_time[1 << (NDISK)];
long	dk_numb[NDISK];
long	dk_wds[NDISK];

struct mapent _coremap[CMAPSIZ];
struct map coremap[1] = {
	_coremap,
	&_coremap[CMAPSIZ],
	"coremap"
};

struct mapent _swapmap[SMAPSIZ];
struct map swapmap[1] = {
	_swapmap,
	&_swapmap[SMAPSIZ],
	"swapmap"
};

struct	mount	*mountNMOUNT	= &mount[NMOUNT];
struct	file	*fileNFILE	= &file[NFILE];
struct	inode	*inodeNINODE	= &inode[NINODE];
struct	proc	*procNPROC	= &proc[NPROC];
struct	text	*textNTEXT	= &text[NTEXT];
/* callNCALL points to the last slot, which must be a terminator */
struct	callout	*callNCALL	= &callout[NCALL];

#ifdef	UCB_METER
char	counted[NTEXT];
#endif

int	bsize	= BSIZE + BSLOP;	/* size of buffers */

#ifdef ACCT
struct	acct	acctbuf;
struct	inode	*acctp;
#endif

char msgbuf[MSGBUFS]	= {"\0"};

/*
 *  Declarations of structures loaded last and allowed to
 *  reside in the 0120000-140000 range (where buffers and clists are
 *  mapped).  These structures must be extern everywhere else,
 *  and the asm output of cc is edited to move these structures
 *  from comm to bss (which is last) (see the script :comm-to-bss).
 */
int	remap_area;	/* start of possibly mapped area; must be first */
struct	proc	proc[NPROC];
struct	file	file[NFILE];
struct	text	text[NTEXT];

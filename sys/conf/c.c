/*
 *
 * Конфигурация системы - таблицы указателей на процедуры
 * драйверов у-в и дисциплин линий.
 *
 * $Log:	c.c,v $
 * Revision 1.8  90/12/12  17:03:36  korotaev
 * Правки под ДВК.
 * 
 * Revision 1.7  90/11/12  18:50:53  root
 * Новые вещи для СМ 1425 и перемещение некоторых include'ов.
 * 
 * Revision 1.6  89/06/28  20:31:35  avg
 * Добавлено обращение к таблице mpytty. (падал select).
 * 
 * Revision 1.5  88/11/16  14:00:36  dvolodin
 * *** empty log message ***
 *
 * Revision 1.4  88/03/23  14:10:23  korotaev
 * Состояние после слияния с АЗЛК, Э-85 и Бурковским планировщиком
 *
 * Revision 1.3  88/01/13  15:40:54  korotaev
 * Исправления ошибок при слиянии ядер ИПК и АЗЛК.
 *
 * Revision 1.2  86/11/23  22:48:01  alex
 * Добавлен dj
 *
 * Revision 1.1  86/04/19  15:01:51  avg
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
#include        <sys/acct.h>
#include        "../include/map.h"
#include        <sys/filsys.h>
#include        "../include/mount.h"


int     nulldev();
int     nodev();
int     nullioctl();

#include        "h/bk.h"
#if     NBK > 0
int     bkopen(), bkclose(), bkread(), bkioctl(), bkinput();
#endif  NBK

#include        "h/dh.h"
#if     NDH > 0
int     dhopen(), dhclose(), dhread(), dhwrite(), dhioctl(), dhstop();
extern  struct  tty     dh11[];
#endif  NDH

#include        "h/dhu.h"
#if     NDHU > 0
int     dhuopen(), dhuclose(), dhuread(), dhuwrite(), dhuioctl(), dhustop();
extern  struct  tty     dhu11[];
#endif  NDHU

#include        "h/dj.h"
#if     NDJ > 0
int     djopen(), djclose(), djread(), djwrite(), djioctl();
extern  struct  tty     dj11[];
#endif  NDJ

#include        "h/dn.h"
#if     NDN > 0
int     dnopen(), dnclose(), dnwrite();
#endif  NDN

#include        "h/dz.h"
#if     NDZ > 0
int     dzopen(), dzclose(), dzread(), dzwrite(), dzioctl();
#ifdef  DZ_PDMA
int     dzstop();
#else
#define dzstop          nulldev
#endif
extern  struct  tty     dz11[];
#endif  NDZ

#include        "h/hk.h"
#if     NHK > 0
int     hkstrategy(), hkread(), hkwrite(), hkroot();
extern  struct  buf     hktab;
#define hkopen          nulldev
#define hkclose         nulldev
#define _hktab          &hktab
#endif  NHK

#include        "h/hp.h"
#if     NHP > 0
int     hpstrategy(), hpread(), hpwrite(), hproot();
extern  struct  buf     hptab;
#define hpopen          nulldev
#define hpclose         nulldev
#define _hptab          &hptab
#endif  NHP

#include        "h/hs.h"
#if     NHS > 0
int     hsstrategy(), hsread(), hswrite(), hsroot();
extern  struct  buf     hstab;
#define _hstab          &hstab
#define hsopen          nulldev
#define hsclose         nulldev
#endif  NHS

#include        "h/ht.h"
#if     NHT > 0
int     htopen(), htclose(), htread(), htwrite(), htstrategy();
#ifdef  HT_IOCTL
int     htioctl();
#else
#define htioctl         nodev
#endif
extern  struct  buf     httab;
#define _httab          &httab
#endif  NHT

#include        "h/lp.h"
#if     NLP > 0
int     lpopen(), lpclose(), lpwrite(), lpioctl();
#endif  NLP

#include        "h/pc.h"
#if     NPC > 0
int     pcopen(), pcclose(), pcread(), pcwrite();
#endif  NPC

#include        "h/pty.h"
#if     NPTY > 0
int     ptcopen(), ptcclose(), ptcread(), ptcwrite(), ptcselect();
int     ptsopen(), ptsclose(), ptsread(), ptswrite(), ptyioctl();
#endif  NPTY

#include        "h/mpy.h"
#if     NMPY > 0
int     mtyopen(), mtyclose(), mtyread(), mtywrite(), mtyioctl();
int     mpyopen(), mpyclose(), mpyinput(), mpyioctl();
extern struct tty mpytty[];
#endif

#include        "h/rk.h"
#if     NRK > 0
int     rkstrategy(), rkread(), rkwrite(), rkopen();
extern  struct  buf     rktab;
#define rkclose         nulldev
#define _rktab          &rktab
#endif  NRK

#include        "h/rw.h"
#if     NRW > 0
int     rwstrategy(), rwread(), rwwrite();
extern  struct  buf     rwtab;
#define rwopen          nulldev
#define rwclose         nulldev
#define _rwtab          &rwtab
#else
#endif  NRW

#include        "h/dw.h"
#if     NDW > 0
int     dwstrategy(), dwread(), dwwrite();
extern  struct  buf     dwtab;
#define dwopen          nulldev
#define dwclose         nulldev
#define _dwtab          &dwtab
#endif  NDW

#include        "h/my.h"
#if     NMY > 0
int     myopen(), mystrategy(), myread(), mywrite();
extern	struct	buf	mytab;
#define	myclose		nulldev
#define	_mytab		&mytab
#endif  NMY

#include        "h/fd.h"
#if     NFD > 0
int     fdstrategy(), fdread(), fdwrite();
extern  struct  buf     fdtab;
#define fdopen          nulldev
#define fdclose         nulldev
#define _fdtab          &fdtab
#endif  NFD

#include        "h/rl.h"
#if     NRL > 0
int     rlstrategy(), rlread(), rlwrite();
extern  struct  buf     rltab;
#define rlopen          nulldev
#define rlclose         nulldev
#define _rltab          &rltab
#endif  NRL

#include        "h/rm.h"
#if     NRM > 0
int     rmstrategy(), rmread(), rmwrite(), rmroot();
extern  struct  buf     rmtab;
#define rmopen          nulldev
#define rmclose         nulldev
#define _rmtab          &rmtab
#endif  NRM

#include        "h/rp.h"
#if     NRP > 0
int     rpstrategy(), rpread(), rpwrite();
extern  struct  buf     rptab;
#define rpopen          nulldev
#define rpclose         nulldev
#define _rptab          &rptab
#endif  NRP

#include        "h/tm.h"
#if     NTM > 0
int     tmopen(), tmclose(), tmread(), tmwrite(), tmstrategy();
#ifdef  TM_IOCTL
int     tmioctl();
#else
#define tmioctl         nodev
#endif
extern  struct  buf     tmtab;
#define _tmtab          &tmtab
#endif  NTM

#include        "h/ts.h"
#if     NTS > 0
int     tsopen(), tsclose(), tsread(), tswrite(), tsstrategy();
#ifdef  TS_IOCTL
int     tsioctl();
#else
#define tsioctl         nodev
#endif
extern  struct  buf     tstab;
#define _tstab          &tstab
#endif  NTS
/*
 *#include        "vp.h"
 *#if     NVP > 0
 *int     vpopen(), vpclose(), vpwrite(), vpioctl();
 *#endif  NVP
 */
#include        "h/xp.h"
#if     NXP > 0
int     xpstrategy(), xpread(), xpwrite(), xproot();
extern  struct  buf     xptab;
#define xpopen          nulldev
#define xpclose         nulldev
#define _xptab          &xptab
#endif  NXP

#include        "h/rx.h"
#if     NRX > 0
int     rxstrategy(), rxread(), rxwrite();
extern  struct  buf     rxtab;
#define rxopen          nulldev
#define rxclose         nulldev
#define _rxtab          &rxtab
#endif  NRX

#include        "h/ra.h"
#if     NRA > 0
int     udopen(), udstrategy(), udread(), udwrite();
extern  struct  buf     udtab;
#define udclose         nulldev
#define _udtab          &udtab
#endif  NRA

#include        "h/vd.h"
#if     NVD > 0
int     vdstrategy(), vdread(), vdwrite(), vdroot();
extern  struct  buf     vdtab;
#define vdopen          nulldev
#define vdclose         nulldev
#define _vdtab          &vdtab
#endif  NVD

struct  bdevsw  bdevsw[] =
{
#if NRK > 0
	rkopen,         rkclose,        rkstrategy,
	nulldev,        _rktab,         /* rk = 0 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,
#endif

#if NRP > 0
	rpopen,         rpclose,        rpstrategy,
	nulldev,        _rptab,         /* rp = 1 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,
#endif

#if NMY > 0
	myopen,         myclose,        mystrategy,
	nulldev,        _mytab,         /* my = 2 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,              /* rf = 2 */
#endif

#if NTM > 0
	tmopen,         tmclose,        tmstrategy,
	nulldev,        _tmtab,         /* tm = 3 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,
#endif

#if NHK > 0
	hkopen,         hkclose,        hkstrategy,
	hkroot,         _hktab,         /* hk = 4 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,
#endif

#if NHS > 0
	hsopen,         hsclose,        hsstrategy,
	hsroot,         _hstab,         /* hs = 5 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,
#endif

#if NXP > 0
	xpopen,         xpclose,        xpstrategy,
	xproot,         _xptab,         /* xp = 6 */
#else
#if NHP > 0
	hpopen,         hpclose,        hpstrategy,
	hproot,         _hptab,         /* hp = 6 */
#else
#if NRM > 0
	rmopen,         rmclose,        rmstrategy,
	rmroot,         _rmtab,         /* rm = 6 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,
#endif
#endif
#endif

#if NHT > 0
	htopen,         htclose,        htstrategy,
	nulldev,        _httab,         /* ht = 7 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,
#endif

#if NRL > 0
	rlopen,         rlclose,        rlstrategy,
	nulldev,        _rltab,         /* rl = 8 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,
#endif

#if NTS > 0
	tsopen,         tsclose,        tsstrategy,
	nulldev,        _tstab,         /* ts = 9 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,
#endif

#if NRW > 0
	rwopen,         rwclose,        rwstrategy,
	nulldev,        _rwtab,         /* rw = 10 */
#else
#if NDW > 0
	dwopen,         dwclose,        dwstrategy,
	nulldev,        _dwtab,         /* dw = 10 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,
#endif
#endif

#if NRX > 0
	rxopen,         rxclose,        rxstrategy,
	nulldev,        _rxtab,         /* rx = 11 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,
#endif

#if NFD > 0
	fdopen,         fdclose,        fdstrategy,
	nulldev,        _fdtab,         /* fd = 12 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,
#endif

#if NVD > 0
	vdopen,         vdclose,        vdstrategy,
	vdroot,         _vdtab,         /* vd = 13 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,
#endif

#if NRA > 0
	udopen,         udclose,        udstrategy,
	nulldev,        _udtab,         /* ra = 14 */
#else
	nodev,          nodev,          nodev,
	nulldev,        0,
#endif
};
int     nblkdev = sizeof(bdevsw) / sizeof(bdevsw[0]);

#include "h/ks.h"
#if NKS > 0
int     ksopen(), ksclose(), ksread(), kswrite(), ksioctl();
extern  struct  tty     ks11[];
#endif NKS

#include "h/kl.h"
#if NKL > 0
int     klopen(), klclose(), klread(), klwrite(), klioctl();
extern  struct  tty     kl11[];
#endif NKL

#include "h/wl.h"
#if NWL > 0
int     wlopen(), wlclose(), wlread(), wlwrite(), wlioctl();
extern  struct  tty     wl11[];
#endif NWL

#include "h/kx.h"
#if NKX > 0
int     kxopen(), kxclose(), kxread(), kxwrite(), kxioctl();
extern  struct  tty     kx11[];
#endif NKX

#include "h/ky.h"
#if NKY > 0
int     kyopen(), kyclose(), kyread(), kywrite(), kyioctl();
extern  struct  tty     ky11[];
#endif NKY

#include "h/video.h"
#if NVI > 0
int     viopen(), viclose(), viread(), viwrite(), viioctl();
extern  struct  tty     vi11[];
#endif NVI

#include "h/mk.h"
#if NMK > 0
int     mkopen(), mkclose(), mkread(), mkwrite(), mkioctl();
extern  struct  tty     mk11[];
#endif NMK

int     mmread(), mmwrite();
int     syopen(), syread(), sywrite(), sysioctl();
int     syselect();
int     seltrue(), ttselect();

struct  cdevsw  cdevsw[] =
{

#if NKL > 0
	klopen,         klclose,        klread,         klwrite,
	klioctl,        nulldev,        kl11,           /* kl = 0 */
	ttselect,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NPC > 0
	pcopen,         pcclose,        pcread,         pcwrite,
	nodev,          nulldev,        0,              /* pc = 1 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NWL > 0
	wlopen,         wlclose,        wlread,         wlwrite,
	wlioctl,        nulldev,        wl11,           /* wl = 2 */
	ttselect,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,              /* vp = 2 */
	nodev,
#endif

#if NLP > 0
	lpopen,         lpclose,        nodev,          lpwrite,
	lpioctl,        nulldev,        0,              /* lp = 3 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NDH > 0
	dhopen,         dhclose,        dhread,         dhwrite,
	dhioctl,        dhstop,         dh11,           /* dh = 4 */
	ttselect,
#else
#if NDHU > 0
	dhuopen,        dhuclose,       dhuread,        dhuwrite,
	dhuioctl,       dhustop,        dhu11,          /* dhu = 4 */
	ttselect,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,              /* ?? = 4 */
	nodev,
#endif
#endif

	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,              /* dp = 5 */
	nodev,

#if NDJ > 0
	djopen,         djclose,        djread,         djwrite,
	djioctl,        nulldev,        dj11,           /* dj = 6 */
	ttselect,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NDN > 0
	dnopen,         dnclose,        nodev,          dnwrite,
	nodev,          nulldev,        0,              /* dn = 7 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

	nulldev,        nulldev,        mmread,         mmwrite,
	nodev,          nulldev,        0,              /* mem = 8 */
	seltrue,

#if NRK > 0
	rkopen,         rkclose,        rkread,         rkwrite,
	nodev,          nulldev,        0,              /* rk = 9 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NMY > 0
	myopen,         myclose,        myread,         mywrite,
	nodev,          nulldev,        0,              /* my = 10 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,              /* rf = 10 */
	nodev,
#endif

#if NRP > 0
	rpopen,         rpclose,        rpread,         rpwrite,
	nodev,          nulldev,        0,              /* rp = 11 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NTM > 0
	tmopen,         tmclose,        tmread,         tmwrite,
	tmioctl,        nulldev,        0,              /* tm = 12 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NHS > 0
	hsopen,         hsclose,        hsread,         hswrite,
	nodev,          nulldev,        0,              /* hs = 13 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NXP > 0
	xpopen,         xpclose,        xpread,         xpwrite,
	nodev,          nulldev,        0,              /* xp = 14 */
	seltrue,
#else
#if NHP > 0

	hpopen,         hpclose,        hpread,         hpwrite,
	nodev,          nulldev,        0,              /* hp = 14 */
	seltrue,
#else
#if NRM > 0
	rmopen,         rmclose,        rmread,         rmwrite,
	nodev,          nulldev,        0,              /* rm = 14 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif
#endif
#endif

#if NHT > 0
	htopen,         htclose,        htread,         htwrite,
	htioctl,        nulldev,        0,              /* ht = 15 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,              /* du = 16 */
	nodev,

	syopen,         nulldev,        syread,         sywrite,
	sysioctl,       nulldev,        0,              /* tty = 17 */
	syselect,

#if NRL > 0
	rlopen,         rlclose,        rlread,         rlwrite,
	nodev,          nulldev,        0,              /* rl = 18 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NHK > 0
	hkopen,         hkclose,        hkread,         hkwrite,
	nodev,          nulldev,        0,              /* hk = 19 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NTS > 0
	tsopen,         tsclose,        tsread,         tswrite,
	tsioctl,        nulldev,        0,              /* ts = 20 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NDZ > 0
	dzopen,         dzclose,        dzread,         dzwrite,
	dzioctl,        dzstop,         dz11,           /* dz = 21 */
	ttselect,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,

#if NVI > 0
	viopen,         viclose,        viread,         viwrite,
	viioctl,        nulldev,        vi11,           /* vi = 23 */
	ttselect,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NRW > 0
	rwopen,         rwclose,        rwread,         rwwrite,
	nodev,          nulldev,        0,              /* rw = 24 */
	seltrue,
#else
#if NDW > 0
	dwopen,         dwclose,        dwread,         dwwrite,
	nodev,          nulldev,        0,              /* dw = 24 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif
#endif

#if NKX > 0
	kxopen,         kxclose,        kxread,         kxwrite,
	kxioctl,        nulldev,        kx11,           /* kx = 25 */
	ttselect,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NRX > 0
	rxopen,         rxclose,        rxread,         rxwrite,
	nodev,          nulldev,        0,              /* rx = 26 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NFD > 0
	fdopen,         fdclose,        fdread,         fdwrite,
	nodev,          nulldev,        0,              /* fd = 27 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NVD > 0
	vdopen,         vdclose,        vdread,         vdwrite,
	nodev,          nulldev,        0,              /* vd = 28 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NKY > 0
	kyopen,         kyclose,        kyread,         kywrite,
	kyioctl,        nulldev,        ky11,           /* ky = 29 */
	ttselect,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NRA > 0
	udopen,         udclose,        udread,         udwrite,
	nodev,          nulldev,        0,              /* ra = 30 */
	seltrue,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NPTY > 0
	ptcopen,        ptcclose,       ptcread,        ptcwrite,
	ptyioctl,       nulldev,        0,              /* ptc = 31 (pty) */
	ptcselect,
	ptsopen,        ptsclose,       ptsread,        ptswrite,
	ptyioctl,       nulldev,        0,              /* pts = 32 (pty) */
	ttselect,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif

#if NMPY > 0
	mtyopen,        mtyclose,       mtyread,        mtywrite,
	mtyioctl,       nulldev,        mpytty,         /* mty = 33 */
	ttselect,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif NMPY

#if NKS > 0
	ksopen,         ksclose,        ksread,         kswrite,
	ksioctl,        nulldev,        ks11,           /* ks = 34 */
	ttselect,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif NKS

#if NMK > 0
	mkopen,         mkclose,        mkread,         mkwrite,
	mkioctl,        nulldev,        mk11,           /* mk = 35 */
	ttselect,
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
	nodev,
#endif NMK
};

int     nchrdev = sizeof(cdevsw) / sizeof(cdevsw[0]);

#ifdef  OLDTTY
int     ttread(), ttyinput(), ttyoutput();
caddr_t ttwrite();
#define ttopen          nulldev
#define ttclose         nulldev
#define ttioctl         nullioctl
#define ttmodem         nulldev
#endif

int     ntyopen(), ntyclose(), ntread(), ntyinput(), ntyoutput();
caddr_t ntwrite();
#define ntyioctl        nullioctl
#define ntymodem        nulldev

struct  linesw linesw[] =
{
#ifdef OLDTTY
	ttopen,         ttclose,        ttread,         ttwrite,
	ttioctl,        ttyinput,       ttyoutput,      ttmodem,        /*0*/
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          nodev,          nodev,          /*0*/
#endif OLDTTY

	ntyopen,        ntyclose,       ntread,         ntwrite,
	ntyioctl,       ntyinput,       ntyoutput,      ntymodem,       /*1*/

#if     NBK > 0
	bkopen,         bkclose,        bkread,         ttwrite,
	bkioctl,        bkinput,        nodev,          nulldev         /*2*/
#else   NBK
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          nodev,          nodev,          /*2*/
#endif  NBK

#if     NMPY > 0
	mpyopen,        mpyclose,       nodev,          nodev,
	mpyioctl,       mpyinput,       nulldev,        nulldev         /*3*/
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          nodev,          nodev,          /*3*/
#endif  NMPY
};

int     nldisp  = sizeof(linesw) / sizeof(linesw[0]);

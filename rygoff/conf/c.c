/*
 *
 * Конфигурация системы - таблицы указателей на процедуры
 * драйверов у-в и дисциплин линий.
 *
 * $Log:        c.c,v $
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

#include        "param.h"
#include        <sys/systm.h>
#include        <sys/buf.h>
#include        <sys/tty.h>
#include        <sys/conf.h>
#include        <sys/proc.h>
#include        <sys/text.h>
#include        <sys/dir.h>
#include        <sys/user.h>
#include        <sys/file.h>
#include        <sys/inode.h>
#include        <sys/acct.h>
#include        <sys/map.h>
#include        <sys/filsys.h>
#include        <sys/mount.h>


int     nulldev();
int     nodev();
int     nullioctl();

#include        "bk.h"
#if     NBK > 0
int     bkopen(), bkclose(), bkread(), bkioctl(), bkinput();
#endif  NBK

#include        "dh.h"
#if     NDH > 0
int     dhopen(), dhclose(), dhread(), dhwrite(), dhioctl(), dhstop();
extern  struct  tty     dh11[];
#endif  NDH

#include        "dhu.h"
#if     NDHU > 0
int     dhuopen(), dhuclose(), dhuread(), dhuwrite(), dhuioctl(), dhustop();
extern  struct  tty     dhu11[];
#endif  NDHU

#include        "dj.h"
#if     NDJ > 0
int     djopen(), djclose(), djread(), djwrite(), djioctl();
extern  struct  tty     dj11[];
#endif  NDJ

#include        "dn.h"
#if     NDN > 0
int     dnopen(), dnclose(), dnwrite();
#endif  NDN

#include        "dz.h"
#if     NDZ > 0
int     dzopen(), dzclose(), dzread(), dzwrite(), dzioctl();
#ifdef  DZ_PDMA
int     dzstop();
#else
#define dzstop          nulldev
#endif
extern  struct  tty     dz11[];
#endif  NDZ

#include        "hk.h"
#if     NHK > 0
int     hkstrategy(), hkread(), hkwrite(), hkroot();
extern  struct  buf     hktab;
#define hkopen          nulldev
#define hkclose         nulldev
#define _hktab          &hktab
#endif  NHK

#include        "hp.h"
#if     NHP > 0
int     hpstrategy(), hpread(), hpwrite(), hproot();
extern  struct  buf     hptab;
#define hpopen          nulldev
#define hpclose         nulldev
#define _hptab          &hptab
#endif  NHP

#include        "hs.h"
#if     NHS > 0
int     hsstrategy(), hsread(), hswrite(), hsroot();
extern  struct  buf     hstab;
#define _hstab          &hstab
#define hsopen          nulldev
#define hsclose         nulldev
#endif  NHS

#include        "ht.h"
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

#include        "lp.h"
#if     NLP > 0
int     lpopen(), lpclose(), lpwrite(), lpioctl();
#endif  NLP

#include        "pc.h"
#if     NPC > 0
int     pcopen(), pcclose(), pcread(), pcwrite();
#endif  NPC

#include        "pty.h"
#if     NPTY > 0
int     ptcopen(), ptcclose(), ptcread(), ptcwrite(), ptcselect();
int     ptsopen(), ptsclose(), ptsread(), ptswrite(), ptyioctl();
#endif  NPTY

#include        "rk.h"
#if     NRK > 0
int     rkstrategy(), rkread(), rkwrite(), rkopen();
extern  struct  buf     rktab;
#define rkclose         nulldev
#define _rktab          &rktab
#endif  NRK

#include        "rw.h"
#if     NRW > 0
int     rwstrategy(), rwread(), rwwrite();
extern  struct  buf     rwtab;
#define rwopen          nulldev
#define rwclose         nulldev
#define _rwtab          &rwtab
#else
#endif  NRW

#include        "fd.h"
#if     NFD > 0
int     fdstrategy(), fdread(), fdwrite();
extern  struct  buf     fdtab;
#define fdopen          nulldev
#define fdclose         nulldev
#define _fdtab          &fdtab
#endif  NFD

#include        "rl.h"
#if     NRL > 0
int     rlstrategy(), rlread(), rlwrite();
extern  struct  buf     rltab;
#define rlopen          nulldev
#define rlclose         nulldev
#define _rltab          &rltab
#endif  NRL

#include        "rm.h"
#if     NRM > 0
int     rmstrategy(), rmread(), rmwrite(), rmroot();
extern  struct  buf     rmtab;
#define rmopen          nulldev
#define rmclose         nulldev
#define _rmtab          &rmtab
#endif  NRM

#include        "rp.h"
#if     NRP > 0
int     rpstrategy(), rpread(), rpwrite();
extern  struct  buf     rptab;
#define rpopen          nulldev
#define rpclose         nulldev
#define _rptab          &rptab
#endif  NRP

#include        "tm.h"
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

#include        "ts.h"
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
#include        "xp.h"
#if     NXP > 0
int     xpstrategy(), xpread(), xpwrite(), xproot();
extern  struct  buf     xptab;
#define xpopen          nulldev
#define xpclose         nulldev
#define _xptab          &xptab
#endif  NXP

#include        "rw.h"
#if     NRW > 0
int     rwstrategy(), rwread(), rwwrite();
extern  struct  buf     rwtab;
#define rwopen          nulldev
#define rwclose         nulldev
#define _rwtab          &rwtab
#endif  NRW

#include        "rx.h"
#if     NRX > 0
int     rxstrategy(), rxread(), rxwrite();
extern  struct  buf     rxtab;
#define rxopen          nulldev
#define rxclose         nulldev
#define _rxtab          &rxtab
#endif  NRX

#include        "ra.h"
#if     NRA > 0
int     udopen(), udstrategy(), udread(), udwrite();
extern  struct  buf     udtab;
#define udclose         nulldev
#define _udtab          &udtab
#endif  NRA

#include        "vd.h"
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

	nodev,          nodev,          nodev,
	nulldev,        0,              /* rf = 2 */

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
	nodev,          nodev,          nodev,
	nulldev,        0,
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

#include "kl.h"
#if NKL > 0
int     klopen(), klclose(), klread(), klwrite(), klioctl();
extern  struct  tty     kl11[];
#endif NKL

#include "rubin.h"
#define ilopen  nulldev
#define ilread  nodev
#define ilioctl nodev
#if NRUBIN > 0
int     ilclose(), ilwrite();
#endif NRUBIN

#include "kx.h"
#if NKX > 0
int     kxopen(), kxclose(), kxread(), kxwrite(), kxioctl();
extern  struct  tty     kx11[];
#endif NKX

#include "ky.h"
#if NKY > 0
int     kyopen(), kyclose(), kyread(), kywrite(), kyioctl();
extern  struct  tty     ky11[];
#endif NKY

#include "video.h"
#if NVI > 0
int     viopen(), viclose(), viread(), viwrite(), viioctl();
extern  struct  tty     vi11[];
#endif NVI

int     mmread(), mmwrite();
int     syopen(), syread(), sywrite(), sysioctl();

#ifdef IPK_SELECT
int     seltrue(), ttselect();
#endif

struct  cdevsw  cdevsw[] =
{

#if NKL > 0
	klopen,         klclose,        klread,         klwrite,
	klioctl,        nulldev,        kl11,           /* kl = 0 */
#ifdef IPK_SELECT
	ttselect,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NPC > 0
	pcopen,         pcclose,        pcread,         pcwrite,
	nodev,          nulldev,        0,              /* pc = 1 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,              /* vp = 2 */
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT

#if NLP > 0
	lpopen,         lpclose,        nodev,          lpwrite,
	lpioctl,        nulldev,        0,              /* lp = 3 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NDH > 0
	dhopen,         dhclose,        dhread,         dhwrite,
	dhioctl,        dhstop,         dh11,           /* dh = 4 */
#ifdef IPK_SELECT
	ttselect,
#endif IPK_SELECT
#else
#if NDHU > 0
	dhuopen,        dhuclose,       dhuread,        dhuwrite,
	dhuioctl,       dhustop,        dhu11,          /* dhu = 4 */
#ifdef IPK_SELECT
	ttselect,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,              /* ?? = 4 */
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif
#endif

	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,              /* dp = 5 */
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT

#if NDJ > 0
	djopen,         djclose,        djread,         djwrite,
	djioctl,        nulldev,        dj11,           /* dj = 6 */
#ifdef IPK_SELECT
	ttselect,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NDN > 0
	dnopen,         dnclose,        nodev,          dnwrite,
	nodev,          nulldev,        0,              /* dn = 7 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

	nulldev,        nulldev,        mmread,         mmwrite,
	nodev,          nulldev,        0,              /* mem = 8 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT

#if NRK > 0
	rkopen,         rkclose,        rkread,         rkwrite,
	nodev,          nulldev,        0,              /* rk = 9 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,              /* rf = 10 */
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT

#if NRP > 0
	rpopen,         rpclose,        rpread,         rpwrite,
	nodev,          nulldev,        0,              /* rp = 11 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NTM > 0
	tmopen,         tmclose,        tmread,         tmwrite,
	tmioctl,        nulldev,        0,              /* tm = 12 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NHS > 0
	hsopen,         hsclose,        hsread,         hswrite,
	nodev,          nulldev,        0,              /* hs = 13 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NXP > 0
	xpopen,         xpclose,        xpread,         xpwrite,
	nodev,          nulldev,        0,              /* xp = 14 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
#if NHP > 0

	hpopen,         hpclose,        hpread,         hpwrite,
	nodev,          nulldev,        0,              /* hp = 14 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
#if NRM > 0
	rmopen,         rmclose,        rmread,         rmwrite,
	nodev,          nulldev,        0,              /* rm = 14 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif
#endif
#endif

#if NHT > 0
	htopen,         htclose,        htread,         htwrite,
	htioctl,        nulldev,        0,              /* ht = 15 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,              /* du = 16 */
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT

	syopen,         nulldev,        syread,         sywrite,
	sysioctl,       nulldev,        0,              /* tty = 17 */
#ifdef IPK_SELECT
	ttselect,
#endif IPK_SELECT

#if NRL > 0
	rlopen,         rlclose,        rlread,         rlwrite,
	nodev,          nulldev,        0,              /* rl = 18 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NHK > 0
	hkopen,         hkclose,        hkread,         hkwrite,
	nodev,          nulldev,        0,              /* hk = 19 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NTS > 0
	tsopen,         tsclose,        tsread,         tswrite,
	tsioctl,        nulldev,        0,              /* ts = 20 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NDZ > 0
	dzopen,         dzclose,        dzread,         dzwrite,
	dzioctl,        dzstop,         dz11,           /* dz = 21 */
#ifdef IPK_SELECT
	ttselect,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NRUBIN > 0
	ilopen,         ilclose,        ilread,         ilwrite,
	ilioctl,        nulldev,        0,              /* lock = 22 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NVI > 0
	viopen,         viclose,        viread,         viwrite,
	viioctl,        nulldev,        vi11,           /* vi = 23 */
#ifdef IPK_SELECT
	ttselect,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NRW > 0
	rwopen,         rwclose,        rwread,         rwwrite,
	nodev,          nulldev,        0,              /* rw = 24 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NKX > 0
	kxopen,         kxclose,        kxread,         kxwrite,
	kxioctl,        nulldev,        kx11,           /* kx = 25 */
#ifdef IPK_SELECT
	ttselect,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NRX > 0
	rxopen,         rxclose,        rxread,         rxwrite,
	nodev,          nulldev,        0,              /* rx = 26 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NFD > 0
	fdopen,         fdclose,        fdread,         fdwrite,
	nodev,          nulldev,        0,              /* fd = 27 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NVD > 0
	vdopen,         vdclose,        vdread,         vdwrite,
	nodev,          nulldev,        0,              /* vd = 28 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NKY > 0
	kyopen,         kyclose,        kyread,         kywrite,
	kyioctl,        nulldev,        ky11,           /* ky = 29 */
#ifdef IPK_SELECT
	ttselect,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NRA > 0
	udopen,         udclose,        udread,         udwrite,
	nodev,          nulldev,        0,              /* ra = 30 */
#ifdef IPK_SELECT
	seltrue,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif

#if NPTY > 0
	ptcopen,        ptcclose,       ptcread,        ptcwrite,
	ptyioctl,       nulldev,        0,              /* ptc = 31 (pty) */
#ifdef IPK_SELECT
	ptcselect,
#endif IPK_SELECT
	ptsopen,        ptsclose,       ptsread,        ptswrite,
	ptyioctl,       nulldev,        0,              /* pts = 32 (pty) */
#ifdef IPK_SELECT
	ttselect,
#endif IPK_SELECT
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          0,
#ifdef IPK_SELECT
	nodev,
#endif IPK_SELECT
#endif
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

#ifdef  UCB_NTTY
int     ntyopen(), ntyclose(), ntread(), ntyinput(), ntyoutput();
caddr_t ntwrite();
#define ntyioctl        nullioctl
#define ntymodem        nulldev
#endif

struct  linesw linesw[] =
{
#ifdef OLDTTY
	ttopen,         ttclose,        ttread,         ttwrite,
	ttioctl,        ttyinput,       ttyoutput,      ttmodem,        /*0*/
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          nodev,          nodev,          /*0*/
#endif OLDTTY

#ifdef  UCB_NTTY
	ntyopen,        ntyclose,       ntread,         ntwrite,
	ntyioctl,       ntyinput,       ntyoutput,      ntymodem,       /*1*/
#else
	nodev,          nodev,          nodev,          nodev,
	nodev,          nodev,          nodev,          nodev,          /*1*/
#endif UCB_NTTY

#if     NBK > 0
	bkopen,         bkclose,        bkread,         ttwrite,
	bkioctl,        bkinput,        nodev,          nulldev         /*2*/
#endif
};

#ifndef MPX_FILS
int     nldisp  = sizeof(linesw) / sizeof(linesw[0]);
#else
int     nldisp  = sizeof(linesw) / sizeof(linesw[0]) - 1;
int     mpxchan();
int     (*ldmpx)()      = mpxchan;
#endif

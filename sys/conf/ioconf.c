/*
 * Конфигурация внешней памяти
 *
 * $Log:	ioconf.c,v $
 * Revision 1.11  91/01/11  17:56:23  korotaev
 * Изменение нек-ых умолчаний в размере разделов.
 * 
 * Revision 1.10  90/11/12  18:52:14  root
 * Новые вещи для СМ 1425 и перемещение некоторых include'ов.
 * 
 * Revision 1.9  89/06/16  14:30:18  korotaev
 * Не был описан nulldev(). 
 * 
 * Revision 1.8  88/11/16  14:02:47  dvolodin
 * *** empty log message ***
 *
 * Revision 1.7  88/09/14  18:43:33  korotaev
 * Изменены разделы hk.
 *
 * Revision 1.6  88/07/25  19:53:31  korotaev
 * Исправлено деление дисков hp (надо чтобы 100 Мбайтники делились
 * на поддиски меньше 65 Мбайт, чтобы адрес был int).
 *
 * Revision 1.5  88/07/07  15:34:35  korotaev
 * Размеры разеделов xp похожи на размеры rp и hk.
 *
 * Revision 1.4  88/03/23  14:11:55  korotaev
 * Состояние после слияния с АЗЛК, Э-85 и Бурковским планировщиком
 *
 * Revision 1.3  88/01/04  20:38:07  korotaev
 * Слияние версий с АЗЛК.
 *
 * Revision 1.2  86/12/11  21:09:32  alex
 * Изменено деление на части дисков ЕС-5051 (rp).
 *
 * Revision 1.1  86/04/19  15:02:20  avg
 * Initial revision
 *
 */

#include        <sys/param.h>
#include        <sys/systm.h>
#include        "h/dw.h"
#include        "h/fd.h"
#include        "h/hk.h"
#include        "h/hp.h"
#include        "h/hs.h"
#include        "h/kx.h"
#include        "h/ky.h"
#include        "h/my.h"
#include        "h/ra.h"
#include        "h/rk.h"
#include        "h/rl.h"
#include        "h/rm.h"
#include        "h/rp.h"
#include        "h/rw.h"
#include        "h/rx.h"
#include        "h/xp.h"

int nulldev();

dev_t   rootdev = %ROOTDEV%;
dev_t   swapdev = %SWAPDEV%;
dev_t   pipedev = %PIPEDEV%;
daddr_t swplo   = (daddr_t) %SWPLO%;
int     nswap   = %NSWAP%;

#ifdef  UCB_AUTOBOOT
/*
 * If dumpdev is NODEV, no dumps will be taken automatically.
 * dump can point to nodev, just to resolve the reference.
 */
dev_t   dumpdev = %DUMPDEV%;
daddr_t dumplo  = (daddr_t) %DUMPLO%;
int     %DUMPROUTINE%();
int     (*dump)()       = %DUMPROUTINE%;
#endif  UCB_AUTOBOOT

/*
 *      Device configuration information.
 */

#if     NHK > 0
struct  hkdevice *HKADDR = 0177440;
struct  size hk_sizes[] = {
	5940,   0,              /* cyl   0 - 89 */
	5280,   90,             /* cyl  90 - 169 */
	41910,  170,            /* cyl 170 - 814 */
	15906,  170,            /* cyl 170 - 410 */
	0,      0,
	0,      0,
	27126,  0,              /* cyl   0 - 410, whole RK06 */
	53790,  0               /* cyl   0 - 814, whole RK07 */
};
#endif  NHK

#if     NHP > 0
struct  hpdevice *HPADDR = 0176700;
#endif
#if     NHP > 0 || NXP > 0
struct  size hp_sizes[] = {
	5852,   0,              /* cyl   0 - 13 */
	5434,   14,             /* cyl  14 - 26 */
	160512, 27,             /* cyl  27 - 410 (rp04, rp05, rp06) */
	168872, 411,            /* cyl 411 - 814 (rp06) */
	80256,  27,             /* cyl  27 - 218 (rp04/5) */
	80256,  219,            /* cyl 219 - 410 (rp04/5) */
	171798, 0,              /* cyl   0 - 410 (whole rp04/5) */
	340670, 0               /* cyl   0 - 814 (whole rp06) */
};
#endif  NHP

#if     NHS > 0
struct  hsdevice *HSADDR = 0172040;
#endif  NHS

struct  dldevice *KLADDR = 0177560;

#if     NKX > 0
struct  kxdevice *KXADDR = 0173400;
#endif  NKX

#if     NKY > 0
struct  kydevice *KYADDR = 0173300;
#endif  NKY

#if     NRX > 0
struct  rkdevice *RXADDR = 0177170;
#endif  NRX

#if     NRK > 0
struct  rkdevice *RKADDR = 0177400;
#endif  NRK

#if     NRL > 0
struct  rldevice *RLADDR = 0174400;
#endif  NRL

/* Винчестер Эл.85 и ДВК-4 имеют похожий контроллер */
#if     NRW > 0
struct  rwdevice *RWADDR = 0174000;
#else
#if     NDW > 0
struct  dwdevice *DWADDR = 0174000;
#endif  NDW
#endif  NRW

/* Гибкий диск MY (для Qbus) */
#if     NMY > 0
struct  mydevice *MYADDR = 0172140;
#endif

#if     NFD > 0
struct  fddevice *FDADDR = 0174200;
#endif

#if     NRM > 0
struct  rmdevice *RMADDR = 0176700;
#endif

#if     NRM > 0 || NXP > 0
struct  size rm_sizes[] = {
	4800,   0,              /* cyl   0 -  29 */
	4800,   30,             /* cyl  30 -  59 */
	122080, 60,             /* cyl  60 - 822 */
	62720,  60,             /* cyl  60 - 451 */
	59360,  452,            /* cyl 452 - 822 */
	0,      0,
	0,      0,
	131680, 0,              /* cyl   0 - 822 */
};
#endif  NRM

#if     NRP > 0
struct  rpdevice *RPADDR = 0176710;
struct  size rp_sizes[] = {
	 6000, 0,              /* cyl   0 -  29 */
	 5000,  30,             /* cyl  30 -  54 */
	 29600, 55,             /* cyl  56 - 202 */
	 0,     0,
	 0,     0,
	 0,     0,
	 0,     0,
	 40600, 0               /* cyl   0 - 202 */
};
#endif  NRP

#if     NXP > 0
#include "../include/hpreg.h"

/* RM05 */
struct  size rm5_sizes[] = {
	9120,     0,            /* cyl   0 -  14 */
	9120,    15,            /* cyl  15 -  29 */
	234080,  30,            /* cyl  30 - 414 */
	248064, 415,            /* cyl 415 - 822 */
	164160,  30,            /* cyl  30 - 299 */
	152000, 300,            /* cyl 300 - 549 */
	165376, 550,            /* cyl 550 - 822 */
	500384,   0             /* cyl   0 - 822 */
};

/* SI Eagle */
struct  size si_sizes[] = {
	11520,    0,            /* cyl   0 -  11 */
	11520,   12,            /* cyl  12 -  23 */
	474240,  24,            /* cyl  24 - 517 */
	92160,  518,            /* cyl 518 - 613 */
	214080, 614,            /* cyl 614 - 836, reserve 5 cyls */
	0,        0,
	0,        0,
	808320,   0             /* cyl   0 - 841 (everything) */
};

/* Diva Comp V + Ampex 9300 in direct mode */
struct  size dv_sizes[] = {
	9405,     0,            /* cyl   0 -  14 */
	9405,    15,            /* cyl  15 -  29 */
	241395,  30,            /* cyl  30 - 414 */
	250800, 415,            /* cyl 415 - 814 */
	169290,  30,            /* cyl  30 - 299 */
	156750, 300,            /* cyl 300 - 549 */
	165528, 550,            /* cyl 550 - 814 */
	511005,   0             /* cyl   0 - 814 */
};

/*
 *  Xp_controller structure: one line per controller.
 *  Only the address need be initialized in the controller structure
 *  if XP_PROBE is defined (at least the address for the root device);
 *  otherwise the flags must be here also.
 *  The XP_NOCC flag is set for RM02/03/05's (with no current cylinder
 *  register); XP_NOSEARCH is set for Diva's without the search command.
 *  The XP_RH70 flag need not be set here, the driver will always check that.
 */
struct  xp_controller xp_controller[NXP_CONTROLLER] = {
/*      0       0       addr      flags                 0 */
	0,      0,      0176700, XP_NOCC|XP_NOSEARCH,   0
};

/*
 *  Xp_drive structure: one entry per drive.
 *  The drive structure must be initialized if XP_PROBE is not enabled.
 *  Macros are provided in hpreg.h to initialize entries according
 *  to drive type, and controller and drive numbers.
 *  See those for examples on how to set up other types of drives.
 *  With XP_PROBE defined, xpslave will fill in this structure,
 *  and any initialization will be overridden.  There is one exception;
 *  if the drive-type field is set, it will be used instead of the
 *  drive-type register to determine the drive's type.
 */
struct  xp_drive xp_drive[NXP] = {
#ifndef XP_PROBE
	RM02_INIT(0,0),         /* RM02, controller 0, drive 0 */
	RM02_INIT(0,1),         /* RM02, controller 0, drive 1 */
	RM05X_INIT(0,2)         /* 815-cyl RM05, controller 0, drive 2 */
#endif  XP_PROBE
};
#endif  NXP

#if     NRA  > 0
#include "../include/ra_info.h"

struct  rasize  ra_sizes[] = {
	 9000,  0,              /* A=blk 0 thru 8999 */
	12600,  9000,           /* B=blk 9000 thru 21599 */
	38880,  21600,          /* C=blk 21600 thru 60479 */
	-1,     60480,          /* D=blk 60480 thru end */
	720,    0,              /* E=blk 0 thru 720 */
	800,    0,              /* F=blk 0 thru 800 */
	21600,  0,              /* G=blk 0 thru 21599 */
	-1,     0               /* H=blk 0 thru end */
};
struct  ra_regs *RAADDR = 0172150;
int     RA_VEC = 0154;
#endif  NRA

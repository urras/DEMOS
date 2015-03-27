/*
 * SCCSID: @(#)ra_info.h        1.3     02/03/87
 */
/*
 * MSCP disk controller common information header file.
 *
 * A. Skukin (azlk)
 *
 * This header file should be included in any program that
 * deals with MSCP disks (RA, RC, RD, RX).
 * It defines all of the data structures used by the MSCP
 * disk driver (uda.c).
 *
 */

#include "../include/mscp.h"

#define MAXUDA  4

/*
 * Macros for disecting a major/minor device number
 * into controller, unit, and major device.
 */
#define ractrl(x)       (int)((x>>6)&03)
#define ramajor(x)      (int)((x>>8)&0377)
#define raunit(x)       (int)((x>>3)&07)
#define raintrlv(x)     (int)((x)&0200)

/*
 * Drive tpyes.
 */
#define RC25    25
#define RX50    50
#define RD51    51
#define RD52    52
#define RA60    60
#define RA80    80
#define RA81    81

/*
 * UQPORT registers and structures
 */

struct  ra_regs {
	int     raaip;          /* initialization and polling */
	int     raasa;          /* status and address */
};

#define UDA_ERR         0100000 /* error bit */
#define UD_STEP4        0040000 /* step 4 has started */
#define UD_STEP3        0020000 /* step 3 has started */
#define UD_STEP2        0010000 /* step 2 has started */
#define UD_STEP1        0004000 /* step 1 has started */
#define UDA_NV          0002000 /* no host settable interrupt vector */
#define UDA_QB          0001000 /* controller supports Q22 bus */
#define UDA_DI          0000400 /* controller implements diagnostics */
#define UDA_IE          0000200 /* interrupt enable */
#define UDA_PI          0000001 /* host requests adapter purge interrupts */
#define UDA_GO          0000001 /* start operation, after init */

/*
 * Parameters for the communications area.
 *
 * The UDA50 has 12 command buffers and the enhanced UDA50
 * has 20, the general consensus seems to be that a
 * command/response ring size of eight is optimum.
 * This could change later if experience shows otherwise,
 * use 4 on non-separate I & D space processors.
 *
 * The UDA50 has 12 data xfer buffers the enhanced UDA50 has 51.
 */

/*#ifdef  MENLO_KOV               /* overlay kernel, small CPU less packets */
/*#define NRSPL2  2               /* log2 number of response packets */
/*#define NCMDL2  2               /* log2 number of command packets */
/*#else */
#define NRSPL2  3               /* log2 number of response packets */
#define NCMDL2  3               /* log2 number of command packets */
/*#endif  MENLO_KOV     */
#define NRSP    (1<<NRSPL2)
#define NCMD    (1<<NCMDL2)

/*
 * UDA Communications Area
 */

struct udaca {
	short   ca_xxx1;        /* unused */
	char    ca_xxx2;        /* unused */
	char    ca_bdp;         /* BDP to purge */
	short   ca_cmdint;      /* command queue transition interrupt flag */
	short   ca_rspint;      /* response queue transition interrupt flag */
	struct {
		unsigned int rl;
		unsigned int rh;
	} ca_rspdsc[NRSP];      /* response descriptors */
	struct {
		unsigned int cl;
		unsigned int ch;
	} ca_cmddsc[NCMD];      /* command descriptors */
};

#define ca_ringbase     ca_rspdsc[0].rl

#define UDA_OWN 0100000 /* UDA owns this descriptor */
#define UDA_INT 0040000 /* allow interrupt on ring transition */

struct ra_drv {                 /* RA drive information */
	char    ra_dt;          /* RA drive type, 60, 80, 81, 0 = NODRIVE */
	char    ra_online;      /* RA drive on-line flag */
	union {
		daddr_t ra_dsize;       /* RA drive size (# of LBN's) */
		struct {
			int     ra_dslo;
			int     ra_dshi;
		};
	} d_un;
				/* , from ONLINE end packet */
};

struct uda_softc {
	short   sc_state;       /* state of controller */
	short   sc_credits;     /* transfer credits */
	short   sc_tcmax;       /* transfer credit limit */
	short   sc_lastcmd;     /* pointer into command ring */
	short   sc_lastrsp;     /* pointer into response ring */
};

/*
 * Controller states
 */
#define S_IDLE  0               /* hasn't been initialized */
#define S_STEP1 1               /* doing step 1 init */
#define S_STEP2 2               /* doing step 2 init */
#define S_STEP3 3               /* doing step 3 init */
#define S_SCHAR 4               /* doing "set controller characteristics" */
#define S_RUN   5               /* running */

struct uda {
	struct udaca    uda_ca;         /* communications area */
	struct mscp     uda_rsp[NRSP];  /* response packets */
	struct mscp     uda_cmd[NCMD];  /* command packets */
};

/*
 * The maintenance area size (ra_mas) is setup at controller
 * initialization time. Some of the partition sizes are changed,
 * if the controller is an RQDX1.
 *
 * A size of -1 says the partition extends to the end of
 * the disk.
 */

struct rasize {
	daddr_t nblocks;
	daddr_t blkoff;
};

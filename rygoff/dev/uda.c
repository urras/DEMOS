/*      uda.c   1.0     (azlk)  87/04/01        */

#include "ra.h"
#if NRA > 0
/*
 * UDA50/RAxx disk device driver
 *
 * Restrictions:
 *	Unit numbers must be less than 8.
 *	Driver support ONE controller.
 *      Driver DO NOT support UNIBUS machines... sorry!
 *
 * Author: A. Skukin (av)
 *
 * $Log:	uda.c,v $
 * Revision 1.1  90/03/02  09:40:39  rygoff
 * Initial revision
 * 
 * Revision 1.1  88/01/04  20:04:18  korotaev
 * Initial revision
 * 
 */

#include "param.h"
#include <sys/systm.h>
#include <sys/buf.h>
#include <sys/dir.h>
#include <sys/conf.h>
#include <sys/file.h>
#include <sys/user.h>
#include <sys/map.h>
#ifndef	INTRLVE
#include <sys/inline.h>
#endif

#include <sys/ra_info.h>

struct uda_softc uda_softc;

struct uda uda;

/* THIS SHOULD BE READ OFF THE PACK, PER DRIVE */
extern struct rasize ra_sizes[];
/* END OF STUFF WHICH SHOULD BE READ IN PER DISK */

int	udaerror = 0;			/* causes hex dump of packets */
int	udadebug = 0;
#define	printd	if (udadebug) printf
/* #define notdef  /* Print unit online information        */

struct ra_drv radrv[NRA];		/* RA drive information		*/
					/* drv type, on-line flag, size */

struct	mscp *udgetcp();

struct  ra_regs *RAADDR;
extern  int     RA_VEC;

struct	buf rudbuf[NRA];
struct	buf udutab[NRA];
struct	buf udwtab;		/* I/O wait queue, per controller */
struct	buf udtab;		/* I/O queue	*/
#define	b_qsize		b_resid		/* queue size per drive, in udutab */
#define	b_ubinfo	b_resid		/* Unibus mapping info, per buffer */

udattach(addr, unit)
struct ra_regs *addr;	
{
	if(unit != 0)
		return(0);
	RAADDR = addr;
	return(1);
}

/*
 * Open a UDA.  Initialize the device and
 * set the unit online.
 */
udopen(dev, flag)
	dev_t dev;
	int flag;
{
	register int unit;
	register struct uda_softc *sc;
	int s;

#ifdef lint
	flag = flag;
#endif
	unit = raunit(dev);
	if (RAADDR == (struct ra_regs *) NULL || unit >= NRA) {
		u.u_error = ENXIO;
		return;
	}
	sc = &uda_softc;
	s = spl5();
	if (sc->sc_state != S_RUN) {
		if (sc->sc_state == S_IDLE)
			udinit();
		/* wait for initialization to complete */
		sleep((caddr_t)sc, 0);
		if (sc->sc_state != S_RUN) {
			u.u_error = EIO;
			return;
		}
	}
	splx(s);
	/* SHOULD PROBABLY FORCE AN ONLINE ATTEMPT
	   TO SEE IF DISK IS REALLY THERE */
	return (0);
}

/*
 * Initialize a UDA.  Set up UBA mapping registers,
 * initialize data structures, and start hardware
 * initialization sequence.
 */
udinit()
{
	register struct uda_softc *sc = &uda_softc;
	register struct uda *ud = &uda;
	register struct buf *cp = &udtab;
	struct ra_regs	*udaddr = (struct ra_regs *)RAADDR;
	int i;

	for(i=0; i < NRA; i++)
		radrv[i].d_un.ra_dsize = (daddr_t)0xffffff;

	cp->b_active++;

	/*
	 * Start the hardware initialization sequence.
	 */
	udaddr->raaip = 0;		/* start initialization */
	while ((udaddr->raasa & UD_STEP1) == 0)
		;
	udaddr->raasa = UDA_ERR|(NCMDL2<<11)|(NRSPL2<<8)|UDA_IE|(RA_VEC/4);
	/*
	 * Initialization continues in interrupt routine.
	 */
	sc->sc_state = S_STEP1;
	sc->sc_credits = 0;
}

udstrategy(bp)
	register struct buf *bp;
{
	register struct buf *cp = &udtab;
	register struct buf *dp;
	register struct ra_drv *ui;
	int unit;
	int xunit = minor(bp->b_dev) & 07;
	daddr_t sz, maxsz;
	int s;

	sz = (bp->b_bcount+511) >> 9;
	unit = dkunit(bp);
	if (unit >= NRA) {
		bp->b_error = ENXIO;
		goto bad;
	}
	ui = &radrv[unit];
	if ((maxsz = ra_sizes[xunit].nblocks) < 0)
		maxsz = ui->d_un.ra_dsize - ra_sizes[xunit].blkoff;
	if (bp->b_blkno < 0 || bp->b_blkno+sz > maxsz ||
	    ra_sizes[xunit].blkoff >= ui->d_un.ra_dsize) {
		bp->b_error = EINVAL;
		goto bad;
	}
#ifdef UNIBUS_MAP
	mapalloc(bp);
#endif UNIBUS_MAP
	s = spl5();
	/*
	 * Link the buffer onto the drive queue
	 */
	dp = &udutab[unit];
	if (dp->b_actf == 0)
		dp->b_actf = bp;
	else
		dp->b_actl->av_forw = bp;
	dp->b_actl = bp;
	bp->av_forw = 0;
	/*
	 * Link the drive onto the controller queue
	 */
	if (dp->b_active == 0) {
		dp->b_forw = NULL;
		if (cp->b_actf == NULL)
			cp->b_actf = dp;
		else
			cp->b_actl->b_forw = dp;
		cp->b_actl = dp;
		dp->b_active = 1;
	}
	if (cp->b_active == 0)
		(void) udstart();
	splx(s);
	return;

bad:
	bp->b_flags |= B_ERROR;
	iodone(bp);
	return;
}

udstart()
{
	register struct buf *bp, *dp;
	register struct mscp *mp;
	register struct	buf  *cp = &udtab;
	struct uda_softc *sc = &uda_softc;
	struct ra_drv *ui;
	struct ra_regs	*udaddr = (struct ra_regs *)RAADDR;
	int i, unit;
	daddr_t	lbn;

loop:
	if ((dp = cp->b_actf) == NULL) {
		/*
		 * Release uneeded UBA resources and return
		 */
		cp->b_active = 0;
		return (0);
	}
	if ((bp = dp->b_actf) == NULL) {
		/*
		 * No more requests for this drive, remove
		 * from controller queue and look at next drive.
		 * We know we're at the head of the controller queue.
		 */
		dp->b_active = 0;
		cp->b_actf = dp->b_forw;
		goto loop;
	}
	cp->b_active++;
	if ((udaddr->raasa&UDA_ERR) || sc->sc_state != S_RUN) {
		printf("ra%d: hard error bn%d\n", minor(bp->b_dev) & 07,
			bp->b_blkno);
		printf("raasa %o, state %d\n", udaddr->raasa&0xffff, sc->sc_state);
		udinit();
		/* SHOULD REQUEUE OUTSTANDING REQUESTS, LIKE UDRESET */
		return (0);
	}
	unit = dkunit(bp);
	ui = &radrv[unit];
	/*
	 * If no credits, can't issue any commands
	 * until some outstanding commands complete.
	 */
	if (sc->sc_credits < 2)
		return (0);
	if ((mp = udgetcp()) == NULL)
		return (0);
	sc->sc_credits--;	/* committed to issuing a command */
	if (ui->ra_online == 0) {	/* not online */
		mp->m_opcode = M_O_ONLIN;
		mp->m_unit = unit;
		dp->b_active = 2;
		cp->b_actf = dp->b_forw;	/* remove from controller q */
		printd("uda: bring unit %d online\n", unit);
		*((int *)mp->m_dscptr) |= UDA_OWN|UDA_INT;
		i = udaddr->raaip;
		goto loop;
	}
	mp->m_cmdref = (unsigned)bp;	/* pointer to get back */
	mp->m_opcode = bp->b_flags&B_READ ? M_O_READ : M_O_WRITE;
	mp->m_unit = unit;
	lbn = bp->b_blkno + ra_sizes[minor(bp->b_dev)&7].blkoff;
	mp->m_lbn_l = (short)lbn;
	mp->m_lbn_h = (short)(lbn >> 16);
	mp->m_bytecnt = bp->b_bcount;
	mp->m_buf_l = (unsigned)bp->b_un.b_addr;
	mp->m_buf_h = bp->b_xmem;
	*((int *)mp->m_dscptr) |= UDA_OWN|UDA_INT;
	i = udaddr->raaip;		/* initiate polling */

	/*
	 * Move drive to the end of the controller queue
	 */
	if (dp->b_forw != NULL) {
		cp->b_actf = dp->b_forw;
		cp->b_actl->b_forw = dp;
		cp->b_actl = dp;
		dp->b_forw = NULL;
	}
	/*
	 * Move buffer to I/O wait queue
	 */
	dp->b_actf = bp->av_forw;
	dp = &udwtab;
	bp->av_forw = dp;
	bp->av_back = dp->av_back;
	dp->av_back->av_forw = bp;
	dp->av_back = bp;
	goto loop;
}

/*
 * UDA interrupt routine.
 */
udintr()
{
	register struct buf	*cp = &udtab;
	register struct ra_regs *udaddr = (struct ra_regs *)RAADDR;
	struct buf *bp;
	int i;
	register struct uda_softc *sc = &uda_softc;
	register struct uda *ud = &uda;
	struct mscp *mp;

	printd("udintr: state %d, raasa %o\n", sc->sc_state, udaddr->raasa);
	switch (sc->sc_state) {
	case S_IDLE:
		printf("uda0: random interrupt ignored\n");
		return;

	case S_STEP1:
#define	STEP1MASK	0174377
#define	STEP1GOOD	(UD_STEP2|UDA_IE|(NCMDL2<<3)|NRSPL2)
		if ((udaddr->raasa&STEP1MASK) != STEP1GOOD) {
			sc->sc_state = S_IDLE;
			wakeup((caddr_t)sc);
			return;
		}
		udaddr->raasa = &ud->uda_ca.ca_ringbase;
		sc->sc_state = S_STEP2;
		return;

	case S_STEP2:
#define	STEP2MASK	0174377
#define STEP2GOOD       (UD_STEP3|UDA_IE|(RA_VEC/4))
		if ((udaddr->raasa&STEP2MASK) != STEP2GOOD) {
			sc->sc_state = S_IDLE;
			wakeup((caddr_t)sc);
			return;
		}
		udaddr->raasa = 0;
		sc->sc_state = S_STEP3;
		return;

	case S_STEP3:
#define	STEP3MASK	0174000
#define	STEP3GOOD	UD_STEP4
		if ((udaddr->raasa&STEP3MASK) != STEP3GOOD) {
			sc->sc_state = S_IDLE;
			wakeup((caddr_t)sc);
			return;
		}
		udaddr->raasa = UDA_GO;
		sc->sc_state = S_SCHAR;
		/*
		 * Initialize the data structures.
		 */
		for (i = 0; i < NRSP; i++) {
			ud->uda_ca.ca_rspdsc[i].rl = &ud->uda_rsp[i].m_cmdref;
			ud->uda_ca.ca_rspdsc[i].rh = UDA_OWN|UDA_INT;
			ud->uda_rsp[i].m_dscptr = &ud->uda_ca.ca_rspdsc[i].rh;
			ud->uda_rsp[i].m_header.uda_msglen = sizeof (struct mscp);
		}
		for (i = 0; i < NCMD; i++) {
			ud->uda_ca.ca_cmddsc[i].cl = &ud->uda_cmd[i].m_cmdref;
			ud->uda_ca.ca_cmddsc[i].ch = UDA_INT;
			ud->uda_cmd[i].m_dscptr = &ud->uda_ca.ca_cmddsc[i].ch;
			ud->uda_cmd[i].m_header.uda_msglen = sizeof (struct mscp);
		}
		bp = &udwtab;
		bp->av_forw = bp->av_back = bp;
		sc->sc_lastcmd = 0;
		sc->sc_lastrsp = 0;
		if ((mp = udgetcp()) == NULL) {
			sc->sc_state = S_IDLE;
			wakeup((caddr_t)sc);
			return;
		}
		mp->m_opcode = M_O_STCON;
		mp->m_cntflgs = M_C_ATTN|M_C_MISC|M_C_THIS;
		*((int *)mp->m_dscptr) |= UDA_OWN|UDA_INT;
		i = udaddr->raaip;	/* initiate polling */
		return;

	case S_SCHAR:
	case S_RUN:
		break;

	default:
		printf("uda0: interrupt in unknown state %d ignored\n",
			sc->sc_state);
		return;
	}

	if (udaddr->raasa&UDA_ERR) {
		printf("uda0: fatal error (%o)\n", udaddr->raasa&0xffff);
		udaddr->raaip = 0;
		wakeup((caddr_t)sc);
	}

	/*
	 * Check for response ring transition.
	 */
	if (ud->uda_ca.ca_rspint) {
		ud->uda_ca.ca_rspint = 0;
		for (i = sc->sc_lastrsp;; i++) {
			i %= NRSP;
			if (ud->uda_ca.ca_rspdsc[i].rh&UDA_OWN)
				break;
			udrsp(cp, ud, sc, i);
			ud->uda_ca.ca_rspdsc[i].rh |= UDA_OWN;
		}
		sc->sc_lastrsp = i;
	}

	/*
	 * Check for command ring transition.
	 */
	if (ud->uda_ca.ca_cmdint) {
		printd("uda: command ring transition\n");
		ud->uda_ca.ca_cmdint = 0;
	}
	(void) udstart();
}

/*
 * Process a response packet
 */
udrsp(cp, ud, sc, i)
	register struct buf *cp;
	register struct uda *ud;
	register struct uda_softc *sc;
	int i;
{
	register struct mscp *mp;
	struct ra_drv *ui;
	struct buf *dp, *bp;
	int st;

	mp = &ud->uda_rsp[i];
	mp->m_header.uda_msglen = sizeof (struct mscp);
	sc->sc_credits += mp->m_header.uda_credits & 0xf;
	if ((mp->m_header.uda_credits & 0xf0) > 0x10)
		return;
	/*
	 * If it's an error log message (datagram),
	 * pass it on for more extensive processing.
	 */
	if ((mp->m_header.uda_credits & 0xf0) == 0x10) {
		uderror((struct mslg *)mp);
		return;
	}
	if (mp->m_unit >= 8)
		return;
	if ((ui = &radrv[mp->m_unit]) == 0)
		return;
	st = mp->m_status&M_S_MASK;
	switch (mp->m_opcode & 0377) {
	case M_O_STCON|M_O_END:
		if (st == M_S_SUCC)
			sc->sc_state = S_RUN;
		else
			sc->sc_state = S_IDLE;
		cp->b_active = 0;
		wakeup((caddr_t)sc);
		break;

	case M_O_ONLIN|M_O_END:
		/*
		 * Link the drive onto the controller queue
		 */
		dp = &udutab[mp->m_unit];
		dp->b_forw = NULL;
		if (cp->b_actf == NULL)
			cp->b_actf = dp;
		else
			cp->b_actl->b_forw = dp;
		cp->b_actl = dp;
		if (st == M_S_SUCC) {
			ui->ra_online = 1;	/* mark it online */
			ui->d_un.ra_dslo = mp->m_ushigh;
			ui->d_un.ra_dshi = mp->m_uslow;
			printd("uda: unit %d online ", mp->m_unit);
			printd("--> size = %D\n", (daddr_t)ui->d_un.ra_dsize);
#ifdef notdef
			printf("uda: unit %d online ", mp->m_unit);
			printf("--> size = %D\n", (daddr_t)ui->d_un.ra_dsize);
#endif
		} else {
			printf("ra%d: ", minor(dp->b_actf->b_dev) & 07);
			printf("OFFLINE\n");
			while (bp = dp->b_actf) {
				dp->b_actf = bp->av_forw;
				bp->b_flags |= B_ERROR;
				iodone(bp);
			}
		}
		dp->b_active = 1;
		break;

	case M_O_AVATN:
		printd("uda: unit %d attention\n", mp->m_unit);
		ui->ra_online = 0;	/* it went offline and we didn't notice */
		break;

	case M_O_READ|M_O_END:
	case M_O_WRITE|M_O_END:
		bp = (struct buf *)mp->m_cmdref;
		/*
		 * Unlink buffer from I/O wait queue.
		 */
		bp->av_back->av_forw = bp->av_forw;
		bp->av_forw->av_back = bp->av_back;
		dp = &udutab[mp->m_unit];
		if (st == M_S_OFFLN || st == M_S_AVLBL) {
			ui->ra_online = 0;	/* mark unit offline */
			/*
			 * Link the buffer onto the front of the drive queue
			 */
			if ((bp->av_forw = dp->b_actf) == 0)
				dp->b_actl = bp;
			dp->b_actf = bp;
			/*
			 * Link the drive onto the controller queue
			 */
			if (dp->b_active == 0) {
				dp->b_forw = NULL;
				if (cp->b_actf == NULL)
					cp->b_actf = dp;
				else
					cp->b_actl->b_forw = dp;
				cp->b_actl = dp;
				cp->b_active = 1;
			}
			return;
		}
		if (st != M_S_SUCC) {
			printf("ra%d: error ", minor(bp->b_dev) & 07);
			printf("status %o ", mp->m_status);
			bp->b_flags |= B_ERROR;
		}
		bp->b_resid = bp->b_bcount - mp->m_bytecnt;
		iodone(bp);
		break;

	case M_O_GTUNT|M_O_END:
		break;

	default:
		printf("uda: unknown packet\n");
	}
}


/*
 * Process an error log message
 *
 * For now, just log the error on the console.
 * Only minimal decoding is done, only "useful"
 * information is printed.  Eventually should
 * send message to an error logger.
 */
uderror(mp)
	register struct mslg *mp;
{
	printf("uda0: %s error, ",
		mp->me_flags&M_LF_SUCC ? "soft" : "hard");
	switch (mp->me_format & 0377) {
	case M_F_CNTERR:
		printf("controller error, event 0%o\n", mp->me_event);
		break;

	case M_F_BUSADDR:
		printf("host memory access error, event 0%o, addr 0%o\n",
			mp->me_event, mp->me_busaddr);
		break;

	case M_F_DISKTRN:
		printf("disk transfer error, unit %d, grp 0x%x, hdr 0x%x%x\n",
		mp->me_unit, mp->me_group, mp->me_hdr[1], mp->me_hdr[0]);
		break;

	case M_F_SDI:
		printf("SDI error, unit %d, event 0%o, hdr 0x%x%x\n",
		mp->me_unit, mp->me_event, mp->me_hdr[1], mp->me_hdr[0]);
		break;

	case M_F_SMLDSK:
		printf("small disk error, unit %d, event 0%o, cyl %d\n",
			mp->me_unit, mp->me_event, mp->me_sdecyl);
		break;

	default:
		printf("unknown error, unit %d, format 0%o, event 0%o\n",
			mp->me_unit, mp->me_format&0377, mp->me_event);
	}
}


/*
 * Find an unused command packet
 */
struct mscp *
udgetcp()
{
	register struct mscp *mp;
	register struct udaca *cp;
	register struct uda_softc *sc;
	register int i;

	cp = &uda.uda_ca;
	sc = &uda_softc;
	i = sc->sc_lastcmd;
	if ((cp->ca_cmddsc[i].ch & (UDA_OWN|UDA_INT)) == UDA_INT) {
		cp->ca_cmddsc[i].ch &= ~UDA_INT;
		mp = &uda.uda_cmd[i];
		mp->m_unit = mp->m_modifier = 0;
		mp->m_opcode = mp->m_flags = 0;
		mp->m_bytecnt = mp->m_copyspd = 0;
		mp->m_buf_l = mp->m_buf_h = 0;
		mp->m_elgfll = mp->m_elgflh = 0;
		mp->m_cntflgs = 0;
		sc->sc_lastcmd = (i + 1) % NCMD;
		return(mp);
	}
	return(NULL);
}

udread(dev)
	dev_t dev;
{
	physio(udstrategy, &rudbuf[(minor(dev) >> 3) & 07], dev, B_READ);
}

udwrite(dev)
	dev_t dev;
{
	physio(udstrategy, &rudbuf[(minor(dev) >> 3) & 07], dev, B_WRITE);
}

uddump()
{
	u.u_error = ENXIO;
	return(-1);
}
 
udsize(dev)
	dev_t dev;
{
	int unit = minor(dev) >> 3;
	struct ra_drv *ui;

	if (unit >= NRA || (ui = &radrv[unit]) == 0 || ui->ra_online == 0)
		return (-1);
	return (ra_sizes[minor(dev) & 07].nblocks);	/* XXX */
}
#endif NRA

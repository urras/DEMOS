/*
 * standalone IZOT-1370/SM-5400/RK03/RK05 disk driver
 * @ АР 24.09.85 - введено 10 повторов обмена при сбое.
 * $Header: rk.c,v 1.2 86/11/22 21:29:00 alex Exp $
 * $Log:	rk.c,v $
 * Revision 1.2  86/11/22  21:29:00  alex
 * rk.c взят из ДЕМОС 1.3
 * 
 */

#include <sys/param.h>
#include <sys/inode.h>
#include "../saio.h"

#define	RKADDR	((struct device *)0177400)
#define	NRK	4
#define	NRKBLK	4872
#define NSECT	12

#define	RESET	0
#define	WCOM	2
#define	RCOM	4
#define	GO	01
#define	DRESET	014
#define	IENABLE	0100
#define	DRY	0200
#define	ARDY	0100
#define	WLO	020000
#define	CTLRDY	0200

struct	device
{
	int	rkds;
	int	rker;
	int	rkcs;
	int	rkwc;
	caddr_t	rkba;
	int	rkda;
};

rkstrategy(io, func)
register struct iob *io;
{
	register com;
	daddr_t bn;
	int dn, cn, sn, nerr=10;
retop:
	bn = io->i_bn;
	dn = io->i_unit;
	cn = bn/NSECT;
	sn = bn%NSECT;
	RKADDR->rkda = (dn<<13) | (cn<<4) | sn;
	RKADDR->rkba = io->i_ma;
	RKADDR->rkwc = -(io->i_cc>>1);
	com = (segflag<<4)|GO;
	if (func == READ)
		com |= RCOM; else
		com |= WCOM;
	RKADDR->rkcs = com;
	while ((RKADDR->rkcs&CTLRDY) == 0)
		;
	if (RKADDR->rkcs<0) {	/* error bit */
		printf("disk error: cyl=%d, sector=%d, er=%o, ds=%o\n",
		    cn, sn, RKADDR->rker, RKADDR->rkds);
		RKADDR->rkcs = RESET | GO;
		while ((RKADDR->rkcs&CTLRDY) == 0)
			;
		if(nerr--) goto retop;
		printf("fatal!\n");
		return(-1);
	}
	return(io->i_cc);
}

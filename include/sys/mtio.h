#ifndef _sys_mtio_h_
#       define  _sys_mtio_h_

/*
 * Structures and definitions for mag tape io control commands
 */

/* mag tape io control commands */
#define	MTIOCTOP	(('m'<<8)|1)	/* do a mag tape op */
#define	MTIOCGET	(('m'<<8)|2)	/* get mag tape status */
#define MTIOCSET        (('m'<<8)|3)    /* set same regim for mag tape */

/* structure for MTIOCTOP/MTIOCSET - mag tape op command */
struct	mtop	{
	short	mt_op;		/* operations defined below */
	u_short	mt_count;	/* how many of them */
};

/* operations */
#define	MTWEOF	0	/* write an end-of-file record */
#define	MTFSF	1	/* forward space file */
#define	MTBSF	2	/* backward space file */
#define	MTFSR	3	/* forward space record */
#define	MTBSR	4	/* backward space record */
#define	MTREW	5	/* rewind */
#define	MTOFFL	6	/* rewind and put the drive offline */
#define	MTNOP	7	/* no operation, sets status only */

/* settings */
#define MTWXGAP 1       /* next op write with xtnd interrecord gap */

/* structure for MTIOCGET - mag tape get status command */

struct	mtget	{
	short	mt_type;	/* type of magtape device */
/* the following two registers are grossly device dependent */
	u_short	mt_dsreg;	/* ``drive status'' register */
	short	mt_erreg;	/* ``error'' register */
/* end device-dependent registers */
	short	mt_resid;	/* residual count */
	short   mt_gapcnt;      /* сколько при последней операции записи */
				/* выполнялась запись с "xtnd interrecord gap" */
/* the following two are not yet implemented */
	daddr_t	mt_fileno;	/* file number of current position */
	daddr_t	mt_blkno;	/* block number of current position */
/* end not yet implemented */
};

/*
 * Constants for mt_type byte
 */
#define	MT_ISTS		01
#define	MT_ISHT		02
#define	MT_ISTM		03

#ifndef	KERNEL
#define	DEFTAPE	"/dev/nrmt0"
#endif

#endif  _sys_mtio_h_

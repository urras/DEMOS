/*
 * SCCSID: @(#)mscp.h	1.1	10/8/84
 */
/*
 * Definitions for the Mass Storage Control Protocol
 *
 *	Unused mscp definitions are commented out because of the
 *	limited symbol table size in the V7 C compiler,
 *	`too many defines'.
 *	Lots of strangeness in mscp packet definitions, i.e.,
 *	dealing with longs as hi & lo words. This is due to the
 *	way the V7 C compiler stores a long, i.e., high order bits
 *	in low memory word and low order bits in high memory word.
 *	The UDA does not like that at all !
 *
 *	Fred Canter 10/22/83
 */


/*
 * Control message opcodes
 */
#define	M_O_ABORT	0001	/* Abort command */
#define	M_O_GTCMD	0002	/* Get command status command */
#define	M_O_GTUNT	0003	/* Get unit status command */
#define	M_O_STCON	0004	/* Set controller characteristics command */
#define	M_O_SEREX	0007	/* Serious exception end message */
#define	M_O_AVAIL	0010	/* Available command */
#define	M_O_ONLIN	0011	/* Online command */
#define	M_O_STUNT	0012	/* Set unit characteristics command */
#define	M_O_DTACP	0013	/* Determine access paths command */
#define	M_O_ACCES	0020	/* Access command */
#define	M_O_CMPCD	0021	/* Compare controller data command */
#define	M_O_ERASE	0022	/* Erase command */
#define	M_O_FLUSH	0023	/* Flush command */
#define	M_O_REPLC	0024	/* Replace command */
#define	M_O_COMP	0040	/* Compare host data command */
#define	M_O_READ	0041	/* Read command */
#define	M_O_WRITE	0042	/* Write command */
#define	M_O_AVATN	0100	/* Available attention message */
#define	M_O_DUPUN	0101	/* Duplicate unit number attention message */
#define	M_O_ACPTH	0102	/* Access path attention message */
#define	M_O_END		0200	/* End message flag */


/*
 * Generic command modifiers
 */
/*#define	M_M_EXPRS	0100000		/* Express request */
/*#define	M_M_COMP	0040000		/* Compare */
/*#define	M_M_CLSEX	0020000		/* Clear serious exception */
/*#define	M_M_ERROR	0010000		/* Force error */
/*#define	M_M_SCHH	0004000		/* Suppress caching (high speed) */
/*#define	M_M_SCHL	0002000		/* Suppress caching (low speed) */
/*#define	M_M_SECOR	0001000		/* Suppress error correction */
/*#define	M_M_SEREC	0000400		/* Suppress error recovery */
/*#define	M_M_SSHDW	0000200		/* Suppress shadowing */
/*#define	M_M_WBKNV	0000100		/* Write back (non-volatile) */
/*#define	M_M_WBKVL	0000040		/* Write back (volatile) */
/*#define	M_M_WRSEQ	0000020		/* Write shadow set one unit at a time */

/*
 * AVAILABLE command modifiers
 */
/*#define	M_M_ALLCD	0000002		/* All class drivers */
/*#define	M_M_SPNDW	0000001		/* Spin down */

/*
 * FLUSH command modifiers
 */
/*#define	M_M_FLENU	0000001		/* Flush entire unit */
/*#define	M_M_VOLTL	0000002		/* Volatile only */

/*
 * GET UNIT STATUS command modifiers
 */
/*#define	M_M_NXUNT	0000001		/* Next unit */

/*
 * ONLINE command modifiers
 */
/*#define	M_M_RIP		0000001		/* Allow self destruction */
/*#define	M_M_IGNMF	0000002		/* Ignore media format error */

/*
 * ONLINE and SET UNIT CHARACTERISTICS command modifiers
 */
/*#define	M_M_ALTHI	0000040		/* Alter host identifier */
/*#define	M_M_SHDSP	0000020		/* Shadow unit specified */
/*#define	M_M_CLWBL	0000010		/* Clear write-back data lost */
/*#define	M_M_STWRP	0000004		/* Set write protect */

/*
 * REPLACE command modifiers
 */
/*#define	M_M_PRIMR	0000001		/* Primary replacement block */


/*
 * End message flags
 */
#define	M_E_BBLR	0200	/* Bad block reported */
#define	M_E_BBLU	0100	/* Bad block unreported */
#define	M_E_ERLOG	0040	/* Error log generated */
#define	M_E_SEREX	0020	/* Serious exception */


/*
 * Controller flags
 */
#define	M_C_ATTN	0200	/* Enable attention messages */
#define	M_C_MISC	0100	/* Enable miscellaneous error log messages */
#define	M_C_OTHER	0040	/* Enable other host's error log messages */
#define	M_C_THIS	0020	/* Enable this host's error log messages */
#define	M_C_MLTHS	0004	/* Multi-host */
#define	M_C_SHADW	0002	/* Shadowing */
#define	M_C_576		0001	/* 576 byte sectors */


/*
 * Unit flags
 */
/*#define	M_U_REPLC	0100000		/* Controller initiated bad block replacement */
/*#define	M_U_INACT	0040000		/* Inactive shadow set unit */
/*#define	M_U_WRPH	0020000		/* Write protect (hardware) */
/*#define	M_U_WRPS	0010000		/* Write protect (software or volume) */
/*#define	M_U_SCHH	0004000		/* Suppress caching (high speed) */
/*#define	M_U_SCHL	0002000		/* Suppress caching (low speed) */
/*#define	M_U_RMVBL	0000200		/* Removable media */
/*#define	M_U_WBKNV	0000100		/* Write back (non-volatile) */
/*#define	M_U_576		0000004		/* 576 byte sectors */
/*#define	M_U_CMPWR	0000002		/* Compare writes */
/*#define	M_U_CMPRD	0000001		/* Compare reads */


/*
 * Status codes
 */
#define	M_S_MASK	037		/* Status code mask */
#define	M_S_SUCC	000		/* Success */
#define	M_S_ICMD	001		/* Invalid command */
#define	M_S_ABRTD	002		/* Command aborted */
#define	M_S_OFFLN	003		/* Unit offline */
#define	M_S_AVLBL	004		/* Unit available */
#define	M_S_MFMTE	005		/* Media format error */
#define	M_S_WRTPR	006		/* Write protected */
#define	M_S_COMP	007		/* Compare error */
#define	M_S_DATA	010		/* Data error */
#define	M_S_HSTBF	011		/* Host buffer access error */
#define	M_S_CNTLR	012		/* Controller error */
#define	M_S_DRIVE	013		/* Drive error */
#define	M_S_DIAG	037		/* Message from an internal diagnostic */


/*
 * The typedef `u_char' should be unsinged, but the V7
 * C compiler will not allow it !
 * Must mask out high order bits due to sign extension.
 */
/*typedef char    u_char;*/
typedef	short	quad[4];		/* a word-aligned quadword */

/*
 * MSCP packet info
 */

struct mscp_header {
	short	uda_msglen;	/* length of MSCP packet */
	char	uda_credits;	/* low 4 bits: credits, high 4 bits: msgtype */
	char	uda_vcid;	/* virtual circuit id */
};

/*
 * An MSCP packet
 */

struct mscp {
	struct	mscp_header m_header;	/* device specific header */
	int	m_cmdref;		/* command reference number */
	int	m_elref;		/* plus error log reference number */
	short	m_unit;			/* unit number */
	short	m_xxx1;			/* unused */
	u_char	m_opcode;		/* opcode */
	u_char	m_flags;		/* end message flags */
	short	m_modifier;		/* modifiers */
	union {
	struct {
		int	Ms_bytecnt;	/* byte count */
		int	Ms_zzz2;	/* 64kb max on V7 */
		int	Ms_buf_l;	/* buffer descriptor low word */
		int	Ms_buf_h;	/* buffer descriptor hi  word */
		long	Ms_xx2[2];	/* unused */
		int	Ms_lbn_l;	/* logical block number low word */
		int	Ms_lbn_h;	/* logical bhock number hi  word */
		long	Ms_xx4;		/* unused */
		long	*Ms_dscptr;	/* pointer to descriptor (software) */
		long	Ms_sftwds[4];	/* software words, padding */
	} m_generic;
	struct {
		short	Ms_version;	/* MSCP version */
		short	Ms_cntflgs;	/* controller flags */
		short	Ms_hsttmo;	/* host timeout */
		short	Ms_usefrac;	/* use fraction */
		long	Ms_time;	/* time and date */
	} m_setcntchar;
	struct {
		short	Ms_multunt;	/* multi-unit code */
		short	Ms_unitflgs;	/* unit flags */
		long	Ms_hostid;	/* host identifier */
		quad	Ms_unitid;	/* unit identifier */
		long	Ms_mediaid;	/* media type identifier */
		short	Ms_shdwunt;	/* shadow unit */
		short	Ms_shdwsts;	/* shadow status */
		short	Ms_track;	/* track size */
		short	Ms_group;	/* group size */
		short	Ms_cylinder;	/* cylinder size */
		short	Ms_xx3;		/* reserved */
		short	Ms_rctsize;	/* RCT table size */
		char	Ms_rbns;	/* RBNs / track */
		char	Ms_rctcpys;	/* RCT copies */
	} m_getunitsts;
	} m_un;
	int	m_msgpad[3];		/* pad msg length to 64 bytes */
					/* required by UQ bus port spec */
};

/*
 * generic packet
 */

#define	m_zzz2		m_un.m_generic.Ms_zzz2
#define	m_bytecnt	m_un.m_generic.Ms_bytecnt
#define	m_buf_l		m_un.m_generic.Ms_buf_l
#define	m_buf_h		m_un.m_generic.Ms_buf_h
#define	m_lbn_l		m_un.m_generic.Ms_lbn_l
#define	m_lbn_h		m_un.m_generic.Ms_lbn_h
#define	m_dscptr	m_un.m_generic.Ms_dscptr
#define	m_sftwds	m_un.m_generic.Ms_sftwds
#define	m_status	m_modifier

/*
 * Abort / Get Command Status packet
 */

#define	m_outref	m_bytecnt

/*
 * Online / Set Unit Characteristics packet
 */

#define m_elgfll	m_lbn_l
#define m_elgflh	m_lbn_h
#define	m_copyspd	m_shdwsts

/*
 * Replace packet
 */

#define	m_rbn	m_bytecnt

/*
 * Set Controller Characteristics packet
 */

#define	m_version	m_un.m_setcntchar.Ms_version
#define	m_cntflgs	m_un.m_setcntchar.Ms_cntflgs
#define	m_hsttmo	m_un.m_setcntchar.Ms_hsttmo
#define	m_usefrac	m_un.m_setcntchar.Ms_usefrac
#define	m_time		m_un.m_setcntchar.Ms_time

/*
 * Get Unit Status end packet
 */

#define	m_multunt	m_un.m_getunitsts.Ms_multunt
#define	m_unitflgs	m_un.m_getunitsts.Ms_unitflgs
#define	m_hostid	m_un.m_getunitsts.Ms_hostid
#define	m_unitid	m_un.m_getunitsts.Ms_unitid
#define	m_mediaid	m_un.m_getunitsts.Ms_mediaid
#define	m_shdwunt	m_un.m_getunitsts.Ms_shdwunt
#define	m_shdwsts	m_un.m_getunitsts.Ms_shdwsts
#define	m_track		m_un.m_getunitsts.Ms_track
#define	m_group		m_un.m_getunitsts.Ms_group
#define	m_cylinder	m_un.m_getunitsts.Ms_cylinder
#define	m_rctsize	m_un.m_getunitsts.Ms_rctsize
#define	m_rbns		m_un.m_getunitsts.Ms_rbns
#define	m_rctcpys	m_un.m_getunitsts.Ms_rctcpys

/*
 * Online / Set Unit Characteristics end packet
 */

#define	m_uslow		m_un.m_getunitsts.Ms_track;
#define	m_ushigh	m_un.m_getunitsts.Ms_group;
#define	m_volser	m_sftwds[0]

/*
 * Set Controller Characteristics end packet
 */

#define	m_cnttmo	m_hsttmo
#define	m_cntcmdl	m_usefrac
#define	m_cntid		m_unitid


/*
 * Error Log message format codes
 */
#define	M_F_CNTERR	0	/* Controller error */
#define	M_F_BUSADDR	1	/* Host memory access error */
#define	M_F_DISKTRN	2	/* Disk transfer error */
#define	M_F_SDI		3	/* SDI error */
#define	M_F_SMLDSK	4	/* Small disk error */

/*
 * Error Log message flags
 */
#define	M_LF_SUCC	0200	/* Operation successful */
#define	M_LF_CONT	0100	/* Operation continuing */
#define	M_LF_SQNRS	0001	/* Sequence number reset */

/*
 * MSCP Error Log packet
 *
 *	NOTE: MSCP packet must be padded to this size.
 */

struct mslg {
	struct	mscp_header me_header;	/* device specific header */
	int	me_cmdref;		/* command reference number */
	int	me_elref;		/* error log reference number */
	short	me_unit;		/* unit number */
	short	me_seqnum;		/* sequence number */
	u_char	me_format;		/* format */
	u_char	me_flags;		/* error log message flags */
	short	me_event;		/* event code */
	quad	me_cntid;		/* controller id */
	u_char	me_cntsvr;		/* controller software version */
	u_char	me_cnthvr;		/* controller hardware version */
	short	me_multunt;		/* multi-unit code */
	quad	me_unitid;		/* unit id */
	u_char	me_unitsvr;		/* unit software version */
	u_char	me_unithvr;		/* unit hardware version */
	short	me_group;		/* group; retry + level */
	int	me_volser[2];		/* volume serial number */
	int	me_hdr[2];		/* header */
	char	me_sdistat[12];		/* SDI status information */
};

#define	me_busaddr	me_unitid
#define	me_sdecyl	me_group

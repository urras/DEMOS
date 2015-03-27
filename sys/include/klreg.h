#ifndef _sys_klreg_h_
#       define  _sys_klreg_h_

/*
 *  KL11/DL11 and СПИ-15 registers and bits
 */
struct	dldevice
{
	short	dlrcsr;
	short	dlrbuf;
	short	dlxcsr;
	short	dlxbuf;
};

/* bits in dlrcsr */
#define	DL_DSC		0100000		/* data status change (read only) */
#define	DL_RNG		0040000		/* ring indicator (read only) */
#define	DL_CTS		0020000		/* clear to send (read only) */
#define	DL_CD		0010000		/* carrier detector (read only) */
#define	DL_RA		0004000		/* receiver active (read only) */
#define	DL_SRD		0002000		/* secondary received data (read only) */
/* bits 9-8 are unused */
#define	DL_RDONE	0000200		/* receiver done (read only) */
#define	DL_RIE		0000100		/* receiver interrupt enable */
#define	DL_DIE		0000040		/* dataset interrupt enable */
/* bit 4 is unused */
#define	DL_STD		0000010		/* secondary transmitted data */
#define	DL_RTS		0000004		/* request to send */
#define	DL_DTR		0000002		/* data terminal ready */
#define	DL_RE		0000001		/* reader enable (write only) */
#define	DL_BITS		\
"\10\20DSC\17RNG\16CTS\15CD\14RA\13SRD\10RDONE\7RIE\6DIE\4STD\3RTS\2DTR\1RE"

/* bits in dlrbuf */
#define	DLRBUF_ERR	0100000		/* error (read only) */
#define	DLRBUF_OVR	0040000		/* overrun (read only) */
#define	DLRBUF_FRE	0020000		/* framing error (read only) */
#define	DLRBUF_RDPE	0010000		/* receive data parity error (read only) */
#define	DLRBUF_BITS	\
"\10\20ERR\17OVR\16FRE\15RDPE"

/* bits in dlxcsr */
/* bits 15-8 are unused */
#define	DLXCSR_TRDY	0000200		/* transmitter ready (read only) */
#define	DLXCSR_TIE	0000100		/* transmitter interrupt enable */
/* bits 5-3 are unused */
#define	DLXCSR_MM	0000004		/* maintenance */
/* bit 1 is unused */
#define	DLXCSR_BRK	0000001		/* break */
#define	DLXCSR_BITS	\
"\10\10TRDY\7TIE\3MM\1BRK"

/* СПИ-15 bits */

/* bits in dlrcsr */
#define DLKR_RDPE       0100000
/* bits 14-9 are unused */
#define DLKR_RESET      0000400         /* сброс интерфейсов */
#define DLKR_RDONE      0000200         /* receiver done (read only) */
#define DLKR_RIE        0000100         /* receiver interrupt enable */
/* bits 5-0 are unused */
#define DLKR_BITS       \
"\10\20RDPE\11RESET\10RDONE\7RIE"

/* bits in dlxcsr */
#define DLKX_OFFL       0100000         /* абонент не готов (read only) */
/* bits 14-9 are unused */
#define DLKX_RESTART    0000400         /* send restart signal to LSI-11 */
#define DLKX_TRDY       0000200         /* transmitter ready (read only) */
#define DLKX_TIE        0000100         /* transmitter interrupt enable */
/* bits 5-0 are unused */
#define DLKX_BITS       \
"\10\20OFFL\11REST\10TRDY\7TIE"

#define	DLDELAY		0000004		/* Extra delay for DLs */

#define klminor(a)      ((short)(a)&0177)
#define MODEM(dev)      ((short)(dev)&0200)

#endif  _sys_klreg_h_

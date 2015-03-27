#ifndef _sys_seg_h_
#       define  _sys_seg_h_

/*
 * Addresses and bits for DEC's KT-11
 * and macros for remapping kernel data space.
 */

#define UISD    ((u_short *) 0177600)   /* first user I-space descriptor */
#       define  UISA    ((u_short *) 0177640)   /* first user I-space address */
#       define  UDSA    ((u_short *) 0177660)   /* first user D-space address */

/*
 * Access abilities
 */
#define RO      02              /* Read only */
#define RW      06              /* Read and write */
#define NOACC   0               /* Abort all accesses */
#define ACCESS  07              /* Mask for access field */
#define ED      010             /* Extension direction */
#define TX      020             /* Software: text segment */
#define ABS     040             /* Software: absolute address */

u_short *ka6;                   /* nonseparate:  KISA6; separate:  KDSA6 */

#       define  KISA0   ((u_short *) 0172340)
#       define  KISA1   ((u_short *) 0172342)
#       define  KISA2   ((u_short *) 0172344)
#       define  KISA3   ((u_short *) 0172346)
#       define  KISA4   ((u_short *) 0172350)
#       define  KISA5   ((u_short *) 0172352)
#       define  KISA6   ((u_short *) 0172354)
#       define  KISA7   ((u_short *) 0172356)
#       define  SSR3    ((u_short *) 0172516)

#define KISD0   ((u_short *) 0172300)
#define KISD1   ((u_short *) 0172302)
#define KISD5   ((u_short *) 0172312)
#define KISD6   ((u_short *) 0172314)
#ifdef  KERN_NONSEP
#               define  KDSA5   KISA5
#               define  KDSA6   KISA6
#       define  KDSD5   KISD5
#       define  KDSD6   KISD6
#       define  KDSD0   KISD0
#else
#               define  KDSA5   ((u_short *) 0172372)
#               define  KDSA6   ((u_short *) 0172374)
#       define  KDSD0   ((u_short *) 0172320)
#       define  KDSD5   ((u_short *) 0172332)
#       define  KDSD6   ((u_short *) 0172334)
#endif
#define SEG5    ((caddr_t) 0120000)


/*
 * Macros for resetting the kernel segmentation registers to map in
 * out-of-address space data.
 * If KDSA5 is used for kernel data space (NOKA5 not defined),
 * only proc, file and text tables are allowed in that range.
 * Routines can repoint KDSA5 to map in data such as buffers or clists
 * without raising processor priority by calling these macros.
 * Copy (in mch.s) uses two registers, KDSA5 and 6. If KDSA6 is in use,
 * the prototype register kdsa6 will be non-zero, and the kernel will
 * be running on a temporary stack in bss.
 * Interrupt routines that access any of the structures in this range
 * or the u. must call savemap (in machdep.c) to save the current mapping
 * information in a local structure and restore it before returning.
 * If KDSA5 is not used for static kernel data, only KDSA6 need be
 * saved and restored to access all static data at interrupt time.
 *
 * USAGE:
 *	To repoint KDSA5 from the top level,
 *		mapseg5(addr, desc);	* KDSA5 now points at addr *
 *		...
 *		normalseg5();		* normal mapping *
 *	To repoint KDSA5 from interrupt or top level,
 *		segm saveregs;
 *		saveseg5(saveregs);	* save previous mapping of segment 5 *
 *		mapseg5(addr, desc);	* KDSA5 now points at addr *
 *		...
 *		restorseg5(saveregs);	* restore previous mapping *
 *	To access proc, text, file or user structures from interrupt level,
 *		mapinfo map;
 *		savemap(map);		* save ALL mapping information, *
 *					* restore normal mapping of KA5 and 6 *
 *		...
 *		restormap(map);		* restore previous mapping *
 */

/*
 *  Structure to hold a saved PAR/PDR pair.
 */
struct segm_reg {
	unsigned	se_desc;
	unsigned	se_addr;
};
typedef	struct segm_reg	segm;

unsigned	kdsa6;				/* saved KDSA6, if any */
#if PDP11 == 1630 || PDP11 == GENERIC
#define KD6 (((128-1)<<8) | RW)       /* prototype descriptor for u. */
#else   /* 1630 || GENERIC */
#define	KD6 (((USIZE-1)<<8) | RW)	/* prototype descriptor for u. */
#endif /* 1630 || GENERIC */

#define	NOMAP	65535			/* invalid PDR value */

/*
 *	remap_area is a dummy var at start of possibly-mapped area,
 *	for consistency check.  Only proc, text and file tables are after it,
 *	and it must lie at <= 0120000, or other kernel data will be mapped out.
 */
extern	int remap_area;

#ifndef	NOKA5

segm seg5;				/* prototype KDSA5, KDSD5 */
typedef	struct segm_reg mapinfo[2];	/* KA5, KA6 */

/* 	Restore normal kernel map for seg5.  */
#define	normalseg5()	restorseg5(seg5)

/*	save previous mapping information and restore normal mapping.  */
#define	savemap(map)	{if (*KDSA5 != seg5.se_addr) Savemap(map); \
			 else map[0].se_desc = NOMAP; }

/*	Restore previous mapping on return from interrupt.  */
#define	restormap(map)	{if (map[0].se_desc != NOMAP) Restormap(map); }

#else	NOKA5

typedef struct segm_reg mapinfo[1];	/* KDSA6 only */

/*
 *	Restore normal kernel map for seg5: unneeded,
 *	since no kernel data are in this range.
 */
#define	normalseg5()	/* not needed */

/*	save previous mapping information and restore normal mapping.  */
#define	savemap(map)	if (kdsa6) { \
		map[0].se_addr = *KDSA6; \
		map[0].se_desc = *KDSD6; \
		*KDSD6 = KD6; \
		*KDSA6 = kdsa6; \
	} else map[0].se_desc = NOMAP;

/*	Restore previous mapping on return from interrupt.  */
#define	restormap(map)	if (map[0].se_desc != NOMAP) { \
		*KDSD6 = map[0].se_desc; \
		*KDSA6 = map[0].se_addr; \
	}

#endif	NOKA5

/*	Use segment 5 to access the given address.  */
#define	mapseg5(addr,desc)	{*KDSA5 = addr; *KDSD5 = desc;}

/*	Save the previous contents of KDSA5/KDSD5.  */
#define	saveseg5(save)		{save.se_addr = *KDSA5; save.se_desc = *KDSD5;}

/*	Restore the previous contents of KDSA5/KDSD5.  */
#define	restorseg5(save)	{*KDSD5 = save.se_desc; *KDSA5 = save.se_addr;}

#endif  _sys_seg_h_

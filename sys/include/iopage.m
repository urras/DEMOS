#ifndef _sys_iopage_h_
#       define _sys_iopage_h_

/*
 *	Access abilities (from seg.h)
 */
#define	RO	2
#define	RW	6

/*
 *	ENABLE/34 registers
 *
 *	All information relevant to the ENABLE/34 is supplied with
 *	the permission of ABLE Computer and may not be disclosed in
 *	any manner to sites not licensed by the University of California
 *	for the Second Berkeley Software Distribution.
 *
 */
#	define	ENABLE_UDSA	0163740
#	define	DEC_UDSA	0177660
#	define	ENABLE_KDSA1	0163762
#	define	ENABLE_KDSA2	0163764
#	define	ENABLE_KDSA5	0163772
#	define	ENABLE_KDSA6	0163774
#	define	DEC_KDSA1	0172362
#	define	DEC_KDSA2	0172364
#	define	DEC_KDSA5	0172372
#	define	DEC_KDSA6	0172374

/*
 *	DEC registers
 */
#define	SISD0	0172200
#define	SISD1	0172202
#define	SISD2	0172204
#define	SISD3	0172206
#define	SDSD0	0172220
#define	SISA0	0172240
#define	SISA1	0172242
#define	SISA2	0172244
#define	SISA7	0172256
#define	SDSA0	0172260

#define	KISD0	0172300
#define	KISD1	0172302
#define	KISD2	0172304
#define	KISD4	0172310
#define	KISD5	0172312
#define	KISD6	0172314
#define	KISD7	0172316
#ifdef	KERN_NONSEP
#   define	KDSD0	KISD0
#   define	KDSD5	KISD5
#   define	KDSD6	KISD6
#else
#   define	KDSD0	0172320
#   define	KDSD5	0172332
#   define	KDSD6	0172334
#endif

#   define	KISA0	0172340
#define	KISA1	0172342
#define	KISA2	0172344
#define	KISA4	0172350
#define	KISA5	0172352
#   define	KISA6	0172354
#define	KISA7	0172356
#ifdef	KERN_NONSEP
#   define	KDSA0	KISA0
#	define	KDSA1	KISA1
#	define	KDSA2	KISA2
#	define	KDSA5	KISA5
#	define	KDSA6	KISA6
#define	KDSA7	KISA7
#else	KERN_NONSEP
#   define	KDSA0	0172360
#	define	KDSA1	0172362
#	define	KDSA2	0172364
#	define	KDSA5	0172372
#	define	KDSA6	0172374
#define	KDSA7	0172376
#endif	KERN_NONSEP

#define	SSR3	0172516
#define	CCSR	0172540
#define	CCSB	0172542
#define	CSW	0177570
#define	SSR0	0177572
#define	SSR1	0177574
#define	SSR2	0177576
#   define	UISA	0177640
#   define	UDSA	0177660

#define	STACKLIM 0177774
#define	PS	0177776
#define TBIT	020		/* trace bit in PS */

#endif _sys_iopage_h_

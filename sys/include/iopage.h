#ifndef _sys_iopage_h_
#       define  _sys_iopage_h_

/*
 * I/O page addresses
 */

#define	PS		((u_short *) 0177776)
#define	SL		((u_short *) 0177774)
#define	PIR		((u_short *) 0177772)

#if     PDP11 == 44 || PDP11 == 70 || PDP11 == 73 || PDP11 == GENERIC
#define	CPUERR		((u_short *) 0177766)
#endif

#if	PDP11 == 70 || PDP11 == GENERIC
#define	SYSSIZEHI	((u_short *) 0177762)
#define	SYSSIZELO	((u_short *) 0177760)
#endif

#if     PDP11 == 44 || PDP11 == 70 || PDP11 == 73 || PDP11 == GENERIC
#define	MEMSYSCTRL	((u_short *) 0177746)
#define	MEMSYSERR	((u_short *) 0177744)
#define	MEMERRHI	((u_short *) 0177742)
#define	MEMERRLO	((u_short *) 0177740)
#endif

#if     PDP11 == 1630 || PDP11 == GENERIC
#define U834_AR0        ((u_short *) 0177760)
#define U834_AR1        ((u_short *) 0177762)
#define U834_AR2        ((u_short *) 0177764)
#endif

#define	CSW		((u_short *) 0177570)
#define	LKS		((u_short *) 0177546)
#define	KW11P_CSR	((u_short *) 0172540)

#if     PDP11 == 23
/* Для Электроники - 85 */
#define TIM85_A         ((u_short *) 0173024)
#define TIM85_B         ((u_short *) 0173026)
#define TIM85_C         ((u_short *) 0173030)
#define I85_RR1         ((u_short *) 0173200)
#define I85_CR1         ((u_short *) 0173202)
#define I85_RR2         ((u_short *) 0173204)
#define I85_CR2         ((u_short *) 0173206)
#define I85_RR3         ((u_short *) 0173210)
#define I85_CR3         ((u_short *) 0173212)
#endif

#endif  _sys_iopage_h_

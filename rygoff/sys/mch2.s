
setreg:
	mov	r0,-(sp)
	bic	$!7,r0
	bis	r0,r2
	mov	(sp)+,r0
	ash	$-3,r0
	bic	$!7,r0
	movb	0f(r0),r0
	tstb	bflg
	beq	1f
	bit	$2,r2
	beq	2f
	bit	$4,r2
	beq	2f
1:
	cmp	r0,$20
	beq	2f
	cmp	r0,$-20
	beq	2f
	asl	r0
2:
#ifndef	NONFP
	tstb	fflg
	beq	3f
	asl	r0
	stfps	r1
	bit	$200,r1
	beq	3f
	asl	r0
3:
#endif
	bisb	r0,r2
	rts	pc

0:	.byte	0,0,10,20,-10,-20,0,0

fetch:
	bic	$1,r0
	mov	nofault,-(sp)
	mov	$1f,nofault
	mfpi	(r0)
	mov	(sp)+,r0
	mov	(sp)+,nofault
	rts	pc

1:
 	mov	(sp)+,nofault
	clrb	r2			/ clear out dest on fault
	mov	$-1,r0
	rts	pc

	.bss
bflg:	.=.+1
jflg:	.=.+1
fflg:	.=.+1
	.text
#endif	KERN_NONSEP

	.globl	_fuibyte, _fubyte, _suibyte, _subyte
_fuibyte:
#ifndef	NONSEPARATE
	mov	2(sp),r1
	bic	$1,r1
	jsr	pc,giword
	br	2f
#endif

_fubyte:
	mov	2(sp),r1
	bic	$1,r1
	jsr	pc,gword

2:
	cmp	r1,2(sp)
	beq	1f
	swab	r0
1:
	bic	$!377,r0
	rts	pc

_suibyte:
#ifndef	NONSEPARATE
	mov	2(sp),r1
	bic	$1,r1
	jsr	pc,giword
	mov	r0,-(sp)
	cmp	r1,4(sp)
	beq	1f
	movb	6(sp),1(sp)
	br	2f
1:
	movb	6(sp),(sp)
2:
	mov	(sp)+,r0
	jsr	pc,piword
	clr	r0
	rts	pc
#endif

_subyte:
	mov	2(sp),r1
	bic	$1,r1
	jsr	pc,gword
	mov	r0,-(sp)
	cmp	r1,4(sp)
	beq	1f
	movb	6(sp),1(sp)
	br	2f
1:
	movb	6(sp),(sp)
2:
	mov	(sp)+,r0
	jsr	pc,pword
	clr	r0
	rts	pc

	.globl	_fuiword, _fuword, _suiword, _suword
_fuiword:
#ifndef	NONSEPARATE
	mov	2(sp),r1
fuiword:
	jsr	pc,giword
	rts	pc
#endif

_fuword:
	mov	2(sp),r1
fuword:
	jsr	pc,gword
	rts	pc

gword:
#ifndef	NONSEPARATE
	mov	PS,-(sp)
#ifdef	UCB_NET
	bis	$30000,PS		/ just in case; dgc; %%%%
#endif
	SPLHIGH
	mov	nofault,-(sp)
	mov	$err,nofault
	mfpd	(r1)
	mov	(sp)+,r0
	br	1f

giword:
#endif
	mov	PS,-(sp)
#ifdef	UCB_NET
	bis	$30000,PS		/ just in case; dgc; %%%%
#endif
	SPLHIGH
	mov	nofault,-(sp)
	mov	$err,nofault
	mfpi	(r1)
	mov	(sp)+,r0
	br	1f

_suiword:
#ifndef	NONSEPARATE
	mov	2(sp),r1
	mov	4(sp),r0
suiword:
	jsr	pc,piword
	rts	pc
#endif

_suword:
	mov	2(sp),r1
	mov	4(sp),r0
suword:
	jsr	pc,pword
	rts	pc

pword:
#ifndef	NONSEPARATE
	mov	PS,-(sp)
#ifdef	UCB_NET
	bis	$30000,PS		/ just in case; dgc; %%%%
#endif
	SPLHIGH
	mov	nofault,-(sp)
	mov	$err,nofault
	mov	r0,-(sp)
	mtpd	(r1)
	br	1f

piword:
#endif
	mov	PS,-(sp)
#ifdef	UCB_NET
	bis	$30000,PS		/ just in case; dgc; %%%%
#endif
	SPLHIGH
	mov	nofault,-(sp)
	mov	$err,nofault
	mov	r0,-(sp)
	mtpi	(r1)
1:
	mov	(sp)+,nofault
	mov	(sp)+,PS
	rts	pc

err:
	mov	(sp)+,nofault
	mov	(sp)+,PS
	tst	(sp)+
	mov	$-1,r0
	rts	pc

	.globl	_copyin, _copyiin, _copyout, _copyiout
_copyin:
#ifndef	NONSEPARATE
	jsr	pc,copsu
1:
	mfpd	(r0)+
	mov	(sp)+,(r1)+
	sob	r2,1b
	br	2f
#endif

_copyiin:
	jsr	pc,copsu
1:
	mfpi	(r0)+
	mov	(sp)+,(r1)+
	sob	r2,1b
	br	2f

_copyout:
#ifndef	NONSEPARATE
	jsr	pc,copsu
1:
	mov	(r0)+,-(sp)
	mtpd	(r1)+
	sob	r2,1b
	br	2f
#endif

_copyiout:
	jsr	pc,copsu
1:
	mov	(r0)+,-(sp)
	mtpi	(r1)+
	sob	r2,1b
2:
	mov	(sp)+,nofault
	mov	(sp)+,r2
	clr	r0
	rts	pc

copsu:
#ifdef	UCB_NET
	bis	$30000,PS		/ make sure that we copy to/from user space
					/ this is a test - dgc - %%%%
#endif
	mov	(sp)+,r0
	mov	r2,-(sp)
	mov	nofault,-(sp)
	mov	r0,-(sp)
	mov	10(sp),r0
	mov	12(sp),r1
	mov	14(sp),r2
	asr	r2
	mov	$1f,nofault
	rts	pc

1:
	mov	(sp)+,nofault
	mov	(sp)+,r2
	mov	$-1,r0
	rts	pc

	.globl	_idle, _waitloc
_idle:
	mov	PS,-(sp)
	SPLLOW
	wait
waitloc:
	mov	(sp)+,PS
	rts	pc

	.data
_waitloc:
	waitloc
	.text
#if	defined(PROFILE) && !defined(ENABLE34)
/ These words are to insure that times reported for _save
/ do not include those spent while in idle mode, when
/ statistics are gathered for system profiling.
/
	rts	pc
	rts	pc
	rts	pc
#endif	defined(PROFILE) && !defined(ENABLE34)

	.globl	_save, _resume
_save:
	mov	(sp)+,r1
	mov	(sp),r0
	mov	r2,(r0)+
	mov	r3,(r0)+
	mov	r4,(r0)+
	mov	r5,(r0)+
	mov	sp,(r0)+
#ifdef	MENLO_KOV
	mov	__ovno,(r0)+
#endif
	mov	r1,(r0)+
	clr	r0
	jmp	(r1)

_resume:
	mov	2(sp),r0		/ new process
	mov	4(sp),r1		/ new stack
	SPL7
	mov	r0,KDSA6		/ In new process
	mov	(r1)+,r2
	mov	(r1)+,r3
	mov	(r1)+,r4
	mov	(r1)+,r5
	mov	(r1)+,sp
#ifdef	MENLO_KOV
	mov	(r1)+,r0
	cmp	r0,__ovno
	beq	1f
	mov	r0,__ovno
	asl	r0
	mov	ova(r0), OVLY_PAR
	mov	ovd(r0), OVLY_PDR
1:
#endif	MENLO_KOV
	mov	$1,r0
	SPLLOW
	jmp	*(r1)+

/*
 *	Note that in the Berkeley system, calls to spl's except splx
 *	are substituted in line in the assembly code on machines
 *	with the spl instruction or mtps/mfps.  Splx is done by macros
 *	in param.h. See the makefile, :splfix.spl, :splfix.mtps,
 *	:splfix.movb and param.h.  Calls to __spl# (_spl# in C)
 *	are always expanded in-line and do not return the previous priority.
 */

#if	defined(KERN_NONSEP) && PDP11 != 34 && PDP11 != 23 && PDP11 != 24
	/  Spl's for machines (like 11/40) without spl or m[tf]ps instructions.
	.globl	_spl0, _spl1, _spl4, _spl5, _spl6, _spl7
_spl0:
	movb	PS,r0
	clrb	PS
	rts	pc
_spl1:
	movb	PS,r0
	movb	$40, PS
	rts	pc
_spl4:
	movb	PS,r0
	movb	$200, PS
	rts	pc
_spl5:
	movb	PS,r0
	movb	$240, PS
	rts	pc
_spl6:
	movb	PS,r0
	movb	$300, PS
	rts	pc
_spl7:
	movb	PS,r0
	movb	$HIPRI, PS
	rts	pc
#endif

	.globl	_copy, _clear, _kdsa6
#ifdef	CGL_RTP
	.globl	_copyu, _wantrtp, _runrtp
#endif

/*
 * Copy count clicks from src to dst.
 * Uses KDSA5 and 6 to copy with mov instructions.
 * Interrupt routines must restore segmentation registers if needed;
 * see seg.h.
 * Note that if CGL_RTP is defined, it checks whether
 * the real-time process is runnable once each loop,
 * and preempts the current process if necessary
 * (which must not swap before this finishes!).
 *
 * copy(src, dst, count)
 * memaddr src, dst;
 * int count;
 */
_copy:
	jsr	r5, csv
#ifdef	UCB_NET
	mov	PS,-(sp)		/ have to lock out interrupts...
	SPL7
	mov	KDSA5,-(sp)		/ save seg5
	mov	KDSD5,-(sp)		/ save seg5
#endif
	mov	10(r5),r3		/ count
	beq	3f
	mov	4(r5),KDSA5		/ point KDSA5 at source
	mov	$RO,KDSD5		/ 64 bytes, read-only
	mov	sp,r4
	mov	$eintstk,sp		/ switch to intstk
	mov	KDSA6,_kdsa6
	mov	6(r5),KDSA6		/ point KDSA6 at destination
	mov	$RW,KDSD6		/ 64 bytes, read-write
1:
#ifdef	CGL_RTP
	tst	_wantrtp
	bne	preempt
9:
#endif
	mov	$5*8192.,r0
	mov	$6*8192.,r1
#if	PDP11==70
	mov	$4.,r2			/ copy one click (4*16)
#else
	mov	$8.,r2			/ copy one click (8*8)
#endif
2:
	mov	(r0)+,(r1)+
	mov	(r0)+,(r1)+
	mov	(r0)+,(r1)+
	mov	(r0)+,(r1)+
#if	PDP11==70
	mov	(r0)+,(r1)+
	mov	(r0)+,(r1)+
	mov	(r0)+,(r1)+
	mov	(r0)+,(r1)+
#endif
	sob	r2,2b

	inc	KDSA5			/ next click
	inc	KDSA6
	dec	r3
	bne	1b
	mov	_kdsa6,KDSA6
	mov	$usize-1\<8|RW, KDSD6
	clr	_kdsa6
#ifndef	NOKA5
	.globl	_seg5
	mov	_seg5+SE_DESC, KDSD5	/ normalseg5();
	mov	_seg5+SE_ADDR, KDSA5	/ (restore all mapping)
#endif	NOKA5
	mov	r4,sp			/ back to normal stack
3:
#ifdef	UCB_NET
	mov	(sp)+,KDSD5		/ restore seg5
	mov	(sp)+,KDSA5		/ restore seg5
	mov	(sp)+,PS		/ back to normal priority
#endif
	jmp	cret

#ifdef	CGL_RTP
/*
 * Save our state and restore enough context for a process switch.
 */
preempt:
	mov	KDSA6, r0
	mov	_kdsa6,KDSA6		/ back to our u.
	mov	$usize-1\<8|RW, KDSD6
	clr	_kdsa6
	mov	r4, sp			/ back to normal stack
	mov	KDSA5, -(sp)
	mov	r0, -(sp)		/ KDSA6
#ifndef	NOKA5
	mov	_seg5+SE_DESC, KDSD5	/ normalseg5();
	mov	_seg5+SE_ADDR, KDSA5	/ (restore all mapping)
#endif	NOKA5
	jsr	pc, _runrtp		/ switch context and run rtpp

	/ Now continue where we left off.
	mov	(sp)+, r0		/ KDSA6
	mov	(sp)+, KDSA5
	mov	$RO,KDSD5		/ 64 bytes, read-only
	mov	KDSA6,_kdsa6
	mov	$eintstk, sp
	mov	r0, KDSA6
	mov	$RW,KDSD6		/ 64 bytes, read-write
	br	9b

/*
 * Copy the u. to dst; not preemptable.
 * Uses KDSA5 to copy with mov instructions.
 * Interrupt routines must restore segmentation registers if needed;
 * see seg.h.
 *
 * copyu(dst)
 * memaddr dst;
 */
_copyu:
	jsr	r5, csv
	mov	4(r5),KDSA5		/ point KDSA5 at dst.
	mov	$usize-1\<8.|RW,KDSD5
	mov	$6*8192.,r0
	mov	$5*8192.,r1
#if	PDP11==70
	mov	$4.*usize,r2		/ copy 4*16 bytes per click
#else
	mov	$8.*usize,r2		/ copy 8*8 bytes per click
#endif
2:
	mov	(r0)+,(r1)+
	mov	(r0)+,(r1)+
	mov	(r0)+,(r1)+
	mov	(r0)+,(r1)+
#if	PDP11==70
	mov	(r0)+,(r1)+
	mov	(r0)+,(r1)+
	mov	(r0)+,(r1)+
	mov	(r0)+,(r1)+
#endif
	sob	r2,2b

#ifndef	NOKA5
	mov	_seg5+SE_DESC, KDSD5	/ normalseg5();
	mov	_seg5+SE_ADDR, KDSA5	/ (restore all mapping)
#endif	NOKA5
	jmp	cret
#endif	CGL_RTP

/*
 * Clear count clicks at dst.
 * Uses KDSA5.
 * Interrupt routines must restore segmentation registers if needed;
 * see seg.h.
 *
 * clear(dst, count)
 * memaddr dst;
 * int count;
 */
_clear:
	jsr	r5, csv
#ifdef	UCB_NET
	mov	KDSA5,-(sp)		/ save seg5
	mov	KDSD5,-(sp)		/ save seg5
#endif
	mov	4(r5),KDSA5		/ point KDSA5 at source
	mov	$RW,KDSD5		/ 64 bytes, read-write
	mov	6(r5),r3		/ count
	beq	3f
1:
#ifdef	CGL_RTP
	tst	_wantrtp
	bne	clrpreempt
9:
#endif
	mov	$5*8192.,r0
	mov	$8.,r2			/ clear one click (8*8)
2:
	clr	(r0)+
	clr	(r0)+
	clr	(r0)+
	clr	(r0)+
	sob	r2,2b

	inc	KDSA5			/ next click
	dec	r3
	bne	1b
3:
#ifndef	NOKA5
	mov	_seg5+SE_DESC, KDSD5	/ normalseg5();
	mov	_seg5+SE_ADDR, KDSA5	/ (restore all mapping)
#endif	NOKA5
#ifdef	UCB_NET
	mov	(sp)+,KDSD5		/ restore seg5
	mov	(sp)+,KDSA5		/ restore seg5
#endif
	jmp	cret

#ifdef	CGL_RTP
clrpreempt:
	mov	KDSA5, -(sp)
#ifndef	NOKA5
	mov	_seg5+SE_DESC, KDSD5	/ normalseg5();
	mov	_seg5+SE_ADDR, KDSA5	/ (restore all mapping)
#endif	NOKA5
	jsr	pc, _runrtp		/ switch context and run rtpp
	/*
	 * Now continue where we left off.
	 */
	mov	(sp)+, KDSA5
	mov	$RW,KDSD5		/ 64 bytes, read-write
	br	9b
#endif	CGL_RTP

#ifdef	UCB_NET
/*
 *	copyv(fromaddr,toaddr,count)
 *	virtual_addr fromaddr,toaddr;
 *	unsigned count;
 *
 *	Copy two arbitrary pieces of PDP11 virtual memory from one location
 *	to another.  Up to 8K bytes can be copied at one time.
 *
 *	A PDP11 virtual address is a two word value; a 16 bit "click" that
 *	defines the start in physical memory of an 8KB segment and an offset.
 */

	.globl	_copyv

	.data
copyvsave: 0			/ saved copy of KDSA6
	.text

_copyv:	jsr	r5,csv

	tst	14(r5)		/* if (count == 0)		*/
	jeq	copyvexit	/* 	return;			*/
	cmp	$20000,14(r5)	/* if (count >= 8192)		*/
	jlos	copyvexit	/*	return;			*/

	mov	PS,-(sp)	/* Lock out interrupts. sigh... */
	SPL7

	mov	KDSA5,-(sp)	/* save seg5			*/
	mov	KDSD5,-(sp)

	mov	4(r5),KDSA5	/* seg5 = fromclick		*/
	mov	$128.-1\<8.|RO,KDSD5
	mov	10(r5),r1	/* click = toclick		*/
	mov	6(r5),r2	/* foff = fromoffset		*/
	add	$5*8192.,r2	/* foff = virtual addr (page 5)	*/
	mov	12(r5),r3	/* toff = tooffset		*/
	add	$6*8192.,r3	/* toff = virtual addr (page 6)	*/
	mov	14(r5),r0	/* count = count		*/

	/* Note: the switched stack is only for use of a fatal	*/
	/* kernel trap occurring during the copy; otherwise we	*/
	/* might conflict with the other copy routine		*/
	mov	sp,r4		/* switch stacks		*/
	mov	$eintstk,sp
	mov	KDSA6,copyvsave

	mov	r1,KDSA6	/* seg6 = click			*/
	mov	$128.-1\<8.|RW,KDSD6

	/****** Finally do the copy 			   ******/
	mov	r3,r1		/* Odd addresses or count?	*/
	bis	r2,r1
	bis	r0,r1
	bit	$1,r1
	bne	copyvodd	/* Branch if odd		*/

	asr	r0		/* Copy a word at a time	*/
1:	mov	(r2)+,(r3)+
	sob	r0,1b
	br	copyvdone

copyvodd: movb	(r2)+,(r3)+	/* Copy a byte at a time	*/
	sob	r0,copyvodd
/	br	copyvdone

copyvdone:
	mov	copyvsave,KDSA6	/* remap in the stack		*/
	mov	$usize-1\<8.|RW,KDSD6
	mov	r4,sp
	mov	(sp)+,KDSD5	/* restore seg5			*/
	mov	(sp)+,KDSA5
	mov	(sp)+,PS	/* unlock interrupts		*/

copyvexit:
	clr	r0
	clr	r1
	jmp	cret
#endif	UCB_NET

hardprobe:
#ifndef	NONFP
	/ Test for floating point capability.
fptest:
	mov	$fpdone, nofault
	setd
	inc	fpp
fpdone:
#endif	NONFP

	/ Test for SSR3 and UNIBUS map capability.  If there is no SSR3,
	/ the first test of SSR3 will trap and we skip past septest.
ubmaptest:
	mov	$cputest, nofault
#ifdef	UNIBUS_MAP
	bit	$20, SSR3
	beq	septest
	incb	_ubmap
#endif

	/ Test for separate I/D capability.
septest:
#ifdef	NONSEPARATE
	/ Don't attempt to determine whether we've got separate I/D
	/ (but just in case we do, we must force user unseparated
	/ because boot will have turned on separation if possible).
	bic	$1, SSR3
#else
	/ Test for user I/D separation (not kernel).
	bit	$1, SSR3
	beq	cputest
	incb	_sep_id
#endif	NONSEPARATE

	/ Try to find out what kind of cpu this is.
	/ Defaults are 40 for nonseparate and 45 for separate.
	/ Cputype will be one of: 24, 40, 60, 45, 44, 70, 73.
cputest:
#ifndef	NONSEPARATE
	tstb	_sep_id
	beq	nonsepcpu

	mov	$1f, nofault
	mfpt
	cmpb    $5,r0
	bne     2f
	mov     $73., _cputype
	clrb    _ubmap
	incb    _q22bus
	br      3f
2:
	mov	$44., _cputype
	/ Disable cache parity interrupts.
3:
	bis	$CCR_DCPI, *$PDP1144_CCR
	br	cpudone
1:
	tstb    _ubmap
	beq     cpudone

	mov	$70., _cputype
	/ Disable UNIBUS and nonfatal traps.
	bis	$CCR_DUT|CCR_DT, *$PDP1170_CCR
	br	cpudone

nonsepcpu:
#endif	NONSEPARATE
	tstb	_ubmap
	beq	1f
	mov	$24., _cputype
	br	cpudone
1:
	mov     $1f, nofault
	mfpt
	mov     $23., _cputype
	clrb    _ubmap
	incb    _q22bus
	br      cpudone
1:
	mov	$cpudone, nofault
	tst	PDP1160_MSR
	mov	$60., _cputype
	/ Disable cache parity error traps.
	bis	$CCR_DT, *$PDP1160_CCR

cpudone:
	/ Test for stack limit register; set it if present.
	mov	$1f, nofault
	mov	$intstk-256., STACKLIM
1:

#ifdef	ENABLE34
	/ Test for an ENABLE/34.  We are very cautious since
	/ the ENABLE's PARs  are in the range of the floating
	/ addresses.
	tstb	_ubmap
	bne	2f
	mov	$2f, nofault
	mov	32., r0
	mov	$ENABLE_KISA0, r1
1:
	tst	(r1)+
	sob	r0, 1b

	tst	*$PDP1170_LEAR
	tst	*$ENABLE_SSR3
	tst	*$ENABLE_SSR4
	incb	_enable34
	incb	_ubmap

	/ Turn on an ENABLE/34.  Enableon() is a C routine
	/ which does a PAR shuffle and turns mapping on.
	.globl	_enableon
	.globl	_UISA, _UDSA, _KISA0, _KISA6, _KDSA1, _KDSA2, _KDSA5, _KDSA6

	.data
_UISA:	DEC_UISA
_UDSA:	DEC_UDSA
_KISA0:	DEC_KISA0
_KISA6:	DEC_KISA6
_KDSA1:	DEC_KDSA1
_KDSA2:	DEC_KDSA2
_KDSA5:	DEC_KDSA5
_KDSA6:	DEC_KDSA6
	.text

	mov	$ENABLE_UISA, _UISA
	mov	$ENABLE_UDSA, _UDSA
	mov	$ENABLE_KISA0, _KISA0
	mov	$ENABLE_KISA6, _KISA6
	mov	$ENABLE_KDSA1, _KDSA1
	mov	$ENABLE_KDSA2, _KDSA2
	mov	$ENABLE_KDSA5, _KDSA5
	mov	$ENABLE_KDSA6, _KDSA6
	mov	$ENABLE_KDSA6, _ka6
	jsr	pc, _enableon

2:
#endif	ENABLE34

	clr	nofault
	rts	pc

/*
 * Long quotient
 */
	.globl	ldiv, lrem
ldiv:
	jsr	r5,csv
	mov	10.(r5),r3
	sxt	r4
	bpl	1f
	neg	r3
1:
	cmp	r4,8.(r5)
	bne	hardldiv
	mov	6.(r5),r2
	mov	4.(r5),r1
	bge	1f
	neg	r1
	neg	r2
	sbc	r1
	com	r4
1:
	mov	r4,-(sp)
	clr	r0
	div	r3,r0
	mov	r0,r4			/high quotient
	mov	r1,-(sp)		/ Stash interim result
	mov	r1,r0
	mov	r2,r1
	div	r3,r0
	bvc	1f
	mov	(sp),r0			/ Recover interim result.
	mov	r2,r1			/ (Regs may be clobbered by failed div.)
	sub	r3,r0			/ this is the clever part
	div	r3,r0
	tst	r1
	sxt	r1
	add	r1,r0			/ cannot overflow!
1:
	tst	(sp)+			/ Pop temp off stack.
	mov	r0,r1
	mov	r4,r0
	tst	(sp)+
	bpl	9f
	neg	r0
	neg	r1
	sbc	r0
9:
	jmp	cret

hardldiv:
	iot				/ ``Cannot happen''

/*
 * Long remainder
 */
lrem:
	jsr	r5,csv
	mov	10.(r5),r3
	sxt	r4
	bpl	1f
	neg	r3
1:
	cmp	r4,8.(r5)
	bne	hardlrem
	mov	6.(r5),r2
	mov	4.(r5),r1
	mov	r1,r4
	bge	1f
	neg	r1
	neg	r2
	sbc	r1
1:
	clr	r0
	div	r3,r0
	mov	r1,-(sp)		/ Stash interim result.
	mov	r1,r0
	mov	r2,r1
	div	r3,r0
	bvc	1f
	mov	(sp),r0			/ Recover interim result.
	mov	r2,r1			/ (Regs may be clobbered by failed div.)
	sub	r3,r0
	div	r3,r0
	tst	r1
	beq	9f
	add	r3,r1
1:
	tst	(sp)+			/ Pop temp off stack.
	tst	r4
	bpl	9f
	neg	r1
9:
	sxt	r0
	jmp	cret

hardlrem:
	iot				/ ``Cannot happen''

	.globl	lmul
lmul:
	mov	r2,-(sp)
	mov	r3,-(sp)
	mov	8(sp),r2
	sxt	r1
	sub	6(sp),r1
	mov	12.(sp),r0
	sxt	r3
	sub	10.(sp),r3
	mul	r0,r1
	mul	r2,r3
	add	r1,r3
	mul	r2,r0
	sub	r3,r0
	mov	(sp)+,r3
	mov	(sp)+,r2
	rts	pc

#ifdef	UCB_NET
	.globl  _htonl,_htons,_ntohl,_ntohs
_htonl:
_ntohl:
	mov     2(sp),r0
	mov     4(sp),r1
	swab    r0
	swab    r1
	rts     pc

_htons:
_ntohs:
	mov     2(sp),r0
	swab    r0
	rts     pc

	.globl  _badaddr
_badaddr:
	mov     2(sp),r0
	mov     nofault,-(sp)
	mov     $1f,nofault
	tst     (r0)
	clr     r0
	br      2f
1:
	mov     $-1,r0
2:
	mov     (sp)+,nofault
	rts     pc

	.globl  _bzero
_bzero:
	mov     2(sp),r0
	beq	3f		/ error checking...  dgc
	mov     4(sp),r1
	beq	3f		/ error checking ... dgc
	bit     $1,r0
	bne     1f
	bit     $1,r1
	bne     1f
	asr     r1
2:      clr     (r0)+
	sob     r1,2b
	rts     pc

1:      clrb    (r0)+
	sob     r1,1b
3:
	rts     pc
#endif  UCB_NET

	.globl	csv, cret
#ifndef	MENLO_KOV
csv:
	mov	r5,r0
	mov	sp,r5
	mov	r4,-(sp)
	mov	r3,-(sp)
	mov	r2,-(sp)
	tst     -(sp)
	jsr	pc,(r0)

cret:
	mov	r5,r2
	mov	-(r2),r4
	mov	-(r2),r3
	mov	-(r2),r2
	mov	r5,sp
	mov	(sp)+,r5
	rts	pc

#else	MENLO_KOV
	.globl  __ovno
	.data
__ovno:	0
	.text
/*
 * Inter-overlay calls call thunk which calls ovhndlr to
 * save registers.  Intra-overlay calls may call function
 * directly which calls csv to save registers.
 */
csv:
	mov	r5,r1
	mov	sp,r5
	mov	__ovno,-(sp)		/ overlay is extra (first) word in mark
	mov	r4,-(sp)
	mov	r3,-(sp)
	mov	r2,-(sp)
	jsr	pc,(r1)			/ jsr part is sub $2,sp

cret:
	mov	r5,r2
	/ Get the overlay out of the mark, and if it is non-zero
	/ make sure it is the currently loaded one.
	mov	-(r2),r4
	bne	1f			/ zero is easy
2:
	mov	-(r2),r4
	mov	-(r2),r3
	mov	-(r2),r2
	mov	r5,sp
	mov	(sp)+,r5
	rts	pc

	/ Not returning to base segment, so check that the right
	/ overlay is mapped in, and if not change the mapping.
1:
	cmp	r4,__ovno
	beq	2b			/ lucked out!

	/ If return address is in base segment, then nothing to do.
	cmp	2(r5),$_etext
	blos	2b

	/ Returning to wrong overlay --- do something!
	mov	PS,-(sp)		/ save PS 
	SPL7
	mov	r4,__ovno
	asl	r4
	mov	ova(r4), OVLY_PAR
	mov	ovd(r4), OVLY_PDR
	mov	(sp)+,PS			/ restore PS, unmask interrupts
	/ Could measure switches[ovno][r4]++ here.
	jmp	2b

/*
 * Ovhndlr1 through ovhndlr7  are called from the thunks,
 * after the address to return to is put in r1.  This address is
 * that after the call to csv, which will be skipped.
 */
.globl  ovhndlr1, ovhndlr2, ovhndlr3, ovhndlr4, ovhndlr5, ovhndlr6, ovhndlr7
#ifdef  IPK_XKOV
.globl  ovhndlr8, ovhndlr9, ovhndlra, ovhndlrb, ovhndlrc, ovhndlrd, ovhndlre
.globl  ovhndlrf, ovhndlrg, ovhndlrh, ovhndlri, ovhndlrj, ovhndlrk, ovhndlrl
.globl  ovhndlrm, ovhndlrn, ovhndlro, ovhndlrp, ovhndlrq, ovhndlrr, ovhndlrs
.globl  ovhndlrt, ovhndlru, ovhndlrv
#endif
ovhndlr1:       mov     $1,r0;  br      ovhndlr
ovhndlr2:       mov     $2,r0;  br      ovhndlr
ovhndlr3:       mov     $3,r0;  br      ovhndlr
ovhndlr4:       mov     $4,r0;  br      ovhndlr
ovhndlr5:       mov     $5,r0;  br      ovhndlr
ovhndlr6:       mov     $6,r0;  br      ovhndlr
ovhndlr7:       mov     $7,r0;  br      ovhndlr
#ifdef  IPK_XKOV
ovhndlr8:       mov    $10,r0;  br      ovhndlr
ovhndlr9:       mov    $11,r0;  br      ovhndlr
ovhndlra:       mov    $12,r0;  br      ovhndlr
ovhndlrb:       mov    $13,r0;  br      ovhndlr
ovhndlrc:       mov    $14,r0;  br      ovhndlr
ovhndlrd:       mov    $15,r0;  br      ovhndlr
ovhndlre:       mov    $16,r0;  br      ovhndlr
ovhndlrf:       mov    $17,r0;  br      ovhndlr
ovhndlrg:       mov    $20,r0;  br      ovhndlr
ovhndlrh:       mov    $21,r0;  br      ovhndlr
ovhndlri:       mov    $22,r0;  br      ovhndlr
ovhndlrj:       mov    $23,r0;  br      ovhndlr
ovhndlrk:       mov    $24,r0;  br      ovhndlr
ovhndlrl:       mov    $25,r0;  br      ovhndlr
ovhndlrm:       mov    $26,r0;  br      ovhndlr
ovhndlrn:       mov    $27,r0;  br      ovhndlr
ovhndlro:       mov    $30,r0;  br      ovhndlr
ovhndlrp:       mov    $31,r0;  br      ovhndlr
ovhndlrq:       mov    $32,r0;  br      ovhndlr
ovhndlrr:       mov    $33,r0;  br      ovhndlr
ovhndlrs:       mov    $34,r0;  br      ovhndlr
ovhndlrt:       mov    $35,r0;  br      ovhndlr
ovhndlru:       mov    $36,r0;  br      ovhndlr
ovhndlrv:       mov    $37,r0;  br      ovhndlr
#endif
/*
 * Ovhndlr makes the argument (in r0) be the current overlay,
 * saves the registers ala csv (but saves the previous overlay number),
 * and then jmp's to the function, skipping the function's initial
 * call to csv.
 */
ovhndlr:
	mov	sp,r5
	mov	__ovno,-(sp)		/ save previous overlay number
	cmp	r0,__ovno		/ correct overlay mapped?
	bne	2f
1:	mov	r4,-(sp)
	mov	r3,-(sp)
	mov	r2,-(sp)
	jsr	pc,(r1)			/ skip function's call to csv

2:	mov	PS,-(sp)		/ save PS
	SPL7
	mov	r0,__ovno		/ set new overlay number
	asl	r0
	mov	ova(r0), OVLY_PAR
	mov	ovd(r0), OVLY_PDR
	mov	(sp)+,PS		/ restore PS, unmask interrupts
	jbr	1b
#endif	MENLO_KOV

/*
 * Save regs r0, r1, r2, r3, r4, r5, r6, K[DI]SA6
 * starting at data location 0300, in preparation for dumping core.
 */
	.globl	_saveregs
_saveregs:
#ifdef	KERN_NONSEP
	movb	$RW, KISD0		/ write enable
#endif
	mov	r0,300
	mov	$302,r0
	mov	r1,(r0)+
	mov	r2,(r0)+
	mov	r3,(r0)+
	mov	r4,(r0)+
	mov	r5,(r0)+
	mov	sp,(r0)+
	mov	KDSA6,(r0)+
	mov	KDSA5,(r0)+
	rts	pc

	.globl _nulldev, _nullsys

_nulldev:		/ placed in insignificant entries in bdevsw and cdevsw
_nullsys:		/ ignored system call
	rts	pc

	.globl	_u
_u	= 140000
#if !defined(UCB_NET) && !defined(IPK_XOVLY)
usize	= 16.
#else
usize   = 32.
#endif

	.data
	.globl  _ka6, _cputype, _sep_id, _ubmap, _q22bus
#ifdef	ENABLE34
	.globl	_enable34
_enable34: .byte 0
#endif
_ubmap:	.byte 0
_sep_id: .byte 0
_q22bus: .byte 0
	.even

#ifdef	KERN_NONSEP
#ifdef	ENABLE34
_ka6:	DEC_KISA6
#else
_ka6:	KISA6
#endif
_cputype:40.

#else	KERN_NONSEP
#ifdef	ENABLE34
_ka6:	DEC_KDSA6
#else
_ka6:	KDSA6
#endif
_cputype:45.
#endif	KERN_NONSEP

	.bss
	.even
intstk:	.=.+INTSTK		/ temporary stack while KDSA6 is repointed
eintstk: .=.+2			/ initial top of intstk

	.data
nofault:.=.+2
#ifndef	NONFP
fpp:	.=.+2
#endif
ssr:	.=.+6
#ifdef	DISPLAY
dispdly:.=.+2
#endif
saveps:	.=.+2

#if	defined(PROFILE) && !defined(ENABLE34)
	.text
/*
 * System profiler
 *
 * Expects to have a KW11-P in addition to the line-frequency
 * clock, and it should be set to BR7.
 * Uses supervisor I space register 2 and 3 (040000 - 0100000)
 * to maintain the profile.
 */

CCSB	= 172542
CCSR	= 172540
_probsiz = 37777

	.globl	_isprof, _sprof, _probsiz, _mode
/
/ Enable clock interrupts for system profiling
/
_isprof:
	mov	$1f,nofault
	mov	$_sprof,104		/ interrupt
	mov	$340,106		/ pri
	mov	$100.,CCSB		/ count set = 100
	mov	$113,CCSR		/ count down, 10kHz, repeat
1:
	clr	nofault
	rts	pc

/
/ Process profiling clock interrupts
/
_sprof:
	mov	r0,-(sp)
	mov	PS,r0
	ash	$-10.,r0
	bic	$!14,r0			/ mask out all but previous mode
	add	$1,_mode+2(r0)
	adc	_mode(r0)
	cmp	r0,$14			/ user
	beq	done
	mov	2(sp),r0		/ pc
	asr	r0
	asr	r0
	bic	$140001,r0
	cmp	r0,$_probsiz
	blo	1f
	inc	_outside
	br	done
1:
	mov	$10340,PS		/ Set previous mode to supervisor
	mfpi	40000(r0)
	inc	(sp)
	mtpi	40000(r0)
done:
	mov	(sp)+,r0
	mov	$113,CCSR
	rtt

	.data
_mode:	.=.+16.
_outside: .=.+2
#endif	defined(PROFILE) && !defined(ENABLE34)


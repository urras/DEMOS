/ $Header: ldiv.s,v 1.1 88/04/01 17:52:40 andrew Exp $
/ $Log:	ldiv.s,v $
/ Revision 1.1  88/04/01  17:52:40  andrew
/ Initial revision
/ 
/ Long quotient

.globl	ldiv
.globl	csv, cret

ldiv:
	jsr	r5,csv
	mov	10.(r5),r3
	sxt	r4
	bpl	1f
	neg	r3
1:
	cmp	r4,8.(r5)
	bne	hrdldiv1
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
	mov	r0,r4		/high quotient
	mov	r1,-(sp)	/ Stash interim result.
	mov	r1,r0
	mov	r2,r1
	div	r3,r0
	bvc	1f
	mov	(sp),r0		/ Recover interim result.
	mov	r2,r1		/ (regs may be clobbered by failed div.)
	sub	r3,r0		/ this is the clever part
	div	r3,r0
	tst	r1
	sxt	r1
	add	r1,r0		/ cannot overflow!
1:
	tst	(sp)+		/ Pop temp off stack.
	mov	r0,r1
	mov	r4,r0
	tst	(sp)+
	bpl	9f
	neg	r0
	neg	r1
	sbc	r0
9:
	jmp	cret

/ The divisor is known to be >= 2^15 so only 16 cycles are needed.
hrdldiv1:
	clr	-(sp)
	mov	6.(r5),r2
	mov	4.(r5),r1
	bpl	1f
	com	(sp)
	neg	r1
	neg	r2
	sbc	r1
1:
	clr	r0
	mov	8.(r5),r3
	bge	1f
	neg	r3
	neg	10.(r5)
	sbc	r3
	com	(sp)
1:
	mov	$16.,r4
1:
	clc
	rol	r2
	rol	r1
	rol	r0
	cmp	r3,r0
	bgt	3f
	blt	2f
	cmp	10.(r5),r1
	blos	2f
3:
	sob	r4,1b
	br	1f
2:
	sub	10.(r5),r1
	sbc	r0
	sub	r3,r0
	inc	r2
	sob	r4,1b
1:
	mov	r2,r1
	clr	r0
	tst	(sp)+
	beq	1f
	neg	r0
	neg	r1
	sbc	r0
1:
	jmp	cret
/ $Header: lrem.s,v 1.1 88/04/01 17:52:52 andrew Exp $
/ $Log:	lrem.s,v $
/ Revision 1.1  88/04/01  17:52:52  andrew
/ Initial revision
/ 
/ Long remainder

.globl	lrem
.globl	csv, cret

lrem:
	jsr	r5,csv
	mov	10.(r5),r3
	sxt	r4
	bpl	1f
	neg	r3
1:
	cmp	r4,8.(r5)
	bne	hrdlrem1
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
	mov	r1,-(sp)	/ Stash interim result.
	mov	r1,r0
	mov	r2,r1
	div	r3,r0
	bvc	1f
	mov	(sp),r0		/ Recover interim result.
	mov	r2,r1		/ (regs may be clobbered by failed div.)
	sub	r3,r0
	div	r3,r0
	tst	r1
	beq	9f
	add	r3,r1
1:
	tst	(sp)+		/ Pop temp off stack.
	tst	r4
	bpl	9f
	neg	r1
9:
	sxt	r0
	jmp	cret

/ The divisor is known to be >= 2^15.  Only 16 cycles are
/ needed to get a remainder.
hrdlrem1:
	mov	6.(r5),r2
	mov	4.(r5),r1
	bpl	1f
	neg	r1
	neg	r2
	sbc	r1
1:
	clr	r0
	mov	8.(r5),r3
	bge	1f
	neg	r3
	neg	10.(r5)
	sbc	r3
1:
	mov	$16.,r4
1:
	clc
	rol	r2
	rol	r1
	rol	r0
	cmp	r3,r0
	blt	2f
	bgt	3f
	cmp	10.(r5),r1
	blos	2f
3:
	sob	r4,1b
	br	1f
2:
	sub	10.(r5),r1
	sbc	r0
	sub	r3,r0
	sob	r4,1b
1:
	tst	4(r5)
	bge	1f
	neg	r0
	neg	r1
	sbc	r0
1:
	jmp	cret
/ $Header: lmul.s,v 1.1 88/04/01 17:52:46 andrew Exp $
/ $Log:	lmul.s,v $
/ Revision 1.1  88/04/01  17:52:46  andrew
/ Initial revision
/ 
/
/ 32-bit multiplication routine for fixed pt hardware.
/  Implements * operator
/ Credit to an unknown author who slipped it under the door.
.globl	lmul
.globl	csv, cret

lmul:
	jsr	r5,csv
	mov	6(r5),r2
	sxt	r1
	sub	4(r5),r1
	mov	10.(r5),r0
	sxt	r3
	sub	8.(r5),r3
	mul	r0,r1
	mul	r2,r3
	add	r1,r3
	mul	r2,r0
	sub	r3,r0
	jmp	cret
/ $Header: alrem.s,v 1.1 88/04/01 17:52:37 andrew Exp $
/ $Log:	alrem.s,v $
/ Revision 1.1  88/04/01  17:52:37  andrew
/ Initial revision
/ 
/ Long remainder

.globl	alrem
.globl	csv, cret
alrem:
	jsr	r5,csv
	mov	8.(r5),r3
	sxt	r4
	bpl	1f
	neg	r3
1:
	cmp	r4,6.(r5)
	bne	hrdlrem2
	mov	4.(r5),r0
	mov	2(r0),r2
	mov	(r0),r1
	mov	r1,r4
	bge	1f
	neg	r1
	neg	r2
	sbc	r1
1:
	clr	r0
	div	r3,r0
	mov	r1,-(sp)	/ Stash interim result.
	mov	r1,r0
	mov	r2,r1
	div	r3,r0
	bvc	1f
	mov	(sp),r0		/ Recover interim result.
	mov	r2,r1		/ (Regs may be clobbered by failed div.)
	sub	r3,r0
	div	r3,r0
	tst	r1
	beq	9f
	add	r3,r1
1:
	tst	(sp)+		/ Pop temp off stack.
	tst	r4
	bpl	9f
	neg	r1
9:
	sxt	r0
	mov	4.(r5),r3
	mov	r0,(r3)+
	mov	r1,(r3)
	jmp	cret

/ The divisor is known to be >= 2^15.  Only 16 cycles are
/ needed to get a remainder.
hrdlrem2:
	mov	4.(r5),r0
	mov	2(r0),r2
	mov	(r0),r1
	bpl	1f
	neg	r1
	neg	r2
	sbc	r1
1:
	clr	r0
	mov	6.(r5),r3
	bge	1f
	neg	r3
	neg	8.(r5)
	sbc	r3
1:
	mov	$16.,r4
1:
	clc
	rol	r2
	rol	r1
	rol	r0
	cmp	r3,r0
	blt	2f
	bgt	3f
	cmp	8.(r5),r1
	blos	2f
3:
	sob	r4,1b
	br	1f
2:
	sub	8.(r5),r1
	sbc	r0
	sub	r3,r0
	sob	r4,1b
1:
	mov	4.(r5),r3
	tst	(r3)
	bge	1f
	neg	r0
	neg	r1
	sbc	r0
1:
	mov	r0,(r3)+
	mov	r1,(r3)
	jmp	cret
/ $Header: almul.s,v 1.1 88/04/01 17:52:35 andrew Exp $
/ $Log:	almul.s,v $
/ Revision 1.1  88/04/01  17:52:35  andrew
/ Initial revision
/ 
/
/ 32-bit multiplication routine for fixed pt hardware.
/  Implements *= operator
/ Credit to an unknown author who slipped it under the door.
.globl	almul
.globl	csv, cret

almul:
	jsr	r5,csv
	mov	4(r5),r4
	mov	2(r4),r2
	sxt	r1
	sub	(r4),r1
	mov	8.(r5),r0
	sxt	r3
	sub	6.(r5),r3
	mul	r0,r1
	mul	r2,r3
	add	r1,r3
	mul	r2,r0
	sub	r3,r0
	mov	r0,(r4)+
	mov	r1,(r4)
	jmp	cret
/ $Header: aldiv.s,v 1.1 88/04/01 17:52:29 andrew Exp $
/ $Log:	aldiv.s,v $
/ Revision 1.1  88/04/01  17:52:29  andrew
/ Initial revision
/ 
/ Long quotient

.globl	aldiv
.globl	csv, cret
aldiv:
	jsr	r5,csv
	mov	8.(r5),r3
	sxt	r4
	bpl	1f
	neg	r3
1:
	cmp	r4,6.(r5)
	bne	hrdldiv2
	mov	4(r5),r1
	mov	2(r1),r2
	mov	(r1),r1
	bge	1f
	neg	r1
	neg	r2
	sbc	r1
	com	r4
1:
	mov	r4,-(sp)
	clr	r0
	div	r3,r0
	mov	r0,r4		/high quotient
	mov	r1,-(sp)	/ Stash interim result
	mov	r1,r0
	mov	r2,r1
	div	r3,r0
	bvc	1f
	mov	(sp),r0		/ Recover interim result.
	mov	r2,r1		/ (Regs may be clobbered by failed div.)
	sub	r3,r0		/ this is the clever part
	div	r3,r0
	tst	r1
	sxt	r1
	add	r1,r0		/ cannot overflow!
1:
	tst	(sp)+		/ Pop temp off stack
	mov	r0,r1
	mov	r4,r0
	tst	(sp)+
	bpl	9f
	neg	r0
	neg	r1
	sbc	r0
9:
	mov	4.(r5),r2
	mov	r0,(r2)+
	mov	r1,(r2)
	jmp	cret

/ The divisor is known to be >= 2^15 so only 16 cycles are needed.
hrdldiv2:
	clr	-(sp)
	mov	4.(r5),r0
	mov	2(r0),r2
	mov	(r0),r1
	bpl	1f
	com	(sp)
	neg	r1
	neg	r2
	sbc	r1
1:
	clr	r0
	mov	6.(r5),r3
	bge	1f
	neg	r3
	neg	8.(r5)
	sbc	r3
	com	(sp)
1:
	mov	$16.,r4
1:
	clc
	rol	r2
	rol	r1
	rol	r0
	cmp	r3,r0
	bgt	3f
	blt	2f
	cmp	8.(r5),r1
	blos	2f
3:
	sob	r4,1b
	br	1f
2:
	sub	8.(r5),r1
	sbc	r0
	sub	r3,r0
	inc	r2
	sob	r4,1b
1:
	mov	r2,r1
	clr	r0
	tst	(sp)+
	beq	1f
	neg	r0
	neg	r1
	sbc	r0
1:
	mov	4.(r5),r2
	mov	r0,(r2)+
	mov	r1,(r2)
	jmp	cret

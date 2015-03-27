/begin
	.data
	.even
L16:
	0.;2.
	.even
L17:
	0.;3.
	.even
L18:
	0.;1.
	.bss
~~pt2_ = _pt2_
~f = 4
~~:
	.data
	.globl	_pt2_
	.globl	_s_wsle
	.globl	_do_lio
	.globl	_e_wsle
	.even
.text
jbr	L11
L14:clr	-124(r5)
clr	-122(r5)
mov	$6,-116(r5)
sxt	-120(r5)
clr	-110(r5)
mov	r5,(sp)
add	$-124,(sp)
jsr	pc,*$_s_wsle
mov	$L16,-(sp)
jsr	pc,*(r4)
tst	(sp)+
mov	r0,-130(r5)
mov	r1,-126(r5)
mov	$4,-(sp)
sxt	-(sp)
mov	r5,-(sp)
add	$-130,(sp)
mov	$L18,-(sp)
mov	$L17,-(sp)
jsr	pc,_do_lio
add	$12,sp
jsr	pc,_e_wsle
jbr	L13
L13:L12:jmp	cret
L11:_pt2_:
~~pt2_:
jsr	r5,csv
sub	$130,sp
mov	r5,r4
add	$4,r4
jbr	L14
.globl
.data

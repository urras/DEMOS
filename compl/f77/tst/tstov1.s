/begin
	.data
	.even
L14:
.byte 0,0
	.bss
~~MAIN__ = _MAIN__
~~:
	.data
	.bss
~~aaa_ = _aaa_
~x = 4
~~:
	.data
	.globl	_MAIN__
	.globl	_pt2_
	.globl	_aaa_
	.globl	_s_stop
	.even
.text
jbr	L11
L12:clr	-(sp)
sxt	-(sp)
mov	$_aaa_,-(sp)
jsr	pc,_pt2_
add	$6,sp
clr	-(sp)
sxt	-(sp)
mov	$L14,-(sp)
jsr	pc,_s_stop
add	$6,sp
L13:jmp	cret
L11:_MAIN__:
~~MAIN__:
jsr	r5,csv
sub	$10,sp
mov	r5,r4
add	$4,r4
jbr	L12
.text
jbr	L15
L17:mov	(r4),r0
mov	+2(r0),r1
mov	(r0),r0
mov	r1,-(sp)
mov   r0,-(sp)
mov	(r4),r0
mov	+2(r0),r1
mov	(r0),r0
mov	r1,-(sp)
mov   r0,-(sp)
jsr	pc,lmul
add	$10,sp
mov	r0,-20(r5)
mov	r1,-16(r5)
jbr	L16
L16:mov	-16(r5),r1
mov	-20(r5),r0
jmp	cret
L15:_aaa_:
~~aaa_:
jsr	r5,csv
sub	$20,sp
mov	r5,r4
add	$4,r4
jbr	L17
.globl
.data

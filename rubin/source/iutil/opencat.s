	.data
	.globl
_Sccsid:
	.word	16424
	.word	9001
	.word	28528
	.word	25966
	.word	25441
	.word	29742
	.word	25353
	.word	14126
	.word	12553
	.word	12847
	.word	13615
	.word	14385
	.word	0
	.text
	.globl	_opencat
_opencat:
@@opencata:
	link	a6,#-.F1
	moveml	#.S1,-.F1(a6)
	.text
@i=-4
@name=8
@mode=12
!	line 68, file "opencat.c"
	movl	8(a6),a4
!	line 71, file "opencat.c"
	movl	#_Desxx,a3
.L31:
!	line 71, file "opencat.c"
	tstl	(a3)
	beq	.L30
!	line 72, file "opencat.c"
	movl	(a3),-(sp)
	pea	(a4)
	jsr	_sequal
	addql	#8,sp
	tstl	d0
	beq	.L33
	bra	.L30
.L33:
.L29:
!	line 74, file "opencat.c"
	addl	#12,a3
	bra	.L31
.L30:
!	line 74, file "opencat.c"
	tstl	(a3)
	bne	.L34
	.text
!	line 75, file "opencat.c"
	pea	(a4)
	movl	#.L36,-(sp)
	jsr	_syserr
	addql	#8,sp
.L34:
!	line 77, file "opencat.c"
	movl	4(a3),a5
!	line 80, file "opencat.c"
	tstw	58(a5)
	beq	.L37
	.text
!	line 82, file "opencat.c"
	pea	(a5)
	jsr	_clearke
	addql	#4,sp
	bra	.L39
.L37:
	.text
!	line 87, file "opencat.c"
	tstl	8(a3)
	beq	.L40
	.text
!	line 89, file "opencat.c"
	jsr	_acc_ini
!	line 90, file "opencat.c"
	movl	#368,-(sp)
	pea	(a5)
	movl	8(a3),-(sp)
	jsr	_bmove
	addl	#12,sp
	bra	.L43
.L40:
	.text
!	line 94, file "opencat.c"
	pea	(a4)
	movl	#2,-(sp)
	pea	(a5)
	jsr	_openr
	addl	#12,sp
	movl	d0,-4(a6)
	beq	.L45
	.text
!	line 95, file "opencat.c"
	movl	-4(a6),-(sp)
	pea	(a4)
	movl	#.L46,-(sp)
	jsr	_syserr
	addl	#12,sp
.L45:
.L43:
!	line 99, file "opencat.c"
	movw	56(a5),d0
	extl	d0
	moveq	#1,d1
	asll	d0,d1
	orl	d1,_CmOfile
.L39:
!	line 103, file "opencat.c"
	movl	12(a6),d0
	bra	.L48
	.text
.L49:
!	line 106, file "opencat.c"
	movw	58(a5),d0
	extl	d0
	movl	d0,-(sp)
	jsr	_abs
	addql	#4,sp
	movw	d0,58(a5)
	bra	.L47
.L51:
!	line 110, file "opencat.c"
	movw	58(a5),d0
	extl	d0
	movl	d0,-(sp)
	jsr	_abs
	addql	#4,sp
	negl	d0
	movw	d0,58(a5)
	bra	.L47
.L52:
	.text
!	line 114, file "opencat.c"
	movl	12(a6),-(sp)
	pea	(a4)
	movl	#.L53,-(sp)
	jsr	_syserr
	addl	#12,sp
	bra	.L47
.L48:
	cmpl	#0,d0
	beq	.L49
	cmpl	#2,d0
	beq	.L51
	bra	.L52
.L47:
	bra	.L26
	bra	.L26
.L26:	moveml	-.F1(a6),#0x3800
	unlk	a6
	rts
@p=a3
@n=a4
@d=a5
.F1 = 16
.S1 = 0x3800
	.data
.L36:
	.byte	111,112,101,110,99,97,116,97
	.byte	108,111,103,58,32,40,37,115
	.byte	41,0
.L46:
	.byte	111,112,101,110,99,97,116,97
	.byte	108,111,103,58,32,111,112,101
	.byte	110,114,40,37,115,41,32,37
	.byte	100,0
.L53:
	.byte	111,112,101,110,99,97,116,97
	.byte	108,111,103,40,37,115,41,58
	.byte	32,109,111,100,101,32,37,100
	.byte	0

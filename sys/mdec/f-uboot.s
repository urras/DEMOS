/ ��������� � ����� � ������� ���������� ��� ��� (aka MY:). zaitcev.
/
/  UCB_BSIZE = 1, �� ������ ������ ���������� ������ ���� ������ 494 ����.
/
/ ������������ ����� ������ ����� �� "uboot.s". � ��������� ���� ��������
/ ��� ������� � ��� ������ � �������� ����������, ������� �� ��������������
/ � ���������� ��������.

major=12.
/ korotay -> major=2.

/ ------ K��������: --------------------------------------------------------
CLSIZE  = 2.                    / ���. ������ ����� �� ���� ����������
CLSHFT  = 1.                    / ����� ��� ��������� �� CLSIZE
BSIZE   = 512.*CLSIZE           / ���������� ������ �����
INOSIZ  = 64.                   / ������ I-���� � ������
INOSHF  = 6.
NDIRIN  = 4.                    / ����� ������ ������� I-����
ADDROFF = 12.                   / �������� ������� ������ � I-����
INOPB   = BSIZE\/INOSIZ         / ����� I-����� � ���������� �����
INOFF   = 31.                   / �������� I-����� = (INOPB * (SUPERB+1)) - 1

WC      = 256.*CLSIZE           / ������� ����
/SECTRK  = 9.			/ PC-writeable via BIOS
SECTRK  = 10.			/ MISS is used for raw writing

/  ����� �������� � ���������� ��������� ����� � ���������
/  SZFLAGS ������ ������ ��� ������������.
ENDCORE    =    160000          / ����� ������ ��� ����������� ��
BOOTOPTS   =    2               / ������������ ������, ������ ����� ENDCORE
BOOTDEV    =    4		/ <major><minor>
CHECKWORD  =    6
RBOOTDEV   =    8.
SZFLAGS    =	8.              / ������ ������ ����������

__ = ENDCORE-512.-SZFLAGS       / ����� ��� ������ ����������
				/ � ��������� ���� ".." � ������� ��������
_B = ENDCORE-512.		/ ������� ����� ����������

start:
	nop			/ ������� ���� ������� ��������� ���
				/ ��-�� ����� ���������� � ����������
				/ �� �������� �������� "BMY0" �� M3 CPU.
	mtps    $340		/ ������ ������� ��� ����� �� ��������!

	mov     $__,sp		/ ������ sp "��� ����".
/ �������� ���� � ����� ������
	mov     $_B,r1
	clr     r0
	cmp     (r0),$407	/ ��������� �� � ���� ������
	bne     1f
	mov     $20,r0		/ �������� ��������� (����� �������� ������)
1:
	mov     (r0)+,(r1)+
	cmp     r1,$ENDCORE
	blo     1b
	jmp	*$_B+restart

/ ��� ������ ��������������� ������.
restart:

/ �������� ������
	clr     r0
	clr	r1
1:
	mov     r1,(r0)+	/ ���� �����, ��� clr (r0)+ �� ��������
	mov	r1,(r0)+
	cmp     r0,sp
	blo     1b

/
/ ��������� ����� ��� ����������� ������ �-�� ��������
/
	mov     *$ENDCORE-BOOTOPTS,r4
	com     r4
	cmp     *$ENDCORE-CHECKWORD,r4
	beq     9f
/
/ �������� ���������� �����������. ��������� ���������� ����������
/ ����� ������� ��������� � �������� ������ ����� ���������, �������
/ �������� ����� ���������� �� ����� 01400.
/
	mov	*$172142,r0
	ash	$-8.,r0
	bic	$!3,r0
	bis	$[major\<8.],r0
	mov	r0,*$ENDCORE-BOOTDEV
9:

/ ��������� ������ I-�����
/ � ��������� �������� � ����� �� ���������
	mov     $_B+names,r1
	mov     $2,r0
nextfile:
	clr     bno		/ �������� ��� rmblk()
	jsr     pc,iget
	tst     (r1)		/ ��� ��� �� �������
	beq     1f
	mov	$_B+slash,r0
	jsr	pc,puts
2:
	jsr     pc,rmblk
		br restart
	mov     $buf,r2
3:
	mov	r1,r0
	mov     r1,r3
	mov     r2,r4
	add     $16.,r2
	tst     (r4)+
	beq     5f
4:
	cmpb    (r3)+,(r4)+
	bne     5f
	cmp     r4,r2
	blo     4b
	jsr	pc,puts
	mov     -16.(r2),r0
	add     $14.,r1
	br      nextfile
5:
	cmp     r2,$buf+BSIZE
	blo     3b
	br      2b
1:

/ ������ ���� � ������ �� ������ ������
/ ("��� ������ �����")
	clr     r1
1:
	jsr     pc,rmblk
		br 1f
	mov     $buf,r2
2:
	mov     (r2)+,(r1)+
	cmp     r2,$buf+BSIZE
	blo     2b
	br      1b
1:

/ ����������� ������ �� ����� ������������� ���������
	clr     r0
	cmp     (r0),$407
	bne     2f
1:
	mov     20(r0),(r0)+
	cmp     r0,sp
	blo     1b
2:

	mov	$_B+crlf,r0
	jsr	pc,puts

// �������� ���������� ��������� �
// ���������������, ���� ��� ������� ����������
/	mov     *$ENDCORE-BOOTOPTS, r4
/	mov     *$ENDCORE-BOOTDEV, r3
/	mov     *$ENDCORE-CHECKWORD, r2
/
/ ������� � r1 �������� ��������� �-�� ��������
/ (major ����� ���������� ������ ��� hardboot-�)
/
/	movb    r3, r1
/	bis     $[major\<8.],r1
/	jsr     pc,*$0
/	br      restart

/ �������� ��������� � ���������
	clr	r1      	/ high byte contains drive no
	bis     $[major\<8.],r1 / rbootdev --> r1
	clr     r2              / clear bootopts, checkword, bootdev ...
	clr     r3
	clr     r4
	jsr     pc,*$0
	br      .

/ ����� I-���� �������� � r0
/ ������������� ����� � ������ i-����:  [i_type:2|i_block:10|i_off:4]
iget:
	add     $INOFF,r0
	mov     r0,r5
	ash     $-4.,r0
	bic     $!1777,r0
	mov     r0,dno
	clr     r0
	jsr     pc,rblk
	bic     $!17,r5
	mul     $INOSIZ,r5
	add     $buf,r5
	mov     $inod,r4
1:
	mov     (r5)+,(r4)+
	cmp     r4,$inod+INOSIZ
	blo     1b
	rts     pc

/ ������ ���� ����� � ���������� ��������� � bno.
/ �������� ����� �������� ������ � �������������� �������.
/ ����� �������, ����� ���� ��������� ������ ����� ������
/ NDIRIN+128 ������.
/
/   r3='NDIRIN'
/   if r5(bno)>=r3 then r5=r3
/   dno:=r5^ ; (dno+1):=r5^
/   r0:='-3'(r5)
/   if = or dno=0 goto noblk
/   call rblk
/   r5=bno ; bno=bno+1
/   (if r5(r5-r3)>=             % ��� �� ��������� ���� ������
/     r5=<2
/     r0='buf'(r5) ; dno='buf+2'(r5)
/     if = or r0=0 goto noblk
/     call rblk
/   )if
/   @sp=+2
/ noblk:
/   return =
/
rmblk:
	mov	$NDIRIN,r3
	mov	$_B+dno,r2
	mov     bno,r5
	cmp     r5,r3
	blt     1f
	mov     r3,r5
1:
	mul	$3,r5
	add     $addr+1,r5
	movb    (r5)+,*r2
	movb    (r5)+,1(r2)
	movb    -3(r5),r0
	bne     1f
	tst     *r2
	beq     noblk
1:
	jsr     pc,rblk
	mov     bno,r5
	inc     bno
	sub     r3,r5
	blt     1f
	ash     $2,r5
	mov     buf(r5),r0
	mov     buf+2(r5),*r2
	bne	2f
	tst     r0
	beq	noblk
2:
	jsr	pc,rblk
1:
	add     $2,(sp)
noblk:
	rts     pc

/--------------------
fycs  = 172140
fydb  = 172142

GO    = 1
DONE  = 40
TR    = 200
ERR   = 100000

READ  = 0

/ ����������� ���� ��� �����������
cb1:			/ 15  13       8  7   3  2        1   0
	0		/ <00><����.���.><00000><������.><������>
	buf		/ <                         ����� ������>
	2		/ <����� �������><         ����� �������>
	WC		/ <���������� ���� ������               >

/ �����-������� �����.
/ ��. ����� ������ � dno, ������� � r0.
/ ��������� ������ �������� � ������ buf.
/ �� ������ �� ��������, ������ �������������.
/ �� ������ �������� r4, r5 - ����� ���������� ���� ���� � iget, rmblk.
rblk:
	mov	r1,-(sp)
	mov     dno,r1
	ashc    $CLSHFT,r0      / �������� �� CLSIZE
	div	$SECTRK,r0
	/ Assume quotient in r0, reminder in r1.
	inc	r1		/ ������� ���� � �������
	movb	r1,cb1+4
	clr	r1
	asr	r0		/ ������� -> r0, ����������� -> C
	rol	r1		/ C -> r1
	asl	r1
	asl	r1
	mov	r1,cb1
	movb	r0,cb1+5
	mov	$fycs,r1
1:	bit	$DONE,*r1
	beq	1b
	mov	$READ+GO,*r1
1:	bit	$TR,*r1
	beq	1b
	mov	$_B+cb1,2(r1)
1:	bit	$ERR+DONE,*r1
	beq	1b
1:	bmi	1b		/ ��� �� ������ ������ ��������
	mov	(sp)+,r1
	rts	pc

/ ����� ������ �� ��������.
tps = 177564
tpb = 177566

puts:
1:	tstb	*r0
	beq	2f
3:	tstb    *$tps
	bge     3b
	movb    (r0)+,*$tpb
	br	1b
2:	rts	pc

bno:	0
dno:	0
names:  <boot\0\0\0\0\0\0\0\0\0\0>
	0
crlf:	<\r\n\0>
slash:	</\0>
end:

/  ...<--------1K--------><---512---><BOPT=8><uboot=512>!<== ENDCORE=56K
/     [inod..][buf...........][stack]
/       !addr
/
inod = __-512.-BSIZE            / ����� ��� I-����, ������, �����
addr = inod+ADDROFF             / ������ ����� � I-����
buf  = inod+INOSIZ

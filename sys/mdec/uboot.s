/ ��������� � RK05       (�� 5400),
/             RP03       (�� 5061),
/             RK06/RK07  (�� 5408),
/             RP04/05/06,
/             RL01/02,
/             RM02/03/05,
/             Diva Comp. IV controller (33 �������)  (DVHP)
/             RD 50/51          (�����������-85)
/             ������ �-85
/             ������ ���-4 my
/
/  $Log:	uboot.s,v $
/ Revision 1.7  90/12/12  17:09:00  korotaev
/ ��������� my � dw (�� rw) ��� ���.
/ 
/ Revision 1.6  89/06/01  15:31:52  avg
/ ��������� ������ ��� ����� fs.
/ 
/ Revision 1.5  88/10/02  19:37:09  avg
/ ������� �������� � ���������� ������ �� RP � HK.
/
/ Revision 1.4  88/01/15  14:07:20  korotaev
/ ��������� ������� � ��������� rauboot, ������ ��������� ������ (!!!).
/ ��� ������� � ����.
/
/ Revision 1.3  88/01/05  16:28:58  korotaev
/ ��������� ����� �������� ���� ��� � ����.
/
/ Revision 1.2  86/07/13  14:06:02  avg
/ �������� ��������� � ����������  �����������-85.
/
/ ���� ��������� ������ ������������ � ���� #1 !!!
/
/ Revision 1.1  86/07/10  22:46:09  avg
/ Initial revision
/
/
/ ��� ������������� ������ �� 1 �� ���� ���������� CLSIZE � 2.
/ ����� ������ ������� I-���� - NDIRIN (������ 4)
/ ������ ������ ���� <= 512 ������; ���� > 494, �� ������� ��������
/ 16-������� ��������� a.out
/

/ ------ ����������������: -----------------------------------------------
#ifdef RK05
# define RK
major=0.
#endif
#ifdef RP03
# define RP
major=1.
#endif
#ifdef RK06
# define HK06
#endif
#ifdef RK07
# define HK07
#endif
#if defined(HK06) || defined(HK07)
# define HK
major=4.
#endif
#ifdef RP04
# define HP04
#endif
#ifdef RP05
# define HP05
#endif
#ifdef RP06
# define HP06
#endif
#if defined(HP04) || defined(HP05) || defined(HP06)
# define HP
major=6.
#endif
#if defined(RL01) || defined(RL02)
# define RL
major=8.
#endif
#if defined(RM05) || defined(RM03) || defined(RM02)
# define RM
major=6.
#endif
#if defined(RX02)
# define RX
#endif

#ifdef FD
major=12.
#endif
#ifdef DW
major=10.
#endif
#ifdef RX
major=11.
#endif
#ifdef MY
major=2.
#endif
/ ------ �����: ------------------------------------------------------------
nohead  = 1             / 0-> ����������, 1-> ���� ��������� �� ������
			/ ��������� ��������� a.out. ( -10 ������ )
readname= 0             / 1-> ����������, ���� ���� �� ��������� �� ������,
			/ ������ ��� � �������, 0-> ����������� ( -36 ������ )
prompt  = 1             / 1-> ����������� ('>') ����� ������� � �������
			/ 0-> ��� �����������. ( -8 ������ )
autoboot= 1             / 1-> ��� ��� ������������.
			/ 0-> ��� ������������, ( -12 ������ )
cyl     = 0.            / ����� ������� �������� �������� �������
			/ (rp,hk,hp,rm,dvhp)
drive   = 0             / ���������� ��� �������� (hp,rm,dvhp)

xautoboot=0             / ���. �������� �� � 0-�� �������

#ifdef RM
# ifdef RM05
   rm05 = 1             / 0-> RM02/03, 1-> RM05
# else
   rm05 = 0
# endif RM05
#endif

#ifdef HK
# ifdef HK07
   rk07 = 1             / 1-> RK07, 0-> RK06
# else
   rk07 = 0
# endif HK07
#endif

#ifdef RX
# ifdef RX02
   rx02 = 1             / 1-> RX02, 0-> RX
# else
   rx02 = 0
# endif RX02
#endif

/ ------ K��������: --------------------------------------------------------
CLSIZE  = 2.                    / ���. ������ ����� �� ���� ����������
#ifdef RX
.if     2-CLSIZE
      ��������� �������� ����� 512 ���� !!!
.endif
#endif /* RX */
CLSHFT  = 1.                    / ����� ��� ��������� �� CLSIZE
BSIZE   = 512.*CLSIZE           / ���������� ������ �����
INOSIZ  = 64.                   / ������ I-���� � ������
NDIRIN  = 4.                    / ����� ������ ������� I-����
ADDROFF = 12.                   / �������� ������� ������ � I-����
INOPB   = BSIZE\/INOSIZ         / ����� I-����� � ���������� �����
INOFF   = 31.                   / �������� I-����� = (INOPB * (SUPERB+1)) - 1
WC      = -256.*CLSIZE          / ������� ����

/  ����� �������� � ���������� ��������� ����� � ���������
/  SZFLAGS ������ ������ ��� ������������.
ENDCORE=        160000          / ����� ������ ��� ����������� ��
SZFLAGS=        8.              / ������ ������ ����������
BOOTOPTS=       2               / ������������ ������, ������ ����� ENDCORE
BOOTDEV=        4
CHECKWORD=      6
RBOOTDEV=       8.


#ifndef RX
.. = ENDCORE-512.-SZFLAGS       / ����� ��� ������ ����������
#else
.. = ENDCORE-1024.-SZFLAGS       / ����� ��� ������ ����������
#endif

/ ����������� ��� FD (������ �85)
#ifdef FD
fdaddr  = 174200
fdtrk   = 6  + fdaddr
fdsec   = 10 + fdaddr
fdcom   = 4  + fdaddr
fdstart = 24 + fdaddr
fdbuf   = 20 + fdaddr
fdzero  = 22 + fdaddr
fdstate = 4  + fdaddr
fdexcom = 16 + fdaddr
fdrs1   = 10 + fdaddr
fdrs2   = 12 + fdaddr
fdrs3   = 14 + fdaddr
EXCOM   = 130
#endif

start:
#ifdef EL85
mtps = 106400^tst
	0; 405; 0; 25; 0; 42020; 105041; 400
	mtps    $340
#endif
#ifdef NOP
	240                     / ���������� NOP
#endif NOP

/ ���������� ���������� �� jsr pc,*$0
/ ��� ��� ����� ���������� �� rts pc

/ ���������� sp, �����������
/ ��������� � ����� ������.

	mov     $..,sp

#ifdef RX
rxcs    = 177170
rxda    = 177172
reads   = 6
readb   = 2
go      = 1

/ ���������� ����������� �������
	mov     $7, r0
	mov     $200, r2
	mov     $3, r3
1:      jsr     pc, 5f
	add     $2, r3
	sob     r0, 1b
	br      9f
5:
1:      bit     $40, *$rxcs
	beq     1b
	mov     $reads+go, *$rxcs
1:      bit     $200, *$rxcs
	beq     1b
	mov     r3, *$rxda
1:      bit     $200, *$rxcs
	beq     1b
	mov     $1, *$rxda
1:      bit     $40, *$rxcs
	beq     1b
.if rx02-1
	mov     $200, r4
	mov     $readb+go, *$rxcs
1:      bit     $200, *$rxcs
	beq     1b
	movb    *$rxda, (r2)+
	sob     r4, 1b
.endif
.if rx02
	mov     $readb+go, *$rxcs
1:      bit     $200, *$rxcs
	beq     1b
	mov     $100, *$rxda
1:      bit     $200, *$rxcs
	beq     1b
	mov     r2, *$rxda
	add     $200, r2
.endif
	rts     pc
9:
#endif RX

	mov     sp,r1
#if !defined(RK)
.if xautoboot        / � �������� 0 �������� ����� ������ �� hardboot
	mov     r0,r5
.endif
#endif
	clr     r0
.if     nohead-1                        / ���� nohead == 1
	cmp     (r0),$407
	bne     1f
	mov     $20,r0
.endif
1:
	mov     (r0)+,(r1)+
	cmp     r1,$end
	blo     1b
	jmp     *$2f


/ ��� ������ ��������������� ������.
restart:

/ �������� ������
	clr     r0
2:
#if !defined(RL) && !defined(DW) && !defined(FD)
	mov     $0,(r0)+
#else
	clr     (r0)+
#endif
	cmp     r0,sp
	blo     2b

/
/ ��������� ����� ��� ����������� ������ �-�� ��������
/
.if xautoboot
	mov     ENDCORE-BOOTOPTS,r0
	com     r0
	cmp     ENDCORE-CHECKWORD,r0
	beq     9f
/
/ �������� ���������� �����������. ��������� ���������� ����������
/
#if !defined(RK)
	ash     $3,r5
	bic     $!70,r5
#endif

#ifdef RK
	mov     *$rkda,r5
	ash     $-13.,r5
	bic     $!7,r5
#endif
	mov     r5,ENDCORE-BOOTDEV
#ifdef DOUBLE
	movb    $128.,ENDCORE-BOOTDEV
#endif
9:
.endif

/ ���������������� ����

#ifdef RM
	mov     $drive,*$rmcs2
	mov     $preset+go,*$rmcs1
	mov     $fmt22,*$rmof
#endif

#if defined(HP) || defined(DVHP)
	mov     $drive,*$hpcs2
	mov     $preset+go,*$hpcs1
	mov     $fmt22,*$hpof
#endif

#ifdef RL
/       mov     $13,*$rlda      /get status
/       mov     $4,*$rlcs
/
/       jsr pc,rdy
/       mov *$rlda,r5           /superstision
/
#endif

#ifdef HK
	mov     $clear,*$hkcs2          / ����� ����������
	mov     $ack,*$hkcs1
0:
	tstb    *$hkcs1
	bpl     0b                      / ����� ���������� ��������
#endif

#ifdef RK
	mov     $reset+go,*$rkcs        / �������� ����������
#endif

#ifdef RP
.if 1-xautoboot
	clr     *$rpcs                  / ������� ���������� 0
.endif
.if xautoboot
	ash     $5,r5
	mov     r5, *$rpcs              / ������� ������ ����������
.endif
#endif
#ifdef MY
/ ���������������� ����
	mov     *$mydat, myparm
	swab    myparm
	mov     $reset+go, *$mycs
#endif MY

/ ������ ��� �����
#ifndef NOTTY
.if     prompt
	mov     $'>, r0
	jsr     pc, putc
.endif
#endif

/ ������������ � ������� 'names', ������ ����������
/ ���� � ������ 14 ������.
	mov     $names,r1
1:
	mov     r1,r2
2:
	jsr     pc,getc
#ifndef NOTTY
	cmp     r0,$'\n
#else
	cmp     r0,$'\r
#endif NOTTY
	beq     1f
.if     readname
	cmp     r0,$'/
	beq     3f
.endif
	movb    r0,(r2)+
	br      2b
.if     readname
3:
	cmp     r1,r2
	beq     2b
	add     $14.,r1
	br      1b
.endif

/ ��������� ������ I-�����
/ � ��������� �������� � ����� �� ���������
1:
	mov     $names,r1
	mov     $2,r0
1:
	clr     bno
	jsr     pc,iget
	tst     (r1)
	beq     1f
2:
	jsr     pc,rmblk
		br restart
	mov     $buf,r2
3:
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
	mov     -16.(r2),r0
	add     $14.,r1
	br      1b
5:
	cmp     r2,$buf+BSIZE
	blo     3b
	br      2b

/ ������ ���� � ������ �� ������ ������
/ ("��� ������ �����")
1:
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

/ ����������� ������ �� ����� ������������� ���������
1:
	clr     r0
	cmp     (r0),$407
	bne     2f
1:
	mov     20(r0),(r0)+
	cmp     r0,sp
	blo     1b

/ �������� ���������� ��������� �
/ ���������������, ���� ��� ������� ����������
2:
.if     autoboot
	mov     ENDCORE-BOOTOPTS, r4
	mov     ENDCORE-BOOTDEV, r3
	mov     ENDCORE-CHECKWORD, r2
/
/ ������� � r1 �������� ��������� �-�� ��������
/ (major ����� ���������� ������ ��� hardboot-�)
/
.if xautoboot
	movb    r3, r1
	bis     $[major\<8],r1
.endif
.if 1-xautoboot
	mov     $[major\<8],r1
.endif
.endif
	jsr     pc,*$0
	br      restart

/ ����� I-���� �������� � r0
iget:
	bic     $140000,r0      / Clear 2 most significant bits in I-number
	add     $INOFF,r0
	mov     r0,r5
	ash     $-4.,r0
	bic     $!7777,r0
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
rmblk:
	add     $2,(sp)
	mov     bno,r0
	cmp     r0,$NDIRIN
	blt     1f
	mov     $NDIRIN,r0
1:
	mov     r0,-(sp)
	asl     r0
	add     (sp)+,r0
	add     $addr+1,r0
	movb    (r0)+,dno
	movb    (r0)+,dno+1
	movb    -3(r0),r0
	bne     1f
	tst     dno
	beq     2f
1:
	jsr     pc,rblk
	mov     bno,r0
	inc     bno
	sub     $NDIRIN,r0
	blt     1f
	ash     $2,r0
	mov     buf+2(r0),dno
	mov     buf(r0),r0
	bne     rblk
	tst     dno
	bne     rblk
2:
	sub     $2,(sp)
1:
	rts     pc

/ ����������� ��� RL
#ifdef RL
rlcs    = 174400
rlba    = 174402
rlda    = 174404
rlmp    = 174406

READ    = 14
SEEK    = 6
RDHDR   = 10
SEEKHI  = 5
SEEKLO  = 1
CRDY    = 200
RLSECT  = 20.
#endif RL

/ ����������� ��� HP
#ifdef HP
hpcs1   = 176700
hpda    = hpcs1+6
hpcs2   = hpcs1+10
hpds    = hpcs1+12
hpof    = hpcs1+32
hpca    = hpcs1+34

read    = 70
preset  = 20
go      = 1
fmt22   = 10000
#endif

/ ����������� ��� HK
#ifdef HK
hkcs1 = 177440  / ��� 1
hkda  = 177446  / ����� �����/�������
hkcs2 = 177450  / ��� 2
hkca  = 177460  / ����� ��������

.if     rk07    / ��������� ��� RK07.
ack = 02003     /  ������ ������
clear = 040     /  ������� ����������
iocom = 2021    /  ������ + �����
.endif
.if     rk07-1  / ��������� ��� RK06.
ack = 03        /  ������ ������
clear = 040     /  ������� ����������
iocom = 021     /  ������ + �����
.endif
#endif

/ ����������� ��� RM
#ifdef RM
rmcs1   = 176700
rmda    = rmcs1+6
rmcs2   = rmcs1+10
rmds    = rmcs1+12
rmof    = rmcs1+32
rmca    = rmcs1+34
read    = 70
preset  = 20
go      = 1
fmt22   = 10000
NSECT   = 32.
.if     rm05
NTRAK   = 19.           / RM05
.endif
.if     rm05-1
NTRAK   = 5.            / RM02/03
.endif
#endif RM

/ ����������� ��� RK05
#ifdef RK
rkcs    = 177404
rkda    = 177412
rkwc    = 177406
rkba    = 177410

reset   = 0
read    = 4
go      = 1
#endif


/ ����������� ��� RP03
#ifdef RP
rpcs    = 176710
rpda    = 176724
rpca    = 176722
rpba    = 176720

read    = 4
go      = 1
#endif

/ ����������� ��� DVHP
#ifdef DVHP
hpcs1   = 176700
hpda    = hpcs1+6
hpcs2   = hpcs1+10
hpds    = hpcs1+12
hpof    = hpcs1+32
hpca    = hpcs1+34

read    = 70
preset  = 20
go      = 1
fmt22   = 10000
#endif

/ ����������� ��� DW (��������� �85)
#ifdef DW
dwsec   = 174006
dwcyl   = 174012
dwtrc   = 174014
dwdat   = 174010
dwrks2  = 174016
dwsin   = 174020
dwid    = 174000
dwerr   = 174004

HSEEK   = 20
READ    = 40
WRITE   = 60
FORMAT  = 120
#endif

/ �����-������� �����.
/ ��. ����� ������ � dno,
/ ������� � r0.
rblk:
	mov     r1,-(sp)
#if defined(MY) || defined(RL)
	mov     r2,-(sp)
#endif
#ifdef RL
	mov     r2,-(sp)
	mov     r3,-(sp)
	mov     $rlcs,r4        / ��������� �� ����������
#endif RL
	mov     dno,r1
.if     CLSIZE-1
	ashc    $CLSHFT,r0      / �������� �� CLSIZE
.endif


/------------------ RL specific
#ifdef RL
	div     $RLSECT,r0      / ����� ��������-�����������
	asl     r1              / ����� �������
	mov     $RDHDR,(r4)     / ��������� ��������� �������
7:      bit     $CRDY,(r4)      / ����� (��� ������������� �����������)
	beq     7b
	mov     *$rlmp,r2
	ash     $-7,r2
	bic     $!777,r2        / ��� �� ������ ��������
	mov     r0,r3
	asr     r3              / ����� ������� ��������
	sub     r3,r2           / ��������� ������������� �������� �������
	bge     1f              / ����� ��� ����?
	neg     r2
	ash     $7,r2
	bis     $SEEKHI,r2      / �����
	br      2f
1:      ash     $7,r2
	bis     $SEEKLO,r2      / ����
2:      mov     r0,r3           / ��������� ������ �����������
	bic     $!1,r3
	ash     $4,r3
	bis     r3,r2
	mov     r2,*$rlda       / �������� ����� ��� ������
	mov     $SEEK,(r4)      / ��������� �����
7:      bit     $CRDY,(r4)      / �����
	beq     7b
	ash     $6,r0           / �������� ����� ��� ������
	bis     r1,r0
	add     $6,r4           / ��������� �� rlmp
	mov     $WC,(r4)        / ������� ���� ��� ������
	mov     r0,-(r4)        / �������� ����� ��� ������
	mov     $buf,-(r4)      / ����� ������ � ���
	mov     $READ,-(r4)     / ������
7:      bit     $CRDY,(r4)      / �����
	beq     7b
	mov     (sp)+,r3
	mov     (sp)+,r2
	mov     (sp)+,r1
	rts     pc
#endif


/------------------ RM specific
#ifdef RM
	div     $NSECT*NTRAK,r0
.if     cyl
	add     $cyl,r0
.endif
	mov     r0,*$rmca
	clr     r0
	div     $NSECT,r0
	swab    r0
	bis     r1,r0
	mov     $rmda,r1
	mov     r0,(r1)
	mov     $buf,-(r1)
	mov     $WC,-(r1)
	mov     $read+go,-(r1)
1:
	tstb    (r1)
	bge     1b
	mov     (sp)+,r1
	rts     pc
#endif


/------------------ HP/DVHP specific
#if defined(HP) || defined(DVHP)
# ifndef DVHP
	div     $22.*19.,r0
# else
	div     $33.*19.,r0
# endif DVHP
.if     cyl
	add     $cyl,r0
.endif
	mov     r0,*$hpca
	clr     r0
# ifndef DVHP
	div     $22.,r0
# else
	div     $33.,r0
# endif DVHP
	swab    r0
	bis     r1,r0
	mov     $hpda,r1
	mov     r0,(r1)
	mov     $buf,-(r1)
	mov     $WC,-(r1)
	mov     $read+go,-(r1)
1:
	tstb    (r1)
	bge     1b
	mov     (sp)+,r1
	rts     pc
#endif

/------------------ RK specific
#ifdef RK
	div     $12.,r0
	ash     $4,r0
	bis     r1,r0
.if xautoboot
	movb    ENDCORE-BOOTDEV,r1
	ash     $13.,r1
	bis     r1,r0           / �������� ����� �����
.endif
	mov     r0,*$rkda       / �������� �������� �����.
	mov     $buf,*$rkba     / ���������� ����� ���.
	mov     $WC,*$rkwc      / ����, ���, ��� ����� �����
	mov     $read+go,*$rkcs / ... �������� ������
1:
	tstb    *$rkcs
	bge     1b
	mov     (sp)+,r1
	rts     pc
#endif

/------------------ HK specific
#ifdef HK
	div     $22.,r0         / r0 = ������ r1 = ����
	mov     r1,-(sp)
	mov     r0,r1
	clr     r0
	div     $3.,r0          / r0 = ������� r1 = ����
	bisb    r1,1(sp)
.if xautoboot
	movb    ENDCORE-BOOTDEV,r1
	ash     $-3.,r1
	mov     $100000,*$hkcs1 / clear controller
	mov     r1,*$hkcs2      / unit ---> hkcs2
.if rk07
	mov     $2005,*$hkcs1   / drive clear
.endif
.if 1-rk07
	mov     $05,*$hkcs1     / drive clear
.endif
0:      tstb    *$hkcs1         / wait for CRDY
	bpl     0b
.endif
	mov     r0,*$hkca       / ������� �������
	mov     $hkda,r1
	mov     (sp)+,(r1)      / ������ ���� � ������ -> hkda
	mov     $buf,-(r1)      / ����� ���
	mov     $WC,-(r1)       / ������� ����
	mov     $iocom,-(r1)
1:
	tstb    (r1)
	bge     1b              / ����� ���������� ��������
	mov     (sp)+,r1
	rts     pc
#endif

/------------------ RP specific
#ifdef RP
	div     $20.*10.,r0
.if     cyl
	add     $cyl,r0
.endif
	mov     r0,*$rpca
	clr     r0
	div     $10.,r0
	swab    r0
	bis     r1,r0
	mov     r0,*$rpda
	mov     $rpba,r1
	mov     $buf,(r1)
	mov     $WC,-(r1)
.if xautoboot
	movb    ENDCORE-BOOTDEV,r0
	ash     $5,r0
	bis     $read+go,r0
	mov     r0,-(r1)        /rpcs
.endif
.if 1-xautoboot
	mov     $read+go,-(r1)  /rpcs
.endif
1:
	tstb    (r1)
	bge     1b
	mov     (sp)+,r1
	rts     pc
#endif

/------------------ DW specific
#ifdef DW
	/ ������ ���� 1
	mov     r2,-(sp)
	mov     $buf, r2
	jsr     pc, 6f

.if CLSIZE-1
	/ ������ ���� 2
	inc     r1
	jsr     pc, 6f
.endif
	mov     (sp)+,r2
	mov     (sp)+,r1
	rts     pc

/ Common code for reading
6:      mov     r0, -(sp)
	mov     r1, -(sp)
7:      div     $16.*4.,r0      / � r0 - �������, � r1 - �������
.if cyl
	add     $cyl, r0
.endif
	mov     r0, *$dwcyl
	clr     r0
	div     $16.,r0         / � r0 - ����, � r1 - ������
	mov     r0, *$dwtrc
	mov     r1, *$dwsec

	mov     $READ, *$dwrks2 / Start this machine
	mov     $400, r1
3:      tstb    *$dwsin
	bpl     3b              / ����� ���������� �����
	mov     *$dwdat, (r2)+
	sob     r1, 3b          / Pseudo DMA
1:      tst     *$dwsin
	bmi     1b
	mov     (sp)+, r1
	mov     (sp)+, r0
	bit     $20400,*$dwrks2
	bne     7b              / Redo operation on I/O error
	rts     pc
#endif

/------------------ FD specific
#ifdef FD
	mov     r2, -(sp)
	mov     r3, -(sp)
	mov     r1, r3          / compute interleaving
	add     r1, r3
	add     r1, r3          / r2 - ������ * 3
	clr     r0
	div     $10., r0        / r0 - �������
	inc     r0

	/ ������ ���� 1
	mov     $buf, r1
	jsr     pc, 9f
.if CLSIZE-1
	/ ������ ���� 2
	add     $2, r3
	mov     $[buf+1000], r1
	jsr     pc, 9f
.endif
	/ ����������� �������� r2 � r3
	mov     (sp)+, r3
	mov     (sp)+, r2
	mov     (sp)+, r1
	rts     pc

9:      clr     r2
	div     $10., r2
	inc     r3                      / r3 - ���. ������
	mov     r0, *$fdtrk
	mov     r3, *$fdsec
	clr     *$fdexcom               / ������ � ��������
.if xautoboot
	movb    ENDCORE-BOOTDEV,r2
	asr     r2
	asr     r2
	bis     $EXCOM,r2
	mov     r2,*$fdcom
.endif
.if 1-xautoboot
	mov     $EXCOM,*$fdcom
.endif
	clr     *$fdstart
1:      bit     $10, *$fdstate          / ����...
	beq     1b
	clr     *$fdzero                / ���������� ��������� ������
	mov     $1000, r2
2:      movb    *$fdbuf, (r1)+
	sob     r2, 2b
	rts     pc
#endif

/------------------ RX specific
#ifdef RX
      mov     r2, -(sp)
      mov     r3, -(sp)
      mov     r4, -(sp)
      mov     r5, -(sp)
      ash     $2, r1
      mov     r1, r3          / ������� ���������� ��������
      mov     $buf, r2        / ����� ������
      jsr     pc, 1f
      jsr     pc, 1f          / ������ ������ ����
      mov     (sp)+, r5
      mov     (sp)+, r4
      mov     (sp)+, r3
      mov     (sp)+, r2
	mov     (sp)+,r1
	rts     pc
/ ������ ����������� ����� �����
1:
      mov     $4, r4
2:      clr     r0
      mov     r3, r1
      div     $26., r0        / r0 -���. ����, r1 - ���. ������
      mov     r0, r5
      mul     $6, r5
      mov     r0, -(sp)
      mov     r1, r0
      asl     r0
      cmp     r1, $13.
      bmi     3f
      inc     r0
3:      add     r5, r0
      mov     r0, r1
      clr     r0
      div     $26., r0
      inc     r1              / r1=((r1<<1)+(r1>=13)+r0*6)%26+1 - ���.���.
      mov     (sp)+, r0
      inc     r0
      cmp     r0, $76.
      bmi     4f
      sub     $77., r0         / r0=(++r0)%77
4:      jsr     pc, 5f
      inc     r3
      sob     r4, 2b
      rts     pc
/ ������ ����������� ������� r0 - ����, r1 - ������
5:
1:      bit     $40, *$rxcs
      beq     1b
      mov     $reads+go, *$rxcs
1:      bit     $200, *$rxcs
      beq     1b
      mov     r1, *$rxda
1:      bit     $200, *$rxcs
      beq     1b
      mov     r0, *$rxda
1:      bit     $40, *$rxcs
      beq     1b
.if rx02-1
      mov     $200, r5
      mov     $readb+go, *$rxcs
1:      bit     $200, *$rxcs
      beq     1b
      movb    *$rxda, (r2)+
      sob     r5, 1b
.endif
.if rx02
      mov     $readb+go, *$rxcs
1:      bit     $200, *$rxcs
      beq     1b
      mov     $100, *$rxda
1:      bit     $200, *$rxcs
     beq     1b
      mov     r2, *$rxda
      add     $200, r2
.endif
      rts     pc
#endif RX

#ifdef MY
/------------------ MY specific

/ ����������� ��� ������� ����� MY

mycs    = 172140
mydat   = 172142

go      = 1
done    = 40
reset   = 40000
read    = 0

	br      mystart
myparm: 0                       / ���� � �������, �����.����� ������ ������
	buf                     / ������� ����� ������ ������
	0                       / ������, �������
	-1.*WC                  / ������� ����

mystart:
	mov     $myparm, r2
	bic     $177774, (r2)
	div     $10., r0        / r0 - ����*2, r1 - �������
	inc     r1              / r1 - ����� �������
/ ��� 2-� ��������� �������
#ifdef DOUBLE
	ror     r0              / r0 - ����
	bcc     1f              / CARY - �������
	bis     $4, (r2)        / ������� 0 (up) 1 (down)
#endif
1:      movb    r1, myparm+4.
	movb    r0, myparm+5.
	mov     $mycs,r1
	mov     $read+go,(r1)
1:      bit     $200,(r1)
	beq     1b
	mov     r2, *$mydat
1:      bit     $done, (r1)
	beq     1b
	mov     (sp)+,r2
	mov     (sp)+,r1
	rts     pc
#endif MY


/---------------- END OF DISK-DEPENDENT PART ---------------

tks = 177560
tkb = 177562

/ ������ � �������� ������ � ���������
/ ���� *cp �� �������� 0, �� ��� - ����. ������ ��� �������� �����
/ ����� �� ���������
getc:
	movb    *cp, r0
	beq     2f
	inc     cp
#ifndef NOTTY
.if     readname
	br      putc
2:
	mov     $tks,r0
	inc     (r0)
1:
	tstb    (r0)
	bge     1b
	mov     tkb,r0
	bic     $!177,r0
	cmp     r0,$'A
	blo     2f
	cmp     r0,$'Z
	bhi     2f
	add     $'a-'A,r0
.endif

tps = 177564
tpb = 177566

2:
/ �������� ������ �� ��������
putc:
	tstb    *$tps
	bge     putc
	mov     r0,*$tpb
	cmp     r0,$'\r
	bne     1f
	mov     $'\n,r0
	br      putc
1:      rts     pc
#else NOTTY
2:      rts     pc
#endif NOTTY

cp:     defnm
defnm:  <boot\r\0>
end:

inod = ..-512.-BSIZE            / ����� ��� I-����, ������, �����
addr = inod+ADDROFF             / ������ ����� � I-����
buf  = inod+INOSIZ
bno  = buf+BSIZE
dno  = bno+2
names = dno+2

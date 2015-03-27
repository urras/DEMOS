/  ��������� ��� ��� ������ ���� ����������
/ � ������������ ���������� UCB_AUTOBOOT
/
/ 94/04/27
/ ������� ��� jbr-like ������� ����� ������ � ����������� ���������� -
/ jbr ������������ � ���������� �������. zaitcev
/
/ 94/05/13
/ ��������� �������� ��� ��������� ����������� ��3, � ������� mtpi
/ �� �������� ��-��� ����������������� ������ (mtpi, mfpi, clrseg).
/
/ 94/05/15
/ ��������������� _reloc(), ������� �� ������������ � ������� boot.
/
/ $Log:	M.s,v $
/ Revision 1.5  88/11/02  22:15:31  dvolodin
/ *** empty log message ***
/ 
/ Revision 1.4  88/03/28  11:09:29  korotaev
/ ����� ������� � AZLK.
/
/ Revision 1.3  87/09/29  15:35:17  avg
/ �������� �������� ����� � ����������������� ����� ����������������
/ �� �������� �����.
/ (KLUGE! �� �������� ���� ����������� ������� ������� � ������
/ ������������ ����������������, ������ � ����������).
/
/ Revision 1.2  86/12/13  15:39:43  alex
/ �������� clr �� mov ��� ��������� ������� ���-�.
/
/ Revision 1.1  86/07/14  20:14:05  avg
/ Initial revision
/
/
/ ������������ 11/40, 11/45, 11/70, 11/23, 11/23+����� �/� (11/24), 11/73
/ � ������ ������� ������

mfpi    = 6500^tst
stst    = 170300^tst
mtpi    = 6600^tst
mfpd    = 106500^tst
mtpd    = 106600^tst
spl     = 230
ldfps   = 170100^tst
stfps   = 170200^tst
wait    = 1
rtt     = 6
halt    = 0
reset   = 5
systrap = 104400

/  ������ �������� � ���������� ���������� � ��������� SZFLAG ������
/  ��� ����� ������� ��� ������������.
/  ������ ���� ���������� ��������� ��� � ���������;
/  ��� ���� ���� ��� ����� � ������ ���, ����� ��� �� ����� ����
/  ������������ ��� ���.
ENDCORE=        160000          / ����� ������ ��� ����������� ��
SZFLAGS=        8.              / ������ ������ ��������
BOOTOPTS=       2               / ���������� ������, ������ ����� ENDCORE
BOOTDEV=        4
CHECKWORD=      6
RBOOTDEV=       8.

.globl  _end
.globl  _main,_ubmapset
	jmp     start

/
/ ������� ����������
/
	trap;340        / ������ ����: �����!
	trap;341        / ������ �������
	trap;342        / BPT
	trap;343        / IOT
	trap;344        / ����� �������
	trap;345        / EMT
tvec:
	start;346       / TRAP
.=400^.


start:
	reset
	mov     $340,PS
	mov     $140100,sp

/ ��������� ����� �������� (���� ��� ����)
	mov     r4,_bootopts
	mov     r3,_bootdev
	mov     r2,_checkword
	mov     r1,_rbootdev
/ �������� ����� � ������
	clr     ENDCORE-BOOTOPTS
	clr     ENDCORE-BOOTDEV
	clr     ENDCORE-CHECKWORD
/
/ ���������� ��� ����������
/ ������ ����� ����������� �����. ���� ��� == 40, 24, 23, 45 ��� 70
/ ���������� ��������������� ��� ����������.
/
	clrb    _sep_id
	clrb    _ubmap
	clrb    _haveCSW
	mov     $2f,nofault     / ��������� ������� ������!
	tst     *$SWREG
	clr     nofault         / ����� ����
	incb    _haveCSW
	mov     $40.,r0
	cmp     *$SWREG,$40
	jmp     gotcha
	cmp     *$SWREG,$23
	bne     1f
	mov     $23.,r0
	jmp     gotcha
1:
	cmp     *$SWREG,$24
	bne     1f
	mov     $24.,r0
	incb    _ubmap
	jmp     gotcha
1:
	cmp     *$SWREG,$45
	bne     1f
	mov     $45.,r0
	incb    _sep_id
	jmp     gotcha
1:
	cmp     *$SWREG,$70
	bne     2f
	mov     $70.,r0
	incb    _sep_id
	incb    _ubmap
	jmp     gotcha
/
/ ���� �� ������ ������ �������� ���,
/ ���������� ���������� ��� ����������
/
2:
	mov     $40.,r0         / �� ��������� - 11/40
	mov     $2f,nofault
	mov     *$KDSA6,r1      / ���� �� ���������� ���� i/d
	incb    _sep_id
	mov     $45.,r0
	mov     $8f,nofault
	mov     *$UBMAP,r1      / ���� �� ����� ��
	incb    _ubmap
	mov     $70.,r0
	br      1f
2:
	mov     $9f,nofault
	mov     *$UBMAP,r1      / ���� �� ����� ��
	incb    _ubmap
	mov     $24.,r0         / ����� ��, ��� ����. I/D = 11/24
	br      1f
9:      clr     nofault
	mov     $1f,nofault
	mov     *$SSR3, r1      / 22�-������� ��������� ?
	mov     $23., r0        / -- ��� ����� �� � ����. I/D = 11/23
	br      1f
8:      clr     nofault
	mov     $1f,nofault
	mov     *$SSR3, r1      / 22�-������� ��������� ?
	mov     $73., r0        / -- ��� ����� ��, ���� ����. I/D = 11/73
1:      clr     nofault

gotcha:
	mov     r0,_cputype
/
/ ���������� ������� ������������� DEC Professional
/
	mov     $0f,nofault
	mov     VI_RID, r0
	cmpb    r0, $02         / ��������� �����������������
				/ ����� ���������������� (3-� �������)
	bne     0f
	inc     _video
0:
	clr nofault

/
/       ���������� �������� ���� I ���������� ������
/       � ���. ����� 0 � �� �������� �/�
/
	clr     r1
	mov     $77406, r2
	mov     $KISA0, r3
	mov     $KISD0, r4
	jsr     pc, setseg
	mov     $IO, -(r3)

/
/       ���������� �������� ���� I ������ ������������
/       � ���. ����� 128�� � �� �������� �/�
/
	mov     $6000, r1       / 06000 = 192*1024/64
	mov     $UISA0, r3
	mov     $UISD0, r4
	jsr     pc, setseg
	mov     $IO, -(r3)

/
/       ���� ��������� ���� 11/40, ����� ������ �������� ������������ I
/
	tstb    _sep_id
	beq     1f

/
/       ���������� �������� ���� D ���������� ������
/       � ���. ����� 0 � �� �������� �/�
/
	clr     r1
	mov     $KDSA0, r3
	mov     $KDSD0, r4
	jsr     pc, setseg
	mov     $IO, -(r3)

/
/       ���������� �������� ���� D ������ ������������
/       � ���. ����� 128�� � �� �������� �/�
/
	mov     $6000, r1       / 06000 = 192*1024/64
	mov     $UDSA0, r3
	mov     $UDSD0, r4
	jsr     pc, setseg
	mov     $IO, -(r3)

1:
/ �������� ��
	clrb    _ksep
	tstb    _ubmap
	beq     2f
	jsr     pc,_ubmapset
	tstb    _sep_id
	bne     3f
	mov     $60,SSR3        / 22-������� ���������, ��� ���������� I/D
	br      1f
3:
	mov     $65,SSR3        / 22-������� ���������, ����������� ���� I/D
	movb    $1,_ksep
	cmp     _cputype,$70.
	bne     1f
	mov     $3,MSCR
	br      1f
2:
	tstb    _sep_id         / ��� ����� ��; sep_id?
	beq     1f
	mov     $5,SSR3
	movb    $1,_ksep
1:
	cmp     _cputype,$23.
	bne     9f
	mov     $60, SSR3       / �������� 22x������� ���������
9:
	cmp     _cputype,$73.
	bne     1f
	bis     $20,SSR3        / �������� 22-� ������� ���������
	mov     $1,MSCR         / Disable cache parity interrupt
1:

	mov     $30340,PS
	inc     SSR0


/ ���������� ��������� � �����. ���� I
	mov     $_end,r0
	clc
	ror     r0
	clr     r1
1:
	mov     (r1),-(sp)
	mtpi    (r1)+
	sob     r0,1b


/ ���������� ������ � ���������������� ��-��
	mov     $140004,sp
	tstb    _sep_id
	bne     1f
	clr     *$KISA6
	br      2f
1:
	clr     *$KDSA6
2:      mov     $140340,-(sp)
	mov     $user,-(sp)
	rtt
user:
/ �������� bss
	mov     $_edata,r0
	mov     $_end,r1
	sub     r0,r1
	inc     r1
	clc
	ror     r1
1:
	mov     $0,(r0)+  / clr �� ����� ��������
	sob     r1,1b
	mov     $_end+512.,sp
	mov     sp,r5

	jsr     pc,_main
	mov     _cputype,r0
	mov     _bootopts,r4
	mov     r4,r2
	com     r2              / ����������� �����
	systrap

	br      user

setseg:
	mov     $8.,r0
1:
	mov     r1,(r3)+
	add     $200,r1
	mov     r2,(r4)+
	sob     r0,1b
	rts     pc

.globl  _setseg
_setseg:
	mov     2(sp),r1
	mov     r2,-(sp)
	mov     r3,-(sp)
	mov     r4,-(sp)
	mov     $77406,r2
	mov     $KISA0,r3
	mov     $KISD0,r4
	jsr     pc,setseg
	tstb    _ksep
	bne     1f
	mov     $IO,-(r3)
1:
	mov     (sp)+,r4
	mov     (sp)+,r3
	mov     (sp)+,r2
	rts     pc

.globl  _setnosep
_setnosep:
	bic     $4,SSR3 / ��������� ���������� ����� i/d ����
	clrb    _ksep
	rts pc

.globl  _setsep
_setsep:
	bis     $4,SSR3 / �������� ���������� ����� i/d ���� (���� ���������)
	movb    $1,_ksep
	rts pc

/P3: ����� �������, �� reloc() �� ����������� � ����������.
/����������� ��� �� ����� ��������.
/
//  ������������� �  ����� ������� ������ ��� -- ���� ���� �������.
/.globl  _reloc, _segflag
/_reloc:
/	jsr     r5, csv
/	mov     $2f, nofault
/	/  ���������� �������� ���� I ���� �� ����� �������
/	mov     $6000, r1                       / 192 ��
/	mov     $77406,r2
/	mov     $KISA0, r3
/	mov     $KISD0, r4
/	jsr     pc, setseg
/	/  ������������ � ����� �������
/	clr     r0
/	mov     $28.*1024., r1                  / 28� ����
/1:
/	mov     (r0), -(sp)
/	mtpi    (r0)+
/	sob     r1, 1b
/	clr     nofault
/
/	/  �������������� �������� ���� I ���� � 0.
/	clr     -(sp)
/	jsr     pc, _setseg
/	tst     (sp)+
/
/	/  ���������� ���������������� �������� ���� D �� ����� �������
/	tstb    _sep_id
/	beq     1f
/	mov     $6000, r1                       / 192 ��
/	mov     $UDSA0, r3
/	mov     $UDSD0, r4
/	jsr     pc, setseg
/	mov     $IO, -(r3)
/
/	/  ���������������� �������� ���� I --
/	/   ���������� ������������ ��� �� ����� �����.
/1:
/	mov     $6000, r1                       / 192 ��
/	mov     $UISA0, r3
/	mov     $UISD0, r4
/	mov     $7,r0
/1:
/	mov     r1,(r3)+
/	add     $200,r1
/	mov     r2,(r4)+
/	sob     r0,1b
/
/	mov     $3, _segflag                    / ����� ���� ���������� ������
/	clr     r0                              / ... �����
/	jmp     cret
/
/	/  ���������� �� ������ ���� (������������ ������)
/2:
/	mov     $-1, r0
/	jmp     cret

/P3: �������� �� ��������� ������ �� C, ���������� ����� mtpi().
/��� �����, ��� ������������, � �� ��������� �� ��������.
// clrseg(addr,count)
/.globl  _clrseg
/_clrseg:
/	mov     4(sp),r0
/	beq     2f
/	asr     r0
/	bic     $!77777,r0
/	mov     2(sp),r1
/1:
/	clr     -(sp)
/	mtpi    (r1)+
/	sob     r0,1b
/2:
/	rts     pc


/ mtpd(word,addr)
.globl  _mtpd
_mtpd:
	cmp     _cputype,$23.		/ ���, ������
	beq	mtpi_23
	mov     4(sp),r0
	mov     2(sp),-(sp)
	mtpd    (r0)+
	rts     pc

/ mtpi(word,addr)
.globl  _mtpi
_mtpi:
	cmp     _cputype,$23.
	beq	mtpi_23
	mov     4(sp),r0
	mov     2(sp),-(sp)
	mtpi    (r0)+
	rts     pc

/P3: ��� mtpi(), ������� �� �������� mtpi.
/� ��������� ���� �������� ������: ������������ �����+2. �� ����, �����
/�� ��� ����-������. �� ������ ������ ����������. ��������������, ���
/�� ��������� ���-�� ����� ��������� ������������ ��������� � �����
/������� ������� #6.
mtpi_23:
	mov	4(sp),r0	/ addr
	mov	2(sp),r1	/ value
	mov	r2,-(sp)
	mov	r3,-(sp)

	mov	r0,r2		/ ��������� ������ �������� ����������
	ash     $-12.,r2
	bic     $177761,r2

	mov	*$UISA6,r3	/ ������� � ������������� ������� #6
	mov	KISA0(r2),*$UISA6

	mov	r0,r2		/ ������� �������� � ���� #6
	bic     $160000,r2
	bis	$140000,r2

	mov	r1,*r2

	mov	r3,*$UISA6	/ ��������������� ���� #6

	mov	(sp)+,r3
	mov	(sp)+,r2
	add	$2,r0
	rts	pc

/ unsigned mfpi(addr)
.globl  _mfpi
_mfpi:
	cmp	_cputype,$23.
	beq	mfpi_23
	mov     2(sp),r0
	mfpi    (r0)
	mov (sp)+,r0
	rts     pc

mfpi_23:
	mov	2(sp),r0
	mov	r2,-(sp)
	mov	r0,r2
	ash     $-12.,r2
	bic     $177761,r2
	mov	*$UISA6,r1
	mov	KISA0(r2),*$UISA6
	bic     $160000,r0
	bis	$140000,r0
	mov	*r0,r0
	mov	r1,*$UISA6
	mov	(sp)+,r2
	rts	pc

.globl  __rtt
__rtt:
	halt

.globl  _trap

trap:
	mov     *$PS,-(sp)
	mov     r0,-(sp)
	mov     r1,-(sp)
	tst     nofault
	bne     3f
	jsr     pc,_trap
	mov     (sp)+,r1
	mov     (sp)+,r0
	tst     (sp)+
	rtt
3:      mov     (sp)+,r1
	mov     (sp)+,r0
	tst     (sp)+
	mov     nofault,(sp)
	rtt

.globl  _autoboot
_autoboot:
	mov     r1,-(sp)
	clr     r0
	mov     $60.,r1
1:
	sob     r0,1b
	sob     r1,1b
	cmpb    $3,*$177562
	beq     1f
	inc     r0
1:
	mov     (sp)+,r1
	rts     pc

PS      = 177776
SSR0    = 177572
SSR1    = 177574
SSR2    = 177576
SSR3    = 172516
KISA0   = 172340
KISA1   = 172342
KISA6   = 172354
KISA7   = 172356
KISD0   = 172300
KISD7   = 172316
KDSA0   = 172360
KDSA6   = 172374
KDSA7   = 172376
KDSD0   = 172320
KDSD5   = 172332
SISA0   = 172240
SISA1   = 172242
SISD0   = 172200
SISD1   = 172202
UISA0   = 177640
UISA6	= 177654
UISD0   = 177600
UDSA0   = 177660
UDSD0   = 177620
MSCR    = 017777746     / ������� ���������� ������� 11/70
IO      = 177600
SWREG   = 177570
UBMAP   = 170200
VI_RID  = 174400        / ������� ������������� ����������������
			/  --- 3 ������� ����. ���������� ---

.data
.globl  _cputype
.globl  _ksep, _sep_id, _ubmap, _haveCSW, _video
.globl  _bootopts, _bootdev, _checkword, _rbootdev

nofault:        .=.+2   / ����������� ������� ����������
_cputype:       .=.+2   / ��� ���������� (40, 45, 23, 24 ��� 70)
_sep_id:        .=.+1   / 1 ���� ���� ���������� ���� I � D
_ubmap:         .=.+1   / 1 ���� ���� ����� ��
_haveCSW:       .=.+1   / 1 ���� ���� ������� ���������� ��������������
_ksep:          .=.+1   / 1 ���� � ���� �������� ���������� ����� I/D
_bootopts:      .=.+2   / ����� ��� ������������
_bootdev:       .=.+2   / �-�� ��� �������� �������, ���� ��� RB_ASKNAME
_checkword:     .=.+2   / ����. r2, ���������� bootopts ��� ������������
_video:         .=.+2   / �������, ���� ������ ����������� ���������
			/ ������������ ������������ DEC Professional
_rbootdev:      .=.+2   / Real boot device

/
/ $Log:	pcopy.s,v $
/ Revision 1.3  87/03/03  22:29:44  avg
/ ������������� ���������� ������ ��� �����������-85.
/ 
/ Revision 1.2  86/11/22  22:12:01  alex
/ ???
/
/ Revision 1.1  86/09/01  21:45:44  avg
/ Initial revision
/
/
/
/ ����������� ����� ������ �� ���. ��������� ��-�� � ������
/
/ ��������� C:
/
/       cop_to(   haddr, laddr, wcnt, myaddr )      <haddr,laddr> <== myaddr
/       cop_from( haddr, laddr, wcnt, myaddr )      myaddr <== <haddr,laddr>
/
/ Attention !!! It works only if psw accessable by mov.
/
/ P3: Avg �� �����������-85 ����� ���:
/    1. ������ $140000 � PSW.
/    2. ���� $KISA0 � ������ �����.
/    3. mtpi
/    4. �������������� PSW, KISA0.
/ ������� � �������, �� �� ��� �� ��������. ������������ �� ����� ���
/ ������ mtpi � ������������ ����. ���������� ������������ ����������.
/
/ Should check _sepid in a future.

.globl  _cop_to, _cop_from, csv, cret

	.text

_cop_to:
	jsr     r5,csv

	jsr	pc,getseg
	/ ������ � r0 ����� ������ ���������� ��������

	/ ��������� �������� ���������
	mov	r0,r1
	mov	r0,r2
	add	$UISD0,r1
	add	$UISA0,r2
	mov     *r2, uisa
	mov     *r1, uisd
	mov     *$PSW, psw

	/ ���������� ��������
	mov     $140340, *$PSW
	mov     $77406, *r1
	mov     4(r5),r4	/ haddr
	ash     $12,r4
	bic     $1777,r4
	mov     6(r5),r1	/ laddr
	ash     $-6,r1
	bic     $176000,r1
	bis     r1,r4
	mov     r4,*r2
	mov     6(r5),r2	/ laddr
	bic     $177700,r2      / addr of user I
	mov	r0,r1
	ash	$14,r1
	add	r1,r2

	/ �������� ....
	mov     10(r5),r4       / wcnt
	mov     12(r5),r3       / myaddr
	br      1f
2:
	mov     (r3)+, (r2)+

1:      mov     r4,r1
	dec     r4
	tst     r1
	bne     2b

	/ ��������������� ���������
	mov	uisa, UISA0(r0)
	mov     uisd, UISD0(r0)
	mov     psw, *$PSW
	jmp     cret

_cop_from:
	jsr     r5,csv

	jsr	pc,getseg
	/ ������ � r0 ����� ������ ���������� ��������

	/ ��������� �������� ���������
	mov	r0,r1
	mov	r0,r2
	add	$UISD0,r1
	add	$UISA0,r2
	mov     *r2, uisa
	mov     *r1, uisd
	mov     *$PSW, psw

	/ ���������� ��������
	mov     $140340, *$PSW
	mov     $77406, *r1
	mov     4(r5),r4
	ash     $12,r4
	bic     $1777,r4
	mov     6(r5),r1
	ash     $-6,r1
	bic     $176000,r1
	bis     r1,r4
	mov     r4,*r2
	mov     6(r5),r2
	bic     $177700,r2      / addr of user I
	mov	r0,r1
	ash	$14,r1
	add	r1,r2

	/ �������� ....
	mov     10(r5),r4       / wcnt
	mov     12(r5),r3       / myaddr
	br      1f
2:
	mov     (r2)+, (r3)+
1:
        mov     r4,r1
	dec     r4
	tst     r1
	bne     2b

	/ ��������������� ���������
	mov     uisa, UISA0(r0)
	mov     uisd, UISD0(r0)
	mov     psw, *$PSW
	jmp     cret

/ ���� ��������� �������
/ �� ������ 12(r5), sp, pc
/ � ���� �������� �������.
getseg:

	/ ������ �����
	mov	$7,r0	        / ������� ������� �����-������
1:	movb	$1,map-1(r0)
	sob	r0,1b

	/ ����� ���, ������, � ����
	mov	pc,r0
	jsr	pc,mapaddr
	mov     12(r5),r0       / myaddr
	jsr	pc,mapaddr
	mov	sp,r0
	jsr	pc,mapaddr

	/ ���� ������������ ������� (��� ���������� ������)
	mov	$map,r0
3:
	tstb	(r0)+
	bne	2f
	cmp	r0,$map+7
	bhis	.-2		/ Never happens, just for sure
	br	3b
2:
	sub	$map+1,r0
	asl	r0
	rts	pc

/ �������� ���� ���������
/ (��� �����, ��� ��������� ������������)
mapaddr:
	ash	$-15,r0
	bic	$177770,r0
	add	$map,r0
	clrb	(r0)+
	clrb	(r0)+
	rts	pc

/ � ������ �������� ������, ��� ��� ����������� ����� ���������.
/ ���� ������� � ����, �� ���-�� ������ ��������� ��� �����.
/ ��� ������ � sepid ����� ������ �� ������ ����. --P3.
/	.data

map:	.byte	0,0,0,0,0,0,0,0
map_1=map-1

uisa:   .=.+2
uisd:   .=.+2
psw:    .=.+2

PSW=177776
UISA0=177640
UISD0=177600

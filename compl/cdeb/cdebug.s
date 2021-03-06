/ === CDEBUGGER === �����, ���������� ����� ������ ���������� C-���������
/
/ � C-��������� ����������� ���������:   jsr pc,*$cdebug; (�����_������)

.globl  cdebug
.globl  cdebflag

          .bss  / ������������ ������ � ���� ����� �� ������ !!!
                / ��� �������� ���������, ������� ���������� �� 'cdebflag'

sr0:      .=.+2         / ������ ��� �������� ����������� r0 - r4
sr1:      .=.+2
sr2:      .=.+2
sr3:      .=.+2
sr4:      .=.+2
sr5:      .=.+2         / ��������� �����
ssp:      .=.+2         / ��������� �����
spc:      .=.+2         / pc � ����� �������� �� 'cdebug'
cdebflag: .=.+2         / ����: ����� �� �������� ���������
pframe:   .=.+2         / ����� ����� ������� ������� ��� 0 (��� ��������,
                        /       ����������� �� �� � ��������)
stpadr:   .=.+2         / 0 ��� ����� �������� (�� pc)
from:     .=.+2         / 0 - 6 ��� ����� ������ ������������ �������
                        /       0 - �� ���������
                        /       1 - ���������, ��� �����  'leng' = 'value'
                        /       2 - ���������, ��� ����   'leng' = 'value'
                        /       3 - ���������, ��� �������'leng' = 'value'
                        /       4 - ���������, ��� �����  'leng' ����������
                        /       5 - ���������, ��� ����   'leng' ���������
                        /       6 - ���������, ��� �������'leng' ���������
leng:     .=.+2         / ����� ������������ ������� ��� ����� ������
value:    .=.+2         / ��������� �� ��� �������� ��� �� ��� �����. �����
          .text

cdebug:
        mov     r0,sr0          / ������� �������� r0 - r5
        mov     $sr0+2,r0
        mov     r1,(r0)+
        mov     r2,(r0)+
        mov     r3,(r0)+
        mov     r4,(r0)+
        mov     r5,(r0)+

        mov     sp,(r0)+        / ������� ��������� ����� � 'cdebug'
        add     $2,(sp)         / � ����� �������� � ��������� ���������
        mov     (sp),(r0)+

        tst     (r0)+       ;  beq  no_stop     / ��������� 'cdebug' �� 0
        cmp     r5,(r0)+    ;  blo  no_stop     / ��������� r5 < pframe

        tst     (r0)+       ;  beq  1f          / ��������� stp_adr != 0
          cmp   -(r0),(sp)                      / ��������� adr = stp_adr
          beq   stopor
          br    no_stop
     1:

        bit     (r0),$!7    ;  bne  ch_zone     / ��������� ������� - ��
        mov     (r0),r0                         /    ������ ���� ��������
        asl     r0
        jmp     *tabjmp(r0)
        .data
tabjmp: stopor; eq_word; eq_byte; eq_reg; ne_word; ne_byte; ne_reg
        .text


stopor: mov     sr0,r0          / �������� ��������
        bpt

no_stop:                        / ����� � �������� ���������
        mov     sr0,r0
        rts     pc


eq_word:                         / ���������, ��� ����� 'leng' = 'value'
        cmp     *leng,value ; beq stopor ; br no_stop

eq_byte:                         / ���������, ��� ���� 'leng' = 'value'
        cmpb    *leng,value ; beq stopor ; br no_stop

eq_reg:                          / ���������, ��� ������� 'leng/2' = value
        mov     leng,r0 ;
        asl     r0
        jmp     *tabreq(r0)
        .data
tabreq: eqr0; eqr1; eqr2; eqr3; eqr4; eqr5; eqr6; eqr7
        .text
eqr0:   cmp     sr0,value   ; beq stopor ; br no_stop
eqr1:   cmp     r1,value    ; beq stopor ; br no_stop
eqr2:   cmp     r2,value    ; beq stopor ; br no_stop
eqr3:   cmp     r3,value    ; beq stopor ; br no_stop
eqr4:   cmp     r4,value    ; beq stopor ; br no_stop
eqr5:   cmp     r5,value    ; beq stopor ; br no_stop
eqr6:   cmp     r6,value    ; beq stopor ; br no_stop
eqr7:   cmp     r7,value    ; beq stopor ; br no_stop

ne_word:                         / ���������, ��� ����� 'leng' ����������
        cmp     *leng,value ; bne stopor ; br no_stop

ne_byte:                         / ���������, ��� ���� 'leng' ���������
        cmpb    *leng,value ; bne stopor ; br no_stop

ne_reg:                          / ���������, ��� ������� 'leng/2'���������
        mov     leng,r0 ;
        asl     r0
        jmp     *tabrne(r0)
        .data
tabrne: ner0; ner1; ner2; ner3; ner4; ner5; ner6; ner7
        .text
ner0:   cmp     sr0,value   ; bne stopor ; br no_stop
ner1:   cmp     r1,value    ; bne stopor ; br no_stop
ner2:   cmp     r2,value    ; bne stopor ; br no_stop
ner3:   cmp     r3,value    ; bne stopor ; br no_stop
ner4:   cmp     r4,value    ; bne stopor ; br no_stop
ner5:   cmp     r5,value    ; bne stopor ; br no_stop
ner6:   cmp     r6,value    ; bne stopor ; br no_stop
ner7:   cmp     r7,value    ; bne stopor ; br no_stop

ch_zone:                        / ���������,��� �����.����� ���� ����������
        .bss
work:   .=.+2
        .text
        mov     from,r2
        mov     leng,r1
        add     r2,r1
        clr     r0                      / � r0 - ����������� �����
        bit     $1,r2                           / ���� 1-�� �������� ����
        beq     1f
          movb  (r2)+,r0
     1: bit     $1,r1                           / ���� ����. �������� ����
        beq     2f
          movb  -(r1),work
          add   work,r0
     2: cmp     r2,r1                           / ������� ����� �� ������
        bhis    3f
          add   (r2)+,r0
          br    2b
     3: mov     sr1,r1                          / ��������������� ���. ���.
        mov     sr2,r2
        cmp     r0,value                        / �������� �����. �����
        jeq     no_stop
        jmp     stopor

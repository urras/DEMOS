.th ���������� \*(RU 3/23/79
.s3
.i0
.in +4
.ti -4
EQUEL(QUEL) \- ������� �������� ����� EQUEL
.br
.s3
.i0
.in +4
.ti -4
MACROS(QUEL) \- �������� ����� ��������������� ��������
.s3
.i0
.in +4
.ti -4
QUEL(QUEL) \- ������� �������� ����� QUEL
.s3
.i0
.in +4
.ti -4
APPEND(QUEL) \- �������� ������� � ���������
.br
.bd append
[\c
.bd to\c
] ���_��� (������_��������) [\c
.bd where
�������\c
]
.s3
.i0
.in +4
.ti -4
COPY(QUEL) \- ����������� ������ �/�� ��������� ��/� ���� \*(UU.
.br
.bd copy
���_���
(���_������ = ������ {, ���_������ = ������ })
.br
.ti +10
����������� "��� �����"
.s3
.i0
.in +4
.ti -4
CREATE(QUEL) \- ������� ����� ���������
.br
.bd create
���_��� (���_������\c
.it 1
= ������ {, ���_������\c
.it 2
= ������ })
.s3
.i0
.in +4
.ti -4
DEFINE(QUEL) \- ���������� ��������
.br
.in +5
.ti -5
.bd define
.bd view
��� (������_��������) [
.bd where
������� ]
.ti -5
.bd "define permit"
������_��������
{
.bd on
|
.bd of
|
.bd to
}
����������
[ (������) ]
.bd to
���
[
.bd at
�������� ]
[
.bd from
�����
.bd to
����� ]
[
.bd on
����_���
.bd to
����_��� ] [
.bd where
������� ]
.ti -5
.bd define
.bd integrity
.bd on
����������
.bd is
�������
.i0
.s3
.i0
.in +4
.ti -4
DELETE(QUEL) \- ������� ������� �� ���������
.br
.bd delete
������_����������
[\c
.bd where
�������\c
]
.s3
.i0
.in +4
.ti -4
DESTROY(QUEL) \- ���������� ������������ ���������(�)
.br
.bd destroy
���_��� {, ���_���}
.br
.bd destroy
[\c
.bd permit
|
.bd integrity\c
] ���_���
[\c
integer {, integer}\c
|
.bd all\c
]
.s3
.i0
.in +4
.ti -4
HELP(QUEL) \- �������� ���������� � ���, ��� ������������ \*(II ��� �� ���������� � ���� ������.
.br
.bd help
[\c
���_���\c
] [\c
"���_������"\c
]
{, ���_���}{, "���_������"}
.br
.bd "help view"
���_��� {, ���_���}
.br
.bd "help permit"
���_��� {, ���_���}
.br
.bd "help integrity"
���_��� {, ���_���}
.s3
.i0
.in +4
.ti -4
INDEX(QUEL) \- ������� ��������� ������ �� ������������ ���������.
.br
.bd "index on"
���_���
.bd is
���_�������
(�����\c
.it 1
{ ,�����\c
.it 2\c
})
.s3
.i0
.in +4
.ti -4
INTEGRITY(QUEL) \- ���������� ����������� �����������.
.br
.bd "define integrity on"
����������
.bd is
�������
.s3
.i0
.in +4
.ti -4
MACROS(QUEL) \- ���������������� ������������� ��������.
.br
.s3
.i0
.in +4
.ti -4
MODIFY(QUEL) \- ������������� ��������� ������ ���������.
.br
.in +5
.ti -5
.bd modify
���_���
.bd to
���������_������
[
.bd on
����1 [ :
.it �������_����
] [ { , ����2 [ :
.it �������_����
] } ] ]
[
.bd where
[
.bd "fillfactor ="
.it n
] [
.bd ", minpages ="
.it n
] [
.bd ", maxpages ="
.it n
] ]
.i0
.s3
.i0
.in +4
.ti -4
MONITOR(QUEL) \- ������������� ������������ �������.
.br
.s3
.i0
.in +4
.ti -4
PERMIT(QUEL) \- �������� ���������� � ���������.
.br
.bd define
.bd permit
������_�������� {
.bd on
|
.bd of
|
.bd to
} ���������� [ (������) ]
.br
.if n .ti +5
.if t .ti +0.5i
.bd to
��� [
.bd at
�������� ]
[
.bd from
�����
.bd to
����� ]
.br
.if n .ti +5
.if t .ti +0.5i
[
.bd on
����_���
.bd to
����_��� ] [
.bd where
������� ]
.s3
.i0
.in +4
.ti -4
PRINT(QUEL) \- �������� ���������(�).
.br
.bd print
���_��� {, ���_���}
.s3
.i0
.in +4
.ti -4
RANGE(QUEL) \- ��'����� ���������� ��� ��������������� �� ���������.
.br
.bd "range of"
����������
.bd is
���_���
.s3
.i0
.in +4
.ti -4
REPLACE(QUEL) \- �������� �������� ������� � ���������.
.br
.bd replace
������_���������� (������_��������)
[\c
.bd where
�������]
.s3
.i0
.in +4
.ti -4
RETRIEVE(QUEL) \- ������ ������� �� ���������.
.br
.bd retrieve
[[\c
.bd into\c
] ���_���] (������_��������)
[\c
.bd where
�������]
.br
.bd "retrieve unique"
(������_��������) [\c
.bd where
�������]
.s3
.i0
.in +4
.ti -4
SAVE(QUEL) \- ������� ��������� ������ �� �������� ����.
.br
.bd save
���_���
.bd until
����� ���� ���
.s3
.i0
.in +4
.ti -4
VIEW(QUEL) \- ���������� ����������� ���������.
.br
.bd define
.bd view
��� (������_��������) [
.bd where
������� ]
.s3
.i0
.in +4
.ti -4
COPYDB(\*(DM) \- �������� ������-�������, ���������� � �����������������
���� ������.
.br
.bd copydb
[
.bd \-u\c
.it ���
[ ����_������ ������_���_��������
[ ��������� ... ]
.s3
.i0
.in +4
.ti -4
CREATDB(\*(DM) \- ������� ���� ������.
.br
.bd creatdb
[
.bd \-u\c
.it ���
] [
.bd \-e
] [
.bd \-m
] [
.bd \*(+-c
] [
.bd \*(+-q
] ���_��
.s3
.i0
.in +4
.ti -4
DESTROYDB(\*(DM) \- ���������� ������������ ���� ������.
.br
.bd destroydb
[
.bd \-s
] [
.bd \-m
] ���_��
.s3
.i0
.in +4
.ti -4
EQUEL(\*(DM) \- ���������� � ���� C ��������� � QUEL.
.br
.bd equel
[
.bd \-d
] [
.bd \-f
] [
.bd \-r
] file.q ...
.s3
.i0
.in +4
.ti -4
HELPR(\*(DM) \- �������� ���������� � ���� ������.
.br
.bd helpr
[
.bd \-u\c
.it ���
] [
.bd \*(+-w
] ����_����� ��������� ...
.s3
.i0
.in +4
.ti -4
\*(RU(\*(DM) \- ������� ���������� ����������� ����� ������ \*(II.
.br
.bd \*(rN
[
.it �����
] ���_�� [ �������_��������� ]
.s3
.i0
.in +4
.ti -4
PRINTR(\*(DM) \- �������� ���������.
.br
.bd printr
[
.it �����
] ����_������ ��������� ...
.s3
.i0
.in +4
.ti -4
PURGE(\*(DM) \- ���������� ��� ������������� � ��������� ���������.
.br
.bd purge
[
.bd \-f
] [
.bd \-p
] [
.bd \-a
] [
.bd \-s
] [
.bd \*(+-w
] [ ����_������ ... ]
.s3
.i0
.in +4
.ti -4
RESTORE(\*(DM) \- �������������� ����� ������  \*(II ��� \*(UU .
.br
.bd restore
[
.bd \-a
] [
.bd \-s
] [
.bd \*(+-w
] [ ����_������ ... ]
.s3
.i0
.in +4
.ti -4
SYSMOD(\*(DM) \- �������� ��������� ��������� � ���������������� ���������� ������.
.br
.bd sysmod
[
.bd \-s
] [
.bd \-w
] ���_��
[
.bd relation
] [
.bd attribute
] [
.bd indexes
] [
.bd tree
] [
.bd protect
] [
.bd integrities
]
.s3
.i0
.in +4
.ti -4
USERSETUP(\*(DM) \- ������� ���� ������������� "users".
.br
.bd \&.../bin/usersetup
[ ������_���_����� ]
.s3
.i0
.in +4
.ti -4
DAYFILE(FILES) \- ��������������� ��������� \*(II.
.br
.s3
.i0
.in +4
.ti -4
DBTMPLT(FILES) \- ������ ���� ������.
.br
.s3
.i0
.in +4
.ti -4
ERROR(FILES) \- ����� � �������� \*(II.
.br
.s3
.i0
.in +4
.ti -4
LIBQ(FILES) \- ���������� ������� EQUEL.
.br
.s3
.i0
.in +4
.ti -4
PROCTAB(FILES) \- ���������� � ������������ \*(II �� ����� ������.
.br
.s3
.i0
.in +4
.ti -4
STARTUP(FILES) \- ���� ������������� \*(II.
.br
.s3
.i0
.in +4
.ti -4
USERS(FILES) \- ���� � ��������� ������������ \*(II.
.br
.s3
.i0
.in +4
.ti -4
INTRODUCTION(ERROR) \- �������� �� ���������� �� �������.
.br
.s3
.i0
.in +4
.ti -4
EQUEL(ERROR) \- ������ ��������� �� ������� \s-2EQUEL\s0.
.br
������ ������ 1000 \- 1999.
.s3
.i0
.in +4
.ti -4
PARSER(ERROR) \- ������ ��������� �� ������� ������������ �����������.
.br
������ ������ 2000 \- 2999.
.s3
.i0
.in +4
.ti -4
QRYMOD(ERROR) \- ������ ��������� �� ������� ����������� ��������.
.br
������ ������ 3000 \- 3999.
.s3
.i0
.in +4
.ti -4
OVQP(ERROR) \- ������ ��������� �� ������� ���������� �������� � ����� ����������.
.br
������ ������ 4000 \- 4499.
.s3
.i0
.in +4
.ti -4
DECOMP(ERROR) \- ������ ��������� �� ������� ������������.
.br
������ ������ 4500 \- 4999.
.s3
.i0
.in +4
.ti -4
DBU(ERROR) \- ������ ��������� �� ������� ������������� �������� ���� ������.
.br
������ ������ 5000 \- 5999

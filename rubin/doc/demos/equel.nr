.th EQUEL \*(DM "���� \*(RU. ������� \*(DM"
.if n .ds EE EQUEL
.if t .ds EE \s-2EQUEL\s0
.sh ���
equel \- ���������� � ���� C ��������� � QUEL
.sh ������
 \fBequel \fR[\fB\-d\fR] [\fB\-f\fR] [\fB\-r\fR] \fIfile\fB.q\fI ...\fR
.sh ��������
.bd Equel
������������� ������������ ����������� ����������
�������������� ����� ���������������� ``\fBC\fR''
� \*(II,
��� ���� ������������
������������
� ����������
\*(EE.
.s1
.bd ����������
.s2
����� ����� ������ �������������:
.s3
.ti +5
.bd equel
[\fI<�����>\fR] \fI����1\fB.q\fR [\fI<�����>\fR] \fI����2\fB.q\fR \fI...\fR
.s3
��� ``\fI����n\fB.q''
��� ����� �������� ������,
������� ������ �������������
.bd \&.q.
�������� ���������� ���������� � ����
``\fIfilen\fB.c\fR''.
��� ������ ����� �������� ����� ����������� ����� ������.
.br
������� ��������� �����:
.de xp
.s3
.lp +5 5
.if n \fB\\$1\fR\t\c
.if t \fB\\$1\fP\t\c
..
.xp \-d
������������ ��������� � ������� ����� ��������� �����
� ������� ������ � ������� �������� ������ �� �����
���������� ���������. ��� ����� ��������� �������� ��� �������,
�� ������� �������������� ������ ��������.
��������� ���������.
.xp \-f
���������� ������������ C-��������� � �������� �����, � ��� �� �������, ��� �
�� ������� �����. ��� �������� ���������������� ���������������
��������� ����� C.
\*(EE
�����, ��� ������� �������� ���������� ��� ������ � C-�����������
� �������� ����� �� ��� �� �������, �� ������� ��� ����������
�� ������� �����.
������, �� ���������
������������ ����� � \fBC\fR-�������������,
������ ����� ���� ������ �� ��������� �����;
�������� ����������� ��������� \fBC\fR-���������� ������
�� ��������� �����.
���� ����� ����
.bd \-f
\- ����� ������� �� ����������, ���� ����� ����� ����� �������������,
������ � \fBC\fR-����������� ����� ���������� �� ������� ������.
��� ������� ��� ���������� ������ �
�������� �����, �� ������� ���������
��������������� ��������� \fBC\fR-�����������
� ��������������� �������� �����.
.xp \-r
���������� �������� ������, ������������ ��������,
����������� ��� �� ���������.
������������ ��� ��������� �������� ������ ������
�� ��������� �����, ��������� � ������ ����������.
.i0
.dt
.s3
����� ���������������
\*(EE
����� ������������� C-������������:
.s3
.ti +5
.bd cc
\fIfile1\fB.c\fR \fIfile2\fB.c\fR ...
.bd \-lq
.s3
.bd \-lq
���������� �� ������������� �������������
��'������ ����������
\*(EE.
.s3
��� ���������� �����, ����������� ���������
\*(EE
���������� � �������� "\fBII\fR"
� ������� ��� ���������� ���������� � ����� �������� ����
\fBII\fIxxx\fR
������������� ��� �������������
\*(EE
� �� �� ������� ��������� �������������
\*(EE.
.s1
.sh "��.�����"
 "\fB���������������� �� ����� \*(EE\fR", � ������������ �� \*(Ru,
 "\fB���������� ����������� �� ����� C\fR", � ������������ �� �� \*(DM,
 Equel(quel), � ������ �����������,
 Quel(QUEL),  � ������ �����������.
.sh ������
.s3
��������� EQUEL-����� ���������� \fB#include\fR � �������������������
������� ������ ��� � ������ � ��������� ������� ���������� \fBC\fR
������������ �����������.
.sh �������
������� �������� �� ����� \*(EE ��������� � ����������� Examples:
equeldemo.q - ������� ������, equeltut.q - ����������� ������, ����������
��������� �����������.

.th MONITOR QUEL "���� \*(RU. ��������� QUEL"
.sh ���
monitor \- ������������� ������������ �������
.sh ��������
������������� ������������ �������
�������� �������� ��������� ��������������
� \*(II.
�� ������������ �����������
������������� ������ �
������������� ��� ����� ���������
�� ��������� \*(II.
���� ���������� ������ �����-���� ���������, ��� �������������� ������ �������
����� ��������������� ����� �� ��������� ����������
\*(UU.
.s1
.bd "��������� � ���������."
.s2
������������ ������� ����������� ������������
� ��������� �������� �
������ ��������.
.s3
��� ����������� ������������
����������  ��������������� ���������.
� ��� ������ ���������� ����� ������ �������
� ����� �����������.
�����, �� \fBdayfile\fR
�������� ����������, ��������������
��� �������������.
.s3
����� \*(II ������ � ������,
���������� ���������
``\fIgo\fR''.
��� ��������, ��� ����� ��������
����.
���������
``\fIcontinue\fR''
��������, ��� � ������ �������� �������
����������.
����� ������ ������� \fB\ego\fR,
���� ���������� ���� ������ �������,
���������� ������ ��������
������������� ���������,
�� ����������� ���� ������, ����� ������ ����������
� ����� ��������
�������� �������, ����������� �� ����� �������.
����� ��������� ��������:
\fB\eappend\fR,
\fB\eedit\fR,
\fB\eprint\fR,
\fB\elist\fR,
\fB\eeval\fR,
�
\fB\ego\fR.
��������, ���� ����������
.br
	help parts
.br
	\ego
.br
	print parts
.br
�� � ������ �������� ����� ����������
.br
print parts
.br
����� ���, ���� ����������
.br
	help parts
.br
	\ego
.br
	\eprint
.br
	print parts
.br
����� �������� ����� ���������
.br
	help parts
.br
	print parts
.br
����� ������� ����
����� �������,
� ������ ������ ������
���������� ���������.
.s1
.bd �������
.s2
���������� ��� ������, �������
������������ ����� ������������
��� ���������� ������� �������� ��� ����� ����������.
��� ��� ������� ������ ����������
� ������� �������� ����� �����
(`\fB\e\fR'),
� ��� ��� ����������� ����������,
(� �� �� ����� ����������, ��� �������� �������).
.s3
��������� ������� ������� ������� ����� �����,
������� ������������ ��� ������ �������� ������
����� ����� ����� �������
� �� ����� �������� ������.
�������� �� ����� ������ ������ � ����� ��������� ������ ������� ������.
�������, ������� �� ����� �������� ����� �����,
����� ������������� ������ �� ����� ������;
��������
.br
	\edate\ego\edate
.br
������ ����� �� � �����
���������� �������� ������ �������.
.de xx
.lp +15 15
.ti -10
.ta 10
\\fB\\$1\\fR\t\c
..
.s3
.xx \er
.xx \ereset
������� �������� ������ (������� ����� ��������).
������� ���������� ������ ��������.
.s3
.xx \ep
.xx \eprint
���������� ������� ������.
���������� ������ �������� �� �������� ������������.
.s3
.xx \el
.xx \elist
���������� ������� ������, � ����� ����, ����� �� ������ �����
��������������.
��������� ��� ������� ��������������, ��������,
��� ������������ ����� ����������.
.s3
.xx \eeval
�������������� ������������ ����� ��������
� �������� ��� �� ��������� ���a�����.
��� ������� � �������� ��������� � �������� \fB\elist\fR,
�� ����������� ����, ��� ��������� ��������� � �����
��������, � �� ��������� �� ��������.
.s3
.xx \ee
.xx \eed
.xx \eedit
.xx \eeditor
����� ��������� ������� \*(UU
(��. ED, RED � "\fB����������� ������������ \*(UU\fR");
��� �������� � ������� \*(II ������������ �������
������ �� ���������. ��� ������������� ��������� ED
� �������� ������ �����, ������������� ���������� ����� �����,
� �� ����� ��������.
���� ��������� ������ ``\fI{editor}\fR'',
�� �� ������������ � �������� ������� ����� ���������,
� ��������� ������ ������������ ����� ``\fI/bin/ed\fR''.
.s3
.xx \eg
.xx \ego
���������� ������� ������.
���������� ������ �������������� ����������������,
���������� \*(II, � �����������.
.s3
.xx \ea
.xx \eappend
�������� � ����� ��������.
��� ����� ������� \ea ����� ���������� ������� �����������
����������� � �������������, ��� ����� ��������
�� ����� �������.
.s3
.xx \etime
.xx \edate
������ ������� ����� ���.
.s3
.xx \es
.xx \esh
.xx \eshell
����� � shell.
\fBCTRL-D\fR ������������ ��������� ������ � shell
� ������� � ������� \*(II.
���� ��� ������� \fB\eshell\fR ������� ��� �����,
�� ��� �������������� ��� ��������� ���� shell,
������� �����������
� ������� ������� � �������� ���������
``\fI$1\fR''.
���� ��� ����� �� ������,
�� shell ��������� �������.
���� ��������� ������ ``\fI{shell}\fR'',
�� �� ������������ ��� ������ ��� shell;
� ��������� ������ ������������
``\fI/bin/sh\fR''.
.s3
.xx "\e!\fI������� ���������\fR"
.br
��������� \fI�������\fR ������������ ������� � �����
��������� � �������. ������� ����������� ��� �� ���������������,
������� ���������� �� ������� \fB\eshell\fR.
.s3
.xx \eq
.xx \equit
����� �� \*(II.
.s3
.xx \ecd
.xx \echdir
�������� ������� ������� �������� �� ��������� �������.
.s3
.xx \ei
.xx \einclude
.xx \eread
������������� �� ���� �� ���������� �����.
������� "\fB\e\fR" � ����� ����� �������������� �� ���� ����������.
.s3
.xx \ew
.xx \ewrite
�������� ���������� ������ �������� � ��������� ����.
.s3
.xx \ebranch
�������� ���������� � ����, ��������� � ����� �������� \fB\einclude\fR.
��. ����� ������ � ���������.
.s3
.xx \emark
���������� ����� ��� \fB\ebranch\fR.
.s3
.xx "\e\fI<����� ������ ������>\fR"
.br
������������ ����� ��������� ����������� �������� �������,
���������� �� '\fB\e\fR'.
��� ��������� ������������ '\fB\e\fR'
� �������� ������������ �������.
(��.����� quel(quel) - ������).
����� ��������, ��� ������������ \fB\e\fR ������� ������
��'������� ����������������;
� ����� ������, ���� ��� ����� ������
\e, ����� ������� ��� ������� \e\e
(�������� ������� ���� \- ����������
�� ������������ ���� �������� ������).
.i0
.s1
.bd ������������
.s2
������������ ����������� � �������
macros(quel).
.s1
.bd "��������"
.s2
������� \fB\ebranch\fR � \fB\emark\fR �������������
����������� ������ ����� �������� � ��������
������, ���������� � ����� �������� \einclude.
(������� �������� ``\fIgoto\fR'' � ``\fI:\fR'' � shell).
�� \fB\emark\fR ������ ���� �����.
�� \fB\ebranch\fR ������ ���� ���� �����,
���������� ����������� �������,
���� ���������, ����� ������� ����� ���� �������
� ����������� ������, ���������� �������� �������.
������� �������������� � ��� ������, ����
�������� ��������� ������ ����.
��������,
.br
	\ebranch ?{tuplecount}<=0 notups
.br
��������� �� ����� ``\fInotups\fR'' ���� ��������� ������������
``\fI{tuplecount}\fR'' ������ ��� ����� ����.
.s3
���������, ������������ � ���������� \ebranch
����� �����������.
��������� +, \-, \*(**,
.if t \(sl,
.if n /,
<=, >=, <, >, = � !=
������������ ������������ ��������.
����� ������� �������� \fB!\fR ����� ��������������
� ��� �������� ����������� ���������.
� ���������� �� ������ ���� ��������, �.�.
������ �������� ��������� ���������.
.s1
.bd "�������������"
.s2
�� ����� ����� � \*(II
���������� ��� ���������������� ��������.
��-������, ������������ ������������
���������� \fB{pathname}\fR,
�������� ������� ���������� ������ ���
��������� \*(II;
��� ������������ ������ ���������� �����������,
��� \fBdemodb\fR.
��- ������,
����������� ���� �������������
\fB\&.../files/startup\fR.
���� ���� ��������������� ��� ����������� �����������������
����������,
�����, ��� �������� � shell.
�-�������, ����������� � �����������
���� ������������� ������������, ��������� �
��������������� ���� ���������������� ����� ������������� "users".
��� ������ ���� ``\fI.\*(rN\fR'' � ��������
�������� ������������.
���� ������������� ����� ����������� ��� �����������
��������� �����������, ���������� ����� ������������
���������� �������� ��������� � �.�.
�������,
���������� ���������� ��
�������� ������������.
.s3
���������� ���������� ������ �� ������ �������������
������� ��������� ���������� ������� ����.
.s1
.bd �����
.s2
� �������� ������ ����� ���� ������ �����
��� \*(II, �������� �� ����� ������ �������������
��������. ����
\fB\-a\fR �������� ������� �����������.
��� ��������, ��� ����� �������� ������� ��
����� ��������� �������������;
��� ������������ ����, ���
����� ������� \fB\ego\fR ��������
������� \fB\eappend\fR.
��� ��������, ��� ������������
������ ����� ������� ������� ����� ��������
�������� \fB\ereset\fR �����
������� �������.
\fB\-d\fR ��������� ������ \fBdayfile\fR.
\fB\-s\fR ��������� ������ ���� ��������� (�� �����������
��������� �� �������) ��������,
������� ��������������� � �������� ���������,
\fBdayfile\fR � ���������.
���� ����� ������������ ��� ����������
��������, ������������ �� ������.
.sh "�������������� ������"
ingres(\*(dM),
quel(quel),
macros(quel)
.sh �����������
.xx go
.xx
�� ������ �������� ����� ������
.s3
.xx continue
���������� ������ ������� � �� ��������� � �������.
.s3
.xx Executing . . .
.br
������ �������������� \*(II.
.s3
.xx >>ed
�� ����� � ��������� �������� \*(UU.
.s3
.xx >>sh
�� ����� �� ������������� ���������� ����� ������� \*(UU.
.s3
.xx "������������ ������ nnn ������������� � ������"
.br
\*(II ���������� ������������ ������� ASCII � �������;
��� ��������� ��������� �� ��, ��� ������ ��� ����� �����
 ���� ����� ��������������.
.i0

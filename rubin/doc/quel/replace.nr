.th REPLACE QUEL "���� \*(RU. ��������� QUEL"
.sh ���
replace \- �������� �������� ������� � ���������
.sh ������
 \fBreplace \fI������-���������� \fB(\fI������_��������\fB)\fR [\fBwhere \fI�������\fR]
.sh ��������
.bd Replace
�������� �������� �������
������������ �
.it ������_��������
��� ���� ��������, �������
�������������
.it �������.
������ ���� ���������
.it ������_����������,
������� ���������� � �������������� ���������.
�������, ��� ��������� \fB������-����������\fR,
� �� ��� ���������.
�
.it ������_�������
������ ��������������
������ �� ������, ������� �������� �����������.
��� ������ ������ ���� ���������� ��� ���_����������
�
.it ������_��������
� ����� ����, ���� �� ��������� (��. quel(quel)).

�������� ������ ����� ���� �������� �� �������� ������
��������� ����
(�� ����������� ���������� ����).
�������� ������ ����� ����������������� � ��� ��������������� ������.
.s1
������ ����� ����������� ������ �������� ���������
��� ������������, ������� ���������� �� ������.
.s1
����  ���������� ������� ����� �������� �����������
����������� (��. integrity(quel)), �� ��� �� ������������.
.sh ������
.nf
/\*(** ���� ��������, ������� �������� �� �����,
       ���� 10% ��������                         \*(**/
   range of e is emp
   replace e(sal = 1.1 \*(** e.sal) where e.mgr = "Smith"
.fi
.sh "�������������� ������"
integrity(quel), permit(quel), quel(quel), range(quel)
.sh �����������
������������� ��������� ��������� ���� ��� ������ ������ �����������
���� ��� �������� ��������
������ ��������������� ���������.

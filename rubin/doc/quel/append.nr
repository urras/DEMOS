.th APPEND QUEL "���� \*(RU. ��������� QUEL"
.sh ���
append \- �������� ������� � ���������
.sh ������
.bd append
[\c
.bd to\c
] ���_��� (������_��������) [\c
.bd where
�������\c
]
.sh ��������
.it Append
��������� �������, ������� �������������
.it �������.
.it ���_���
������ ������������ ����� ��� ������������� ���������.
.it ������_��������
������ �������� ��������� ��� ��������, ������� ������
���� ��������� � ���������
.it ���_���.
�������� ����� ������������� � ����� �������.
�������� ��������������� ���������,
������� �� ������������ �
.it ������_��������,
�������� �������� �� ���������: 0 ���
��������� ��������� ��� ������ ���
���������� ���������.
.sp 1
�������� ��� ��������� ������ ���������� ����
����� �������������� ��� �������
�������� ������ ���������� ����.
��� ���� ���������� ��������������
� �������������� ��� ������.
��������� �������� �� ����� ���������������
������������� ���������� �������.
�������������� �� ���������� � ���������� ���
����� ���� ����������� � �������������� ���������
.bd ascii
(��. quel(quel)).
���������� �������� �� �����
��������������� �������������
��������� �������.
��� �������������� ���������� ��������
� ��������, ����������� �������
.bd int1\c
,
.bd int2\c
 � �.�. (��. quel(quel))
.s3
�������� �����
.bd all
����� �������������� ��� �������������
.bd ����
������� ���������.
.s3
�������
.it append
����� ����������� ������ ���������� ���������
��� �������������, ������� ����������
.it append
�� ������ ���������.
.sh ������
.nf
/\*(** ������ ������ ��������� ������, ����������� ��� ����� \*(**/
   range of n is newemp
   append to emp(n.name, n.sal, mgr = "Smith", bdate = 1975\-n.age)
		where n.name = "Jones"
/\*(** ������������ ��������� newemp1 � newemp \*(**/
   range of n1 is newemp1
   append to newemp(n1.all)
.fi
.sh "�������������� ������"
copy(quel), permit(quel), quel(quel), retrieve(quel)
.sh �����������
������������� ��������� ���������� ����
��� ����������� ��������
������ ����������� ����
��� ��������, �������� � ��������� ����������������
���������.
.sh ������
������������� �������, �������������� � ���������,
��������� ��� ''paged heap''
("����������� ����"),
�� ���������.

c
c*********************************************
c       �/� ������������ ������� (�) � (�).
c*********************************************
c
c       n - ����� �������
c       k - ����� �����
c       l - ����� ��������
c
	subroutine prmatr(a,b,n,k,l)
	dimension a(10,4,4),b(10,4,4)
	do 10 j=1,k
	do 10 i=1,l
10      b(n,j,i)=a(n,j,i)
	return
	end
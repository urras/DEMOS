c
c**********************************************
c       �/� �������� ������� (�).
c**********************************************
c
c       n - ����� �������
c       k - ����� �����
c       l - ����� ��������
c
c
	subroutine obmatr(a,n,k,l)
	dimension a(10,4,4)
	do 10 j=1,k
	do 10 i=1,l
10      a(n,j,i)=0.
	return
	end

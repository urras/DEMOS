c
c**************************************
c      π/π χχοδ νατςιγ πο στςολαν
c**************************************
c
	subroutine vvodm(a,n,naz,for)
	dimension a(10,4,4)
	print 100, naz,for
100     format(1x,'enter value of matr',a3,
&       ' by format ',a4)
	do 102 i=1,n
	read(5,202)((a(i,j,k), k=1,4),j=1,4)
202     format(f10.6)
102     continue
	return
	end

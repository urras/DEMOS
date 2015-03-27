c
c*********************************************
c       п/п ввод вектора
c*********************************************
c
	subroutine vvodv(a,n,naz,for)
	dimension a(10)
	print 100,naz,for
100     format(1x,'enter value of vector ',a2,
&       ' by format ',a4)
	read(5,200)(a(i),i=1,n)
200     format(f10.6)
	return
	end

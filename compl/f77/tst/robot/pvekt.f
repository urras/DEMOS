c
c*********************************************
c       п/п печати вектора (а)
c*********************************************
c
c       fnas - название вектора
c       fimy - коментарий к названию вектора
c       l    - число строк в векторе
c       м    - число звеньев (номер вектора)
c
c
	subroutine print(a,fnas,fimy,l,m)
	dimension a(10,4,1)
	double precision fimy
	print 10,fnas,fimy
10      format(1x,a4/a8)
	do 20 n=1,l
	print 30,(a(i,n,1),i=1,m)
20      continue
30      format(11x,6(f10.5))
	return
	end

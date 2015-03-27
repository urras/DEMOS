	subroutine primat(nm,mm,d,np,pm)
	dimension d(nm,mm),mst(10)
	do 3 j=1,10
3       mst(j) = j
	m1=1
	l = mp / 10
	if (mp-l*10.ne.0) l = l+1
	print 1020
	do 5 i=1,l
	print 1021,mst
	m2 = m1 + 9
	if(i.eq.l) m2 = mp
	print 1022,(n,(d(n,m),m=m1,m2),n=1,np)
	m1 = m2 +1
	do 4 j=1,10
4       mst(j) = mst(j)+10
5       continue
	print 1020
c комментарий
1020    format(/1x,123(1h-))
1021    format(1x,10(5x,i4,3x))
1022    format(1x,i3,10e12.3)
	return
	end

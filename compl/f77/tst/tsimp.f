c расчет таблицы простых чисел
	parameter (marr=20)
	parameter (nlinep=80/7)
	integer*2 isymp(marr),iline(nlinep)
	character ee*20
	data ee/'Конец расчета'/
	ia=1
	ip=3
	isymp(ia)=ip
	lp=1
	iline(lp)=ip
2       ip=ip+2
	do 3 i=1,ia
	j=isymp(i)
	if(j*j.gt.ip) goto 4
	l=ip/j
	if(l*j.eq.ip) goto 2
3       continue
	print 102,(iline(i),i=1,lp)
102     format(20i6,x)
	print 101,ee
101     format('w=',a)
	stop
4       if(ia.ge.marr) goto 41
	ia=ia+1
	isymp(ia)=ip
41      lp=lp+1
	iline(lp)=ip
	if(lp.lt.nlinep) goto 2
	print 102,iline
	lp=0
	goto 2
	end

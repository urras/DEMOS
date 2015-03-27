	common nfaces,erce,volume
1       type 71
71      format("Введите число граней, длину грани")     ! Программа считает
	accept 70,nfaces,erce                           ! об'ем правильного
	type 70,                       ! многогранника
     *  nfaces,erce

70      format(i2,f8.5)
	if(nfaces.eq.0) stop
	call obmng
	print 72,volume
72      format("obiem =",f8.5)
	goto 1
	end
	subroutine squarg(n)
	common nfaces,erce,volume
	cubes = erce**n
	goto 101
	entry obmng
	cubed = erce**3
101     goto (6,6,6,1,6,2,6,3,6,6,6,4,6,6,6,6,6,6,5,6),nfaces
1       volume = cubed*0.11785
	return
2       volume = cubed
	return
3       volume = cubed*0.47140
	return
4       volume = cubed*7.66312
	return
5       volume = cubed*2.18170
	return
6       print 25,nfaces
25      format("Uncorrect number of faces: ",i6)
	return
	end

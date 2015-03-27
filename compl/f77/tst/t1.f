	i=0
	x=1.2
	k=i/x
	call t(k)
	stop
	end
	subroutine z(i)
	i=0
	x=x*1.2
	k=mod(5,i)
	call t(k)
	end

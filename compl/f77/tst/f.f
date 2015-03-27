	subroutine aaa(s,c,n)
	dimension s(20),c(n),t(20)
	integer*2 i2
	character*50 cc(100)
	common /alpha/cc,rr,i2,t
	cc(1)="abcdef"
	rr=0.0
	do 1 i=1,20
	s(i)=sin(rr)
	c(i)=cos(rr)
	t(i)=s(i)/c(i)
	rr = rr + 3.14/20.
	write(6,2)rr,s(i),c(i),t(i)
1       continue
	stop
2       format(2hr=,f5.2,3h s=,e9.2,3h c=,e9.2,e9.2)
	end

	program test
	integer*2 i,j,rep,thirty,ten,five,n
	real*4 acc, divn,y,root,x
	dimension a(30),b(30),c(30)
	n=1000
c       n=10000
	five=5
	ten=10
	thirty=30
	divn=1.0/float(n)
	x=.1
	acc=0.0
	y=1.0
	do 1 i=1,30
	a(i)=i
	b(i)=-y
1       y=-y
	do 15 rep=1,n
	i=30
	do 2 j=1,thirty
	c(i)=a(i)+b(i)
2       i=i-1
	y=0.
	do 3 i=1,ten
3       y=(y+c(i))*x
	acc=acc+y*divn
	y=c(11)
	do 4 i=12,20
	if(c(i).gt.y) y=c(i)
4       continue
	root=1.0
	do 5 i=1,5
5       root=0.5*(root+y/root)
	acc=acc+root*divn
	do 6 i=1,thirty
6       a(i)=c(i)-b(i)
	y=0.0
	do 7 i=1,ten
7       y=(y+a(i))*x
	root=1.0
	do 8 i=1,five
8       root=0.5*(root+y/root)
	acc=acc+root*divn
	do 9 i=1,thirty
9       c(i)=c(i)*b(i)
	y=c(20)
	do 10 i=21,thirty
	if(c(i).gt.y) y=c(i)
10      continue
	root=1.0
	do 11 i=1,5
11      root=0.5*(root+y/root)
	acc=acc+root*divn
	y=0.0
	do 12 i=1,ten
12      y=(y+c(i))*x
	acc=acc+y*divn
	y=c(1)
	do 13 i=2,ten
	if(c(i).gt.y) y=c(i)
13      continue
	root=1.0
	do 14 i=1,five
14      root=0.5*(root+y/root)
	acc=acc+root*divn
15      continue
	print 100,n,acc
100     format(1h0,i10,e17.9)
	stop
	end

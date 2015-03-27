	program tmem
	integer*2 i,j,k
	read *,k,m
	print *,k,m
	j=0
	do 1 k=1,256
	j = j + i
1       continue
	do 2 i=1,k
	j = j + i
2       continue
	do 11 k=1,256
	j = j + i
11       continue
	do 12 i=1,k
	j = j + i
12       continue
	do 21 k=1,256
	j = j + i
21       continue
	do 22 i=1,k
	j = j + i
22       continue
	print *,j
	stop
	end

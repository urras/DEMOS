	integer iarray(-10:10,-10:10),iar1(101)
	equivalence (iarray,iar1(2))
	do 1 i=1,101
1       iar1(i) = i
	print "(10i5)",((iarray(i,j),i=-10,10),j=-10,10)
	stop
	end

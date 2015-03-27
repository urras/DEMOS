	program test
	common /opneof/opene
	integer i(512)
	open(3,file="poker.f",form="unformatted")
	opene=1
	open(2,file="a",form="unformatted")
	do10 j = 1,2
	read(3)i
	write(2)i
10      continue
	stop
	end

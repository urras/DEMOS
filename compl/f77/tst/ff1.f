	subroutine obmng
	common nfaces,erce,volume
	byte c
	cubed = erce**3
	goto (6,6,6,1,6,2,6,3,6,6,6,4,6,6,6,6,6,6,5,6),nfaces
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
6       type 25,nfaces
25      format("Uncorrect number of faces: ",i6)
	return
	end

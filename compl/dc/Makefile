all:	dc

cmp:	dc
	cmp dc ${DESTDIR}/usr/bin/dc

cp:	dc
	cp dc ${DESTDIR}/usr/bin/dc
	rm dc

dc:	dc.c dc.h
	cc -n -s -O dc.c -o dc

clean:
	rm -f *.o

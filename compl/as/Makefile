all:	as as2

cmp:	as as2
	cmp as ${DESTDIR}/bin/as
	cmp as2 ${DESTDIR}/lib/as2
	rm a.out

cp:	as as2
	cp as ${DESTDIR}/bin/as
	cp as2 ${DESTDIR}/lib/as2
	rm as as2 a.out

as:
	as /usr/include/sys.s as1?.s
	ld -n -s a.out -o as

as2:
	as /usr/include/sys.s as2?.s
	ld -n -s a.out -o as2

clean:
	rm -f *.o a.out

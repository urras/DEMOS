CFLAGS=		-O

libdbm.a:	dbm.c
		cc -c ${CFLAGS} dbm.c
		mv dbm.o libdbm.a

cp:		libdbm.a
		cp libdbm.a ${DESTDIR}/usr/lib/libdbm.a
		ranlib ${DESTDIR}/usr/lib/libdbm.a

clean:
		-rm -f *.o libdbm.a

#	@(#)Makefile	4.1	12/25/82
#
CFLAGS=-O
OBJS= pow.o gcd.o msqrt.o mdiv.o mout.o mult.o madd.o util.o

libmp.a: $(OBJS)
	ar cr libmp.a $(OBJS)

cp:	libmp.a
	cp libmp.a ${DESTDIR}/usr/lib/libmp.a
	ranlib ${DESTDIR}/usr/lib/libmp.a

clean:
	rm -f *.o libmp.a

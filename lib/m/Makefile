DESTDIR =
FP =			# use -f if no floating point
CFLAGS = -O ${FP}

.c.o:
			$(CC) $(CFLAGS) -c $*.c
			ld -r -x $*.o -o a.lib
			mv a.lib $*.o

OBJS =	asin.o atan.o hypot.o jn.o j0.o j1.o pow.o fabs.o gamma.o \
	log.o sin.o sqrt.o tan.o tanh.o sinh.o exp.o floor.o

libm.a:	${OBJS}
	-rm -f libm.a
	ar rcv libm.a `lorder ${OBJS} | tsort`
	ranlib libm.a

cp: libm.a
	cp libm.a ${DESTDIR}/lib/libm.a
	ranlib ${DESTDIR}/lib/libm.a

clean:
	-rm -f *.o libm.a

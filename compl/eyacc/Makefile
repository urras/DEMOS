INSTALLDIR = ${DESTDIR}/usr/bin
CC =	cc -O -I${INCLUDES}
INCLUDES =
LDFLAGS = -i -s

all: eyacc

cp: eyacc
	cp eyacc ${INSTALLDIR}/eyacc

cmp: eyacc
	cmp eyacc ${INSTALLDIR}/eyacc

eyacc: ey0.o ey1.o ey2.o ey3.o ey4.o ey5.o
	cc ${LDFLAGS} -o eyacc ey0.o ey1.o ey2.o ey3.o ey4.o ey5.o

.c.o:
	cc -O -c $*.c

clean:
	-rm -f *.o eyacc

print:
	@ls -l | pr
	@pr READ_ME makefile
	@pr ey.h ey0.c ey1.c ey2.c ey3.c ey4.c ey5.c

ey1.o:	ey.h
ey2.o:	ey.h
ey3.o:	ey.h
ey4.o:	ey.h

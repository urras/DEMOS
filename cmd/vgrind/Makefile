CFLAGS=-O -I${INCLUDES}
INCLUDES=
SOURCES=vfontedpr.c vgrindefs.c regexp.c retest.c
OBJS=vfontedpr retest

all:    $(OBJS)

vfontedpr: vfontedpr.o vgrindefs.o regexp.o
	cc -f -O -o vfontedpr vfontedpr.o vgrindefs.o regexp.o
retest: retest.o regexp.o
	cc -o retest retest.c regexp.o

install: all
	cp vfontedpr ${DESTDIR}/usr/ucb/lib/vfontedpr
	cp vgrind.csh ${DESTDIR}/usr/ucb/bin/vgrind
	chmod +x ${DESTDIR}/usr/ucb/bin/vgrind
	cp vgrindefs.src ${DESTDIR}/usr/ucb/lib/vgrindefs
	cp tmac.vgrind ${DESTDIR}/usr/lib/tmac/tmac.vgrind

clean:
	rm -f ${OBJS}

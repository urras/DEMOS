#
# Makefile для библиотеки libsa
#

#CC=		cc
#AR=		ar
#INCLUDES=       -I.. -I/usr/include
#CCFLAGS=        -O ${INCLUDES}
CC=		$(HOME)/d22/bin/cc
AS=		$(HOME)/d22/bin/as
AR=		$(HOME)/d22/bin/ar
RANLIB=		$(HOME)/d22/bin/ranlib
INCLUDES=	-I.. -I$(HOME)/d22/include
CFLAGS=		${INCLUDES}

# avoid "-target sun4"
.c.o:
	$(CC) -c $(CFLAGS) $*.c

.s.o:
	$(AS) -u -o $*.o $*.s

DRIVERS=        prf.o xp.o ht.o rk.o rm.o rl.o rp.o dvhp.o uda.o\
		hk.o ts.o tm.o fd.o video.o pcopy.o lprf.o dw.o my.o # rw.o

../libsa.a:     SYS.o ${DRIVERS}       # font1.o
		$(AR) rv ../libsa.a $?
		$(RANLIB) ../libsa.a

font1.o: ../../dev/font1.cm ../../dev/font1.mm ../../dev/font1fix.c
	mm ../../dev/font1.mm < ../../dev/font1.cm  >font1.c
	$(CC) -S font1.c
	mv font1.s f.s
	sed 's/data/text/' < f.s >font1.s
	$(CC) -c font1.s ../../dev/font1fix.c
	$(LD) -r -o f.o font1.o font1fix.o
	mv f.o font1.o
	-rm -f f.s font1.s font1.c font1fix.o

clear clean:
	-rm -f *.[ob]

CLRSRC: clear
	-rm -f *.c

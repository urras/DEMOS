#
#  Makefile for RUBIN gutil library
#
#	@(#)Makefile	7.3	9/26/83
#

RUBIN=	../..
H=	../h
LIB=	$(RUBIN)/lib
# LIBC=   /lib/Slibc.a

OBJS=	append.o atof.o bequal.o bitcnt.o bitpos.o cat.o \
	concat.o freebuf.o ftoa.o fullwait.o \
	getufield.o iocv.o itoa.o length.o \
	xalloc.o need.o oatoi.o pad.o pmove.o \
	prargs.o scompare.o sequal.o set_si_buf.o set_so_buf.o \
	setflag.o trace.o atol.o smove.o \
	lprintf.o clrmem.o xputchar.o ztack.o syserr.o \
	min.o xfree.o bmove.o perror.o locv.o strcspn.o

SRCS1=	append.c atof.c bequal.c bitcnt.c bitpos.c cat.c \
	concat.c freebuf.c ftoa.c fullwait.c \
	bmove.c \
	getufield.c iocv.c itoa.c length.c \
	xalloc.c need.c oatoi.c pad.c pmove.c
SRCS2=	prargs.c scompare.c sequal.c set_si_buf.c set_so_buf.c \
	setflag.c trace.c atol.c smove.c \
	xputchar.c ztack.c syserr.c \
	min.c xfree.c lprintf.c clrmem.c perror.c locv.c  strcspn.c
SRCS=	$(SRCS1) $(SRCS2)

CFLAGS=	-I$H -O
AR=	-ar
ARFLAGS=urv
RANLIB= ranlib
GET=	sccs get
REL=
PR=	print
VGRIND=	csh /usr/ucb/vgrind
HEADER=	Gutil Library
CP=     ln
CHMOD=	chmod
LIBMODE=664

gutil:	$(OBJS)
	$(AR) $(ARFLAGS) gutil `lorder $?|tsort`
	$(CHMOD) $(LIBMODE) gutil
	$(RANLIB) gutil

doprnt.o: doprnt.s
	cp doprnt.s doprnt.c
	cc -E doprnt.c | as -o doprnt.o
	-ld -x -r doprnt.o
	mv a.out doprnt.o
	rm doprnt.c

errlst.o: $(LIBC)
	ar x $(LIBC) errlst.o
	touch errlst.o

install: $(LIB)/gutil

$(LIB)/gutil: gutil
	rm -f $(LIB)/gutil
	$(CP) gutil $(LIB)/gutil
	$(RANLIB) $(LIB)/gutil

new:
	rm -f gutil

clean:
	rm -f [0-m]*.o
	rm -f [n-z]*.o
	rm -f core gutil

sources: $(SRCS1) $(SRCS2)

A$(SRCS1):
	$(GET) $(REL) SCCS/s.$@
A$(SRCS2):
	$(GET) $(REL) SCCS/s.$@

print: sources
	$(PR) Makefile *.[hm] *.[csy]

vgrind: sources
	cp /dev/null index
	$(VGRIND) -h "$(HEADER) (Release 7)" -n Makefile
	$(VGRIND) -h "$(HEADER) (Release 7)" *.[csy]
	$(VGRIND) -h "$(HEADER) (Release 7)" -x index

depend:
	grep '^#[ 	]*include' $(SRCS) | sed -n -f ../h/depend.sed > makedep
	echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile Makefile.bak
	ed - Makefile < eddep
	rm eddep makedep
	echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	echo '# see make depend above' >> Makefile

# DO NOT DELETE THIS LINE -- make depend uses it

bequal.o:$H/useful.h
xalloc.o:$H/useful.h
sequal.o:$H/useful.h
set_si_buf.o:$H/defines.h
set_so_buf.o:$H/defines.h
trace.o:$H/useful.h
lprintf.o:$H/useful.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above

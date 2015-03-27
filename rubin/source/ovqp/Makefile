#
#  Makefile for RUBIN One Variable Query Processor (OVQP)
#
#	@(#)Makefile	7.2	9/26/83
#

RUBIN=	../..
H=	../h
LIB=	$(RUBIN)/lib

CFLAGS=	-O -I$H
AR=	-ar
ARFLAGS=rvu
RANLIB= ls -l
LDFLAGS=
CP=     ln
CHMOD=	chmod
LIBMODE=664
BINMODE=4740
GET=	sccs get
REL=
PR=	print
VGRIND=	csh /usr/ucb/vgrind
HEADER=	One Variable Query Processor

CTLMOD=	$(LIB)/ctlmod
IUTIL=	$(LIB)/iutil
GUTIL=	$(LIB)/gutil

OBJS=	scan.o interp.o startovqp.o\
	strategy.o findsimps.o key.o string.o typecheck.o interp1.o \
	equelpr.o prsym.o tidtest.o datimop.o
SRCS=	scan.c interp.c startovqp.c\
	strategy.c findsimps.c key.c string.c typecheck.c interp1.c \
	equelpr.c prsym.c tidtest.c datimop.c\
	ovqp.h strategy.h

LIBS=	$(CTLMOD) $(IUTIL) $(GUTIL)

ovqplib: $(OBJS)
	$(AR) rvu ovqplib  `lorder $?|tsort`
	$(CHMOD) $(LIBMODE) ovqplib
	$(RANLIB) ovqplib

install: $(LIB)/ovqp

$(LIB)/ovqp: ovqplib
	rm -f $(LIB)/ovqp
	$(CP) ovqplib $(LIB)/ovqp
	$(RANLIB) $(LIB)/ovqp

ovqp:	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o ovqp
	$(CHMOD) $(BINMODE) ovqp

sources: $(SRCS)

$(SRCS):
	$(GET) $(REL) SCCS/s.$@

new:
	rm -f ovqplib

clean:
	rm -f *.o
	rm -f ovqplib a.out core ovqp

print: sources
	$(PR) Makefile TraceFlags *.[hm] *.[csy]

vgrind: sources
	cp /dev/null index
	$(VGRIND) -h "$(HEADER) (Release 7)" -n Makefile TraceFlags
	$(VGRIND) -h "$(HEADER) (Release 7)" *.[hm] *.[csy]
	$(VGRIND) -h "$(HEADER) (Release 7)" -x index

ncheck: 
	ckdir -c

backup:  ncheck
	ar vu backup.a *.[chys]

check:
	ckdir

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

# interp.o:interp.c
#         $(CC) -NT INTER -c $(CFLAGS) interp.c
#
# typecheck.o:typecheck.c
#         $(CC) -NT INTER -c $(CFLAGS) typecheck.c

# DO NOT DELETE THIS LINE -- make depend uses it

scan.o:$H/defines.h $H/local.h
scan.o:$H/symbol.h
scan.o:$H/tree.h
scan.o:../decomp/globs.h
interp.o:$H/defines.h $H/local.h
interp.o:$H/aux.h
interp.o:$H/symbol.h
interp.o:$H/tree.h
interp.o:$H/datetime.h
interp.o:$H/access.h
interp.o:../decomp/globs.h
startovqp.o:$H/defines.h $H/local.h
startovqp.o:$H/symbol.h
startovqp.o:$H/tree.h
startovqp.o:$H/aux.h
startovqp.o:../decomp/globs.h
startovqp.o:../ctlmod/pipes.h
startovqp.o:$H/datetime.h
strategy.o:$H/defines.h $H/local.h
strategy.o:$H/aux.h
strategy.o:$H/catalog.h
strategy.o:$H/symbol.h
strategy.o:$H/tree.h
strategy.o:../decomp/globs.h
strategy.o:strategy.h
findsimps.o:$H/defines.h $H/local.h
findsimps.o:$H/symbol.h
findsimps.o:$H/tree.h
findsimps.o:../decomp/globs.h
findsimps.o:strategy.h
key.o:$H/defines.h $H/local.h
key.o:$H/aux.h
key.o:$H/symbol.h
key.o:$H/tree.h
key.o:../decomp/globs.h
key.o:strategy.h
string.o:$H/defines.h $H/local.h
string.o:$H/aux.h
string.o:$H/symbol.h
string.o:$H/tree.h
string.o:../decomp/globs.h
string.o:$H/datetime.h
typecheck.o:$H/defines.h $H/local.h
typecheck.o:$H/aux.h
typecheck.o:$H/tree.h
typecheck.o:$H/symbol.h
typecheck.o:../decomp/globs.h
typecheck.o:$H/datetime.h
interp1.o:$H/defines.h $H/local.h
interp1.o:$H/aux.h
interp1.o:$H/symbol.h
interp1.o:$H/tree.h
interp1.o:../decomp/globs.h
equelpr.o:$H/defines.h $H/local.h
equelpr.o:$H/aux.h
equelpr.o:$H/symbol.h
equelpr.o:$H/tree.h
equelpr.o:$H/access.h
equelpr.o:../ctlmod/pipes.h
equelpr.o:../decomp/globs.h
prsym.o:$H/defines.h $H/local.h
prsym.o:$H/aux.h
prsym.o:$H/symbol.h
prsym.o:$H/tree.h
prsym.o:../decomp/globs.h
prsym.o:$H/datetime.h
tidtest.o:$H/defines.h $H/local.h
tidtest.o:$H/symbol.h
tidtest.o:$H/tree.h
tidtest.o:../decomp/globs.h
tidtest.o:strategy.h
datimop.o:$H/defines.h $H/local.h
datimop.o:$H/datetime.h
datimop.o:$H/aux.h
datimop.o:$H/symbol.h
datimop.o:$H/tree.h
datimop.o:../decomp/globs.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above

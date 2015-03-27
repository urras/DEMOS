#
#  Makefile for tty monitor
#
#	@(#)Makefile	7.1	2/5/81
#

RUBIN=	../..
LIB=	$(RUBIN)/lib
H=	../h

CTLMOD=	$(LIB)/ctlmod
IUTIL=	$(LIB)/iutil
GUTIL=	$(LIB)/gutil
LIBS=	$(CTLMOD) $(IUTIL) $(GUTIL)

OBJS=   mon_sql.o mon_nosql.o\
	ttymon.o monitor.o clear.o edit.o go.o trapquery.o \
	print.o quit.o shell.o eval.o writeout.o xwait.o \
	clrline.o prompt.o getch.o branch.o newdirec.o \
	include.o getfilenm.o expr.o mcall.o mac.o buf.o \
	syscntrl.o
SRCS=	ttymon.c monitor.c clear.c edit.c go.c trapquery.c \
	print.c quit.c shell.c eval.c writeout.c xwait.c \
	clrline.c prompt.c getch.c branch.c newdirec.c \
	include.c getfilenm.c expr.c mcall.c mac.c buf.c \
	syscntrl.c buf.h monitor.h\
	mon_sql.c mon_nosql.c

CFLAGS=	-I$H -O -I../ctlmod
LDFLAGS=
BIN=	$(RUBIN)/bin
GET=	sccs get
REL=
AR=	-ar
ARFLAGS=rvu
RANLIB= ls -l
PR=	print
VGRIND=	csh /usr/ucb/vgrind
HEADER=	Terminal Monitor
CP=     cp
CHMOD=	chmod
LIBMODE=664
BINMODE=751
ROMODE=	444

monlib: $(OBJS)
	$(AR) $(ARFLAGS) monlib  `lorder $?|tsort`
	$(CHMOD) $(LIBMODE) monlib
	$(RANLIB) monlib

install: $(LIB)/monitor

$(LIB)/monitor: monlib
	rm -f $(LIB)/monitor
	$(CP) monlib $(LIB)/monitor
	$(RANLIB) $(LIB)/monitor

monitor: $(OBJS) config.o
	$(CC) $(LDFLAGS) config.o $(OBJS) $(OTHERS) $(LIBS) -o monitor
	$(CHMOD) $(BINMODE) monitor
	ls -l monitor; size monitor

new:
	rm -f monlib monitor

clean:
	rm -f a.out monitor monlib core
	rm -f *.o

sources: $(SRCS)

print: sources
	$(PR) Makefile TraceFlags *.[hm] *.[csy]

vgrind: sources
	cp /dev/null index
	$(VGRIND) -h "$(HEADER) (Release 7)" -n Makefile TraceFlags
	$(VGRIND) -h "$(HEADER) (Release 7)" *.[hm] *.[csy]
	$(VGRIND) -h "$(HEADER) (Release 7)" -x index

config.c: ../conf/SCCS/s.monitor.c
	rm -f config.c
	$(GET) $(REL) -p ../conf/SCCS/s.monitor.c > config.c
	$(CHMOD) $(ROMODE) config.c

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

ttymon.o:monitor.h
ttymon.o:$H/version.h
ttymon.o:$H/opsys.h
ttymon.o:$H/pv.h
ttymon.o:$H/func.h
monitor.o:monitor.h
monitor.o:$H/aux.h
clear.o:monitor.h
clear.o:$H/aux.h
edit.o:monitor.h
edit.o:$H/aux.h
edit.o:$H/opsys.h
go.o:monitor.h
go.o:$H/aux.h
go.o:$H/resp.h
go.o:$H/symbol.h
go.o:$H/pv.h
trapquery.o:monitor.h
trapquery.o:$H/trace.h
trapquery.o:$H/resp.h
print.o:monitor.h
print.o:$H/aux.h
quit.o:monitor.h
quit.o:$H/aux.h
shell.o:monitor.h
shell.o:$H/aux.h
shell.o:$H/opsys.h
eval.o:monitor.h
eval.o:$H/aux.h
writeout.o:monitor.h
writeout.o:$H/aux.h
xwait.o:monitor.h
xwait.o:$H/aux.h
clrline.o:monitor.h
clrline.o:$H/aux.h
prompt.o:monitor.h
prompt.o:$H/aux.h
getch.o:monitor.h
getch.o:$H/aux.h
branch.o:monitor.h
branch.o:$H/aux.h
newdirec.o:monitor.h
newdirec.o:$H/aux.h
include.o:monitor.h
include.o:$H/aux.h
getfilenm.o:monitor.h
getfilenm.o:$H/aux.h
mac.o:$H/code.h
mac.o:$H/useful.h
mac.o:$H/trace.h
mac.o:buf.h
buf.o:buf.h
syscntrl.o:monitor.h
syscntrl.o:$H/func.h
monitor.h:$H/useful.h
mon_sql.o:monitor.h
mon_sql.o:$H/version.h
mon_sql.o:$H/opsys.h
mon_sql.o:$H/pv.h
mon_sql.o:$H/func.h
mon_nosql.o:monitor.h
mon_nosql.o:$H/version.h
mon_nosql.o:$H/opsys.h
mon_nosql.o:$H/pv.h
mon_nosql.o:$H/func.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above

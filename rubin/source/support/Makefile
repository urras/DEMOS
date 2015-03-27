#
#  Makefile for RUBIN support routines
#
#	@(#)Makefile	7.5	9/26/83
#

RUBIN=	../..
H=	../h
LIB=	$(RUBIN)/lib
BIN=	$(RUBIN)/bin
SYSBIN= /usr/local/bin

ALL=	creatdb destroydb helpr printr purge sysmod \
	restore usersetup rubin # printadmin
LOCK=   rubin_lock
SRCS=	creatdb.c destroydb.c error.c helpr.c rubin.y \
	printr.c purge.c subs.c sysmod.c usersetup.c \
	indconv.q restore.c copydb.q prvect.c printadmin.c \
	demodb.sh initsocket.c ildr.c

CTLMOD=	$(LIB)/ctlmod
IUTIL=	$(LIB)/iutil
GUTIL=	$(LIB)/gutil
DBULIB=	$(LIB)/dbulib
LIBS1=	$(GUTIL)
LIBS2=  $(IUTIL) $(IUTIL) $(GUTIL)
LIBS3=  $(DBULIB) $(IUTIL) $(IUTIL) $(GUTIL)
LIBQ =  $(LIB)/libq.a

CFLAGS=	-O -I$H 
EQUEL=	equel
LDFLAGS= -n
CP=     ln
CHMOD=	chmod
BINMODE=751
SUMODE=	4751
ROMODE=	444
SHMODE=	755
CHOWN=	-chown
GET=	sccs get
REL=
PR=	print
VGRIND=	csh /usr/ucb/vgrind
HEADER=	Support Programs
.SUFFIXES: .q

.q.c:
	$(EQUEL) $<

all: $(ALL)

copydb:	copydb.o $(LIBS1)
	$(CC) $(LDFLAGS) copydb.o $(LIBS1) $(LIBQ) -o  copydb
	$(CHMOD) $(BINMODE) copydb
	ls -l copydb; size copydb

copydb.c: copydb.q
	equel -d copydb.q

copydb5.c: copydb5.q
	equel5 copydb5.q

copydb5: copydb5.o $(LIBS2)
	$(CC) $(LDFLAGS) copydb5.o $(LIBS2) -lq5 -o copydb5
	$(CHMOD) $(BINMODE) copydb5

creatdb: creatdb.o error.o prvect.o $(LIBS3)
	$(CC) $(LDFLAGS) creatdb.o error.o prvect.o $(LIBS3) -o creatdb
	$(CHMOD) $(SUMODE) creatdb
	ls -l creatdb; size creatdb

destroydb: destroydb.o $(LIBS3)
	$(CC) $(LDFLAGS) destroydb.o $(LIBS3) -o destroydb
	$(CHMOD) $(SUMODE) destroydb
	ls -l destroydb; size destroydb

helpr:	helpr.o error.o prvect.o $(LIBS3)
	$(CC) $(LDFLAGS) helpr.o error.o prvect.o $(LIBS3) -o helpr
	$(CHMOD) $(SUMODE) helpr
	ls -l helpr; size helpr

indconv: indconv.o $(LIBS2)
	$(CC) indconv.o $(LIB)/equel $(LIBS2) -o indconv
	$(CHMOD) $(BINMODE) indconv
	ls -l indconv; size indconv

rubin: rubin.o $(LIBS2)
	$(CC) rubin.o $(LDFLAGS) $(LIBS2) -o rubin
	$(CHMOD) $(SUMODE) rubin
	ls -l rubin; size rubin
rubin.o: rubin.c
	$(CC) $(CFLAGS) -I../ctlmod -c rubin.c

rubin_lock: ildr.o initsocket.o
	$(CC) $(CFLAGS) -o rubin_lock ildr.o initsocket.o
	$(CHMOD) $(BINMODE) rubin_lock
	ls -l rubin_lock ; size rubin_lock

printadmin: printadmin.o $(LIBS2)
	$(CC) $(LDFLAGS) printadmin.o $(LIBS2) -o printadmin
	$(CHMOD) $(SUMODE) printadmin
	ls -l printadmin; size printadmin

printr: printr.o error.o prvect.o $(LIBS3)
	$(CC) $(LDFLAGS) printr.o error.o prvect.o $(LIBS3) -o printr
	$(CHMOD) $(SUMODE) printr
	ls -l printr; size printr

purge: purge.o subs.o error.o $(LIBS3)
	$(CC) $(LDFLAGS) purge.o subs.o error.o $(LIBS3) -o purge
	$(CHMOD) $(SUMODE) purge
	ls -l purge; size purge

restore: restore.o subs.o error.o $(LIBS3)
	$(CC) $(LDFLAGS) restore.o subs.o error.o $(LIBS3) -o restore
	-c7t10 restore
	$(CHMOD) $(SUMODE) restore
	ls -l restore; size restore

sysmod:	sysmod.o error.o prvect.o $(LIBS3) 
	$(CC) $(LDFLAGS) sysmod.o error.o prvect.o $(LIBS3) -o sysmod
	$(CHMOD) $(SUMODE) sysmod
	ls -l sysmod; size sysmod

usersetup: usersetup.o $(LIBS1)
	$(CC) $(LDFLAGS) usersetup.o $(LIBS1) -o usersetup
	$(CHMOD) 700 usersetup

new:
	rm -f $(ALL) $(LOCK)

clean:
	rm -f *.o
	rm -f $(ALL) $(LOCK) copydb.c copydb5.c indconv.c rubin.c a.out core y.tab.c

install: $(BIN)/creatdb $(BIN)/destroydb $(BIN)/helpr
install: $(BIN)/printr $(BIN)/purge $(BIN)/restore $(BIN)/sysmod
install: $(BIN)/usersetup $(BIN)/rubin $(BIN)/printadmin
install: $(BIN)/demodb
instlock: $(BIN)/rubin_lock

$(BIN)/creatdb: creatdb
	rm -f $@
	$(CP) creatdb $(BIN)/creatdb; $(CHMOD) $(SUMODE) $(BIN)/creatdb
$(BIN)/destroydb: destroydb
	rm -f $@
	$(CP) destroydb $(BIN)/destroydb; $(CHMOD) $(SUMODE) $(BIN)/destroydb
$(BIN)/helpr: helpr
	rm -f $@
	$(CP) helpr $(BIN)/helpr; $(CHMOD) $(SUMODE) $(BIN)/helpr
$(BIN)/indconv: indconv
	rm -f $@
	$(CP) indconv $(BIN)/indconv; $(CHMOD) $(BINMODE) $(BIN)/indconv
$(BIN)/printr: printr
	rm -f $@
	$(CP) printr $(BIN)/printr; $(CHMOD) $(SUMODE) $(BIN)/printr
$(BIN)/purge: purge
	rm -f $@
	$(CP) purge $(BIN)/purge; $(CHMOD) $(SUMODE) $(BIN)/purge
$(BIN)/restore: restore
	rm -f $@
	$(CP) restore $(BIN)/restore; $(CHMOD) $(SUMODE) $(BIN)/restore
$(BIN)/sysmod: sysmod
	rm -f $@
	$(CP) sysmod $(BIN)/sysmod; $(CHMOD) $(SUMODE) $(BIN)/sysmod
$(BIN)/usersetup: usersetup
	rm -f $@
	$(CP) usersetup $(BIN)/usersetup; $(CHMOD) 700 $(BIN)/usersetup
$(BIN)/rubin: rubin
	rm -f $@
	$(CP) rubin $(BIN)/rubin; $(CHMOD) $(SUMODE) $(BIN)/rubin
$(BIN)/printadmin: printadmin
	rm -f $@
	$(CP) printadmin $(BIN)/printadmin; $(CHMOD) $(SUMODE) $(BIN)/printadmin
$(BIN)/demodb: demodb.sh
	cp    demodb.sh $(BIN)/demodb; $(CHMOD) $(SHMODE) $(BIN)/demodb

$(BIN)/rubin_lock: rubin_lock
	rm -f $@
	cp rubin_lock $(BIN)/rubin_lock ; $(CHMOD) $(BINMODE) $(BIN)/rubin_lock
	rm -f $(BIN)/drubin_lock;ln $(BIN)/rubin_lock $(BIN)/drubin_lock

sysinstall:
sysinstall: $(SYSBIN)/rubin $(SYSBIN)/creatdb $(SYSBIN)/destroydb $(SYSBIN)/demodb
sysinstall: $(SYSBIN)/sysmod $(SYSBIN)/purge $(SYSBIN)/restore
sysinstall: $(SYSBIN)/printr $(SYSBIN)/helpr
# sysinstall: $(SYSBIN)/printr $(SYSBIN)/helpr $(SYSBIN)/rubin_lock

$(SYSBIN)/rubin: rubin
	$(CP) rubin $(SYSBIN)/rubin
	$(CHOWN) rubin $(SYSBIN)/rubin; $(CHMOD) $(SUMODE) $(SYSBIN)/rubin
$(SYSBIN)/creatdb: creatdb
	$(CP) creatdb $(SYSBIN)/creatdb
	$(CHOWN) rubin $(SYSBIN)/creatdb; $(CHMOD) $(SUMODE) $(SYSBIN)/creatdb
$(SYSBIN)/destroydb: destroydb
	$(CP) destroydb $(SYSBIN)/destroydb
	$(CHOWN) rubin $(SYSBIN)/destroydb; $(CHMOD) $(SUMODE) $(SYSBIN)/destroydb
$(SYSBIN)/demodb: demodb.sh
	$(CP) demodb.sh $(SYSBIN)/demodb
	$(CHOWN) rubin $(SYSBIN)/demodb; $(CHMOD) $(SHMODE) $(SYSBIN)/demodb
$(SYSBIN)/sysmod: sysmod
	$(CP) sysmod $(SYSBIN)/sysmod
	$(CHOWN) rubin $(SYSBIN)/sysmod; $(CHMOD) $(SUMODE) $(SYSBIN)/sysmod
$(SYSBIN)/purge: purge
	$(CP) purge $(SYSBIN)/purge
	$(CHOWN) rubin $(SYSBIN)/purge; $(CHMOD) $(SUMODE) $(SYSBIN)/purge
$(SYSBIN)/restore: restore
	$(CP) restore $(SYSBIN)/restore
	$(CHOWN) rubin $(SYSBIN)/restore; $(CHMOD) $(SUMODE) $(SYSBIN)/restore
$(SYSBIN)/printr: printr
	$(CP) printr $(SYSBIN)/printr
	$(CHOWN) rubin $(SYSBIN)/printr; $(CHMOD) $(SUMODE) $(SYSBIN)/printr
$(SYSBIN)/helpr: helpr
	$(CP) helpr $(SYSBIN)/helpr
	$(CHOWN) rubin $(SYSBIN)/helpr; $(CHMOD) $(SUMODE) $(SYSBIN)/helpr
$(SYSBIN)/rubin_lock: rubin_lock
	$(CP) rubin_lock $(SYSBIN)/rubin_lock ; $(CHMOD) $(BINMODE) $(SYSBIN)/rubin_lock

sources: $(SRCS)

# $(SRCS):
#         $(GET) $(REL) SCCS/s.$@

print: sources
	$(PR) Makefile TraceFlags *.[hm] *.[csy]

vgrind: sources
	cp /dev/null index
	$(VGRIND) -h "$(HEADER) (Release 7)" -n Makefile TraceFlags
	$(VGRIND) -h "$(HEADER) (Release 7)" *.[hm] *.[csy]
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

creatdb.o:$H/defines.h $H/local.h
creatdb.o:$H/aux.h
creatdb.o:$H/catalog.h
creatdb.o:$H/version.h
creatdb.o:$H/access.h
creatdb.o:$H/symbol.h
creatdb.o:$H/opsys.h
creatdb.o:$H/pv.h
destroydb.o:$H/defines.h $H/local.h
destroydb.o:$H/aux.h
destroydb.o:$H/access.h
error.o:$H/defines.h $H/local.h
error.o:$H/aux.h
helpr.o:$H/defines.h $H/local.h
helpr.o:$H/aux.h
helpr.o:$H/access.h
helpr.o:$H/lock.h
helpr.o:$H/pv.h
rubin.y:$H/defines.h $H/local.h
rubin.y:$H/aux.h
rubin.y:$H/version.h
rubin.y:$H/access.h
rubin.y:$H/lock.h
rubin.y:$H/opsys.h
printr.o:$H/defines.h $H/local.h
printr.o:$H/aux.h
printr.o:$H/access.h
printr.o:$H/lock.h
printr.o:$H/pv.h
purge.o:$H/defines.h $H/local.h
purge.o:$H/aux.h
purge.o:$H/lock.h
purge.o:$H/pv.h
purge.o:$H/opsys.h
purge.o:$H/access.h
purge.o:$H/code.h
subs.o:$H/defines.h $H/local.h
subs.o:$H/aux.h
subs.o:$H/access.h
subs.o:$H/lock.h
subs.o:$H/opsys.h
sysmod.o:$H/defines.h $H/local.h
sysmod.o:$H/aux.h
sysmod.o:$H/lock.h
sysmod.o:$H/access.h
sysmod.o:$H/pv.h
usersetup.o:$H/defines.h $H/local.h
usersetup.o:$H/aux.h
usersetup.o:$H/opsys.h
restore.o:$H/defines.h $H/local.h
restore.o:$H/aux.h
restore.o:$H/catalog.h
restore.o:$H/access.h
restore.o:$H/opsys.h
restore.o:$H/lock.h
restore.o:$H/symbol.h
restore.o:$H/resp.h
restore.o:$H/code.h
prvect.o:$H/tree.h
prvect.o:$H/pv.h
printadmin.o:$H/defines.h $H/local.h
printadmin.o:$H/access.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above

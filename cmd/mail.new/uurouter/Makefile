#
# Inernet to UUCP address converting software
#
# $Header: Makefile,v 1.1 90/09/01 15:22:03 avg Exp $
# $Log:	Makefile,v $
# Revision 1.1  90/09/01  15:22:03  avg
# Initial revision
# 
#

RLIB=/usr/local/lib/relcom
RBIN=/usr/local/bin

#
# Set R=X if you have sources without RSC archive files
#
R=X
RCS=RCS
COFLAGS=-q

SHELL=/bin/sh
D=.
SD=.
SM=Y
UU=Y
CF=
MODE=


all: $(D)/uumail-sm $(D)/uumail-nosm $(D)/uu-router $(D)/uu-meta

$(D)/uumail-sm: $(D)/uumail-sm.o $(D)/bsearch.o $(D)/date.o
	cd $(D); $(CC) $(MODE) -O -o uumail-sm $(SEP) uumail-sm.o bsearch.o date.o

$(D)/uumail-nosm: $(D)/uumail-nosm.o $(D)/bsearch.o $(D)/date.o
	cd $(D); $(CC) $(MODE) -O -o uumail-nosm $(SEP) uumail-nosm.o bsearch.o date.o

$(D)/uumail-sm.o: uumail.c serv.h
	cd $(D); $(CC) $(MODE) -c -O $(OPTIONS) -I$(SD) $(SD)/uumail.c && \
	mv uumail.o uumail-sm.o

$(D)/uumail-nosm.o: uumail.c serv.h
	cd $(D); $(CC) $(MODE) -c -O $(OPTIONS) -I$(SD) -DNOSM $(SD)/uumail.c && \
	mv uumail.o uumail-nosm.o

$(D)/date.o: date.c
	cd $(D); $(CC) $(MODE) -c -O $(OPTIONS) $(SD)/date.c

$(D)/bsearch.o: bsearch.c
	cd $(D); $(CC) $(MODE) -c -O $(OPTIONS) $(SD)/bsearch.c

$(D)/uu-meta: uu-meta.c serv.h
	cd $(D); $(CC) $(MODE) -O -o uu-meta $(OPTIONS) $(SD)/uu-meta.c

$(D)/uu-router: $(D)/rmain.o $(D)/rsort.o
	cd $(D); $(CC) $(MODE) -O -o uu-router $(SEP) rmain.o rsort.o

$(D)/rsort.o: rsort.c def.h
	cd $(D); $(CC) $(MODE) -c -O $(OPTIONS) $(SD)/rsort.c

$(D)/rmain.o: rmain.c def.h
	cd $(D); $(CC) $(MODE) -c -O $(OPTIONS) $(SD)/rmain.c

#
# INSTALL SECTION
#

install: ${CF}mkall $(SM)tcpinst $(UU)uucpinst

Ytcpinst ytcpinst:
	cp $(D)/uu-meta $(RLIB)/uu-meta
	chown bin $(RLIB)/uu-meta
	chgrp bin $(RLIB)/uu-meta
	chmod 755 $(RLIB)/uu-meta
	cp $(D)/uumail-sm $(RLIB)/uumail

Ntcpinst ntcpinst:
	cp $(D)/uumail-nosm $(RLIB)/uumail

Yuucpinst yuucpinst:
	chown uucp $(RLIB)/uumail
	chgrp uucp $(RLIB)/uumail
	chmod 4755 $(RLIB)/uumail
	rm -f $(RBIN)/rmail
	ln $(RLIB)/uumail $(RBIN)/rmail
	cp $(D)/uu-router $(RLIB)/uu-router
	chown uucp $(RLIB)/uu-router
	chgrp uucp $(RLIB)/uu-router
	chmod 4755 $(RLIB)/uu-router

Nuucpinst nuucpinst:
	rm -f $(RLIB)/uumail

mkall:  all
Cmkall:

#
# Handling RCS archives
#
#$(R)rmain.c:    $(RCS)/rmain.c,v ;      co $(COFLAGS) rmain.c
#$(R)rsort.c:    $(RCS)/rsort.c,v ;      co $(COFLAGS) rsort.c
#$(R)def.h:      $(RCS)/def.h,v ;        co $(COFLAGS) def.h
#$(R)uumail.c:   $(RCS)/uumail.c,v ;     co $(COFLAGS) uumail.c
#$(R)bsearch.c:  $(RCS)/bsearch.c,v ;    co $(COFLAGS) bsearch.c
#$(R)uu-meta.c:  $(RCS)/uu-meta.c,v ;    co $(COFLAGS) uu-meta.c
#$(R)serv.h:     $(RCS)/serv.h,v ;       co $(COFLAGS) serv.c
#$(R)Makefile:   $(RCS)/Makefile,v ;     co $(COFLAGS) Makefile

#
# Sending and receiving batched mail
#
# $Header: Makefile,v 1.1 90/12/10 16:53:47 avg Exp $
# $Log:	Makefile,v $
# Revision 1.1  90/12/10  16:53:47  avg
# Initial revision
# 
#

RLIB=/usr/lib/relcom
RBIN=/usr/bin

CFLAGS=-O

#
# Set R=X if you have sources without RCS archive files
#
R=X
RCS=RCS
COFLAGS=-q

SHELL=/bin/sh
D=.
SD=.
CF=

UU=y

all: $(D)/batchmail $(D)/rbmail

$(D)/batchmail: batchmail.c
	cd $(D); $(CC) $(CFLAGS) -o batchmail $(SD)/batchmail.c

$(D)/rbmail: rbmail.c
	cd $(D); $(CC) $(CFLAGS) -o rbmail $(SD)/rbmail.c

install: $(UU)inst

Ninst ninst:

Yinst yinst: ${CF}mkall
	cp $(D)/rbmail $(RBIN)/rbmail
	chown uucp $(RBIN)/rbmail
	chgrp daemon $(RBIN)/rbmail
	chmod 4755 $(RBIN)/rbmail
	cp $(D)/batchmail $(RLIB)/batchmail
	chown uucp $(RLIB)/batchmail
	chgrp daemon $(RLIB)/batchmail
	chmod 4755 $(RLIB)/batchmail
	-@set -x; \
	[ -d /usr/spool/batchmail ] || \
		( mkdir /usr/spool/batchmail ; \
		  chmod 700 /usr/spool/batchmail ; \
		  chown uucp /usr/spool/batchmail )


mkall:  all
Cmkall:

#
# Handling RCS archives
#
$(R)batchmail.c: $(RCS)/batchmail.c,v ;  co $(COFLAGS) batchmail.c
$(R)rbmail.c:    $(RCS)/rbmail.c,v ;     co $(COFLAGS) rbmail.c
$(R)Makefile:    $(RCS)/Makefile,v ;     co $(COFLAGS) Makefile

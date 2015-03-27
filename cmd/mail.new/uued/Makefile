#
# Sending and receiving files through E-mail
#
# $Header: Makefile,v 1.1 90/08/15 20:57:18 avg Exp $
# $Log:	Makefile,v $
# Revision 1.1  90/08/15  20:57:18  avg
# Initial revision
# 
#

RLIB=/usr/local/lib/relcom
RBIN=/usr/local/bin

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

all: $(D)/uu-decode $(D)/mto mpick

$(D)/uu-decode: uu-decode.c ../Dispose.h
	cd $(D); $(CC) $(OPTIONS) -O -o uu-decode $(SD)/uu-decode.c

$(D)/mto: mto.c
	cd $(D); $(CC) $(OPTIONS) -O -o mto $(SD)/mto.c

install: ${CF}mkall
	cp $(D)/mto $(RBIN)/mto
	chown bin $(RBIN)/mto
	chgrp bin $(RBIN)/mto
	chmod 755 $(RBIN)/mto
	cp mpick $(RBIN)/mpick
	chown bin $(RBIN)/mpick
	chgrp bin $(RBIN)/mpick
	chmod 755 $(RBIN)/mpick
	cp $(D)/uu-decode $(RLIB)/uu-decode
	chown root $(RLIB)/uu-decode
	chgrp 0    $(RLIB)/uu-decode
	chmod 4755 $(RLIB)/uu-decode

mkall:  all
Cmkall:

#
# Handling RCS archives
#
#$(R)uu-decode.c: $(RCS)/uu-decode.c,v ;  co $(COFLAGS) uu-decode.c
#$(R)mto.c:       $(RCS)/mto.c,v ;        co $(COFLAGS) mto.c
#$(R)mpick:       $(RCS)/mpick,v ;        co $(COFLAGS) mpick
#$(R)Makefile:    $(RCS)/Makefile,v ;     co $(COFLAGS) Makefile

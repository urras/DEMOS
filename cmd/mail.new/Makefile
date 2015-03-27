#
# RELCOM network software
#
# General Makefile
#
# $Header$
# $Log$
#

all:    mkall

RD=relcom
SHELL=/bin/sh
MAKE=make

# Set R=X if you have no RCS archives
R=X


MACHINES=pdp vax x286 x386 ix386 dos
OBJDIRS =PDP VAX X286 X386 IX386 DOS EXCELAN


DIRS=   mailx uurouter uued sendmail

D_PDP:  ; @echo mailx uurouter uued
D_DEMOS:; @echo mailx uurouter uued
D_VAX:  ; @echo mailx uurouter uued sendmail
D_X286: ; @echo mailx uurouter uued
D_X386: ; @echo mailx uurouter uued
D_IX386:; @echo mailx uurouter uued sendmail
D_DOS:  ; @echo mailx

SM=N
UU=Y
CF=C
Q=
ORG=

V= SM=$(SM) UU=$(UU) CF=$(CF) ORG=\"$(ORG)\" R=$(R) D=$$CPU

#
# Make all that stuff
#
mkall:; @. ./Ask-CPU ; \
	xdirs="`$(MAKE) D_$$CPU`"; \
	$(MAKE) XDIRS="$$xdirs" make_all_for_cpu

make_all_for_cpu:
	@for i in $(XDIRS) ; \
	 do (echo ==== $$i ======; cd $$i; make $(V) all); \
	 done

clean:
	-rm -f MREP */MREP
	-@for i in $(DIRS) sendmail; do (cd $$i ; rm -rf $(OBJDIRS)) ; done

install:; @. ./Ask-CPU ; \
	xdirs="`$(MAKE) D_$$CPU`"; \
	echo -n "Is the TCP/IP installed on your machine [y/n] ? " ; read tcp ; \
	echo -n "Has your machine UUCP links [y/n] ? " ; read uucp ; \
	echo "Enter the full name of your organization:" ; echo -n "> " ; read org ; \
	$(MAKE) XDIRS="$$xdirs" SM=$$tcp UU=$$uucp "ORG=\"$$org\"" make_copy

copy:; @. ./Ask-CPU ; \
	xdirs="`$(MAKE) D_$$CPU`"; \
	echo -n "Is the TCP/IP installed on your machine [y/n] ? " ; read tcp ; \
	echo -n "Has your machine UUCP links [y/n] ? " ; read uucp ; \
	echo "Enter the full name of your organization:" ; echo -n "> " ; read org ; \
	$(MAKE) R=X CF=C XDIRS="$$xdirs" SM=$$tcp UU=$$uucp "ORG=\"$$org\"" make_copy

make_copy:
	@for i in $(XDIRS) ; \
	 do (echo ==== $$i ======; cd $$i; make $(V) install;) \
	 done

tarit:; @. ./Ask-CPU ; \
	xdirs="`$(MAKE) D_$$CPU`"; \
	$(MAKE) XDIRS="$$xdirs" make_tarit

make_tarit:
	> .tarlist
	for i in . $(XDIRS) ; \
	do \
	    sed -e "s?@?$${CPU}?; s?^?${RD}/$$i/?" $$i/TarFiles >> .tarlist; \
	done
	@echo -n "Enter the name of the dest. file: " ; read dest ; \
	cd .. ; set -x ; \
	tar cvFf ${RD}/.tarlist $$dest
	@rm -f .tarlist

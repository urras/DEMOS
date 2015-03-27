#
# RELCOM Mailer
#

all: mkall

RLIB=/usr/lib/relcom
RBIN=/usr/bin

ORG=unknown
MAKE=make

#
# Set R=X if you have sources without RSC archive files
#
R=X
RCS=RCS
COFLAGS=-q

SHELL=/bin/sh

MISCS=Mail.rc Mail.help Mail.help.~ Mail.rhelp Mail.rhelp.~

# default dirs
SD=.
D=X286
SM=N
CF=

OBJS=$(D)/maux.o    $(D)/cmd1.o   $(D)/cmd2.o    $(D)/cmd3.o $(D)/cmdtab.o \
     $(D)/collect.o $(D)/config.o $(D)/edit.o    $(D)/fio.o $(D)/getconf.o \
     $(D)/getname.o $(D)/head.o   $(D)/local.o   $(D)/lock.o \
     $(D)/lex.o     $(D)/list.o   $(D)/main.o    $(D)/names.o $(D)/optim.o \
     $(D)/quit.o    $(D)/send.o   $(D)/strings.o $(D)/temp.o \
     $(D)/tty.o     $(D)/vars.o   $(D)/date.o
# $(D)/popen.o

SRCS=maux.c cmd1.c cmd2.c cmd3.c cmdtab.c collect.c config.c edit.c fio.c \
     getconf.c getname.c head.c local.c lock.c lex.c list.c main.c \
     names.c optim.c quit.c send.c strings.c temp.c tty.c vars.c date.c
# popen.c

OSRCS=fmt.c mail.c

HDRS=rcv.h confdefs.h def.h glob.h local.h

S = $(SRCS) $(HDRS)

VAX_OPTS:       ; @echo "SEP= OPTIONS=-DVMUNIX"
X386_OPTS:      ; @echo "SEP= LIBS="
X286_OPTS:      ; @echo "SEP='-i -F 3000' MODE=-Ml CC='cc -LARGE -M2'"
IX386_OPTS:     ; @echo "SEP= OPTIONS=-DM_SYSV"
PDP_OPTS:       ; @echo "SEP=-i OVLQ=-Q OVLL=-L"

LDFLG=-s
OPTIM=-O

CFLAGS=$(OPTIM) ${OPTIONS} ${MODE}

COMP=cd $(D); $(CC) -c -I$(SD) $(CFLAGS)


mkall:; @ . ../Ask-CPU ; \
	xopts="`$(MAKE) $${CPU}_OPTS`"; \
	eval "$(MAKE) SD=.. D=$$CPU $$xopts MODE=$$mode cpu_dir make_all_for_cpu"

make_all_for_cpu: ${D}/localmail
make_all_for_cpu: ${D}/mailx-sm ${D}/mailx-nosm
make_all_for_cpu: ${D}/fmt

localmail: ; @ . ../Ask-CPU ; \
	xopts="`$(MAKE) $${CPU}_OPTS`"; \
	eval "$(MAKE) SD=.. D=$$CPU $$xopts MODE=$$mode cpu_dir $(D)/localmail"

mailx:     ; @ . ../Ask-CPU ; \
	xopts="`$(MAKE) $${CPU}_OPTS`"; \
	eval "$(MAKE) SD=.. D=$$CPU $$xopts MODE=$$mode cpu_dir $(D)/mailx-sm $(D)/mailx-nosm"

fmt:       ; @ . ../Ask-CPU ; \
	xopts="`$(MAKE) $${CPU}_OPTS`"; \
	eval "$(MAKE) SD=.. D=$$CPU $$xopts MODE=$$mode cpu_dir $(D)/fmt"


$(D)/mailx-sm:  $(OBJS) $(D)/sndmailN.o
	${CC} -o $(D)/mailx-sm   $(LDFLG) $(MODE) $(SEP) $(OVLQ) $(OBJS) $(D)/sndmailN.o $(OVLL) $(LIBS) $(OVLQ)
	size ${D}/mailx-sm

$(D)/mailx-nosm:  $(OBJS) $(D)/uumailN.o
	${CC} -o $(D)/mailx-nosm $(LDFLG) $(MODE) $(SEP) $(OVLQ) $(OBJS) $(D)/uumailN.o $(OVLL) $(LIBS) $(OVLQ)
	size ${D}/mailx-nosm

$(D)/fmt:  fmt.c head.c ${HDRS}
	@cp head.c _head.c
	cd ${D} ; ${CC} -o fmt $(LDFLG) $(OPTIONS) $(OPTIM) -I$(SD) $(SD)/fmt.c $(SD)/_head.c
	@rm -f _head.c
	size ${D}/fmt

$(D)/localmail:  mail.c
	cd ${D} ; ${CC} -o localmail $(LDFLG) $(OPTIONS) $(OPTIM) -I$(SD) ${SD}/mail.c
	size ${D}/localmail

$(OBJS):        $(HDRS)


cpu_dir:
	@-mkdir $$CPU >/dev/null 2>&1 ; exit 0

CLRSRC: clean
	${RM} $S $(OSRCS)
	${RM} tags *.b

source: $S Makefile ${OSRCS}

tags:   $(SRCS) $(HDRS)
	ctags $(HDRS) $(SRCS)

#
# INSTALL SECTION
#
install: ${CF}mkall ${SM}imail
	rm -f ${RBIN}/mail
	ln ${RBIN}/mailx ${RBIN}/mail
	chown bin $(RBIN)/mailx
	chgrp 10 $(RBIN)/mailx
	chmod 755 $(RBIN)/mailx
	cd misc ; cp $(MISCS) $(RLIB)
	cd $(RLIB) ; \
		chown root $(MISCS) ; \
		chgrp 0 $(MISCS) ; \
		chmod 644 $(MISCS)
	cp ${D}/fmt ${RBIN}/fmt
	chown bin $(RBIN)/fmt
	chgrp 10 $(RBIN)/fmt
	chmod 755 $(RBIN)/fmt
	cp ${D}/localmail ${RLIB}/localmail
	chown root $(RLIB)/localmail
	chgrp 0    $(RLIB)/localmail
	chmod 4755 $(RLIB)/localmail
#	(echo '1,$$s?=UNKNOWN=?$(ORG)?'; echo w ; echo q) | ed - $(RLIB)/Mail.rc

Yimail yimail:
	cp ${D}/mailx-sm ${RBIN}/mailx

Nimail nimail:
	cp ${D}/mailx-nosm ${RBIN}/mailx

Cmkall:

##### COMPILING #####
$(D)/cmd1.o      : cmd1.c      ; $(COMP) ${SD}/cmd1.c
$(D)/cmd2.o      : cmd2.c      ; $(COMP) ${SD}/cmd2.c
$(D)/cmd3.o      : cmd3.c      ; $(COMP) ${SD}/cmd3.c
$(D)/cmdtab.o    : cmdtab.c    ; $(COMP) ${SD}/cmdtab.c
$(D)/collect.o   : collect.c   ; $(COMP) ${SD}/collect.c
$(D)/config.o    : config.c    ; $(COMP) ${SD}/config.c
$(D)/edit.o      : edit.c      ; $(COMP) ${SD}/edit.c
$(D)/fio.o       : fio.c       ; $(COMP) ${SD}/fio.c
$(D)/getconf.o   : getconf.c   ; $(COMP) ${SD}/getconf.c
$(D)/getname.o   : getname.c   ; $(COMP) ${SD}/getname.c
$(D)/head.o      : head.c      ; $(COMP) ${SD}/head.c
$(D)/local.o     : local.c     ; $(COMP) ${SD}/local.c
$(D)/lock.o      : lock.c      ; $(COMP) ${SD}/lock.c
$(D)/lex.o       : lex.c       ; $(COMP) ${SD}/lex.c
$(D)/list.o      : list.c      ; $(COMP) ${SD}/list.c
$(D)/main.o      : main.c      ; $(COMP) ${SD}/main.c
$(D)/maux.o      : maux.c      ; $(COMP) ${SD}/maux.c
$(D)/names.o     : names.c     ; $(COMP) ${SD}/names.c
$(D)/optim.o     : optim.c     ; $(COMP) ${SD}/optim.c
#$(D)/popen.o     : popen.c     ; $(COMP) ${SD}/popen.c
$(D)/quit.o      : quit.c      ; $(COMP) ${SD}/quit.c
$(D)/send.o      : send.c      ; $(COMP) ${SD}/send.c
$(D)/strings.o   : strings.c   ; $(COMP) ${SD}/strings.c
$(D)/temp.o      : temp.c      ; $(COMP) ${SD}/temp.c
$(D)/tty.o       : tty.c       ; $(COMP) ${SD}/tty.c
$(D)/vars.o      : vars.c      ; $(COMP) ${SD}/vars.c
$(D)/date.o      : date.c      ; $(COMP) ${SD}/date.c
$(D)/sndmailN.o  : sndmailN.c  ; $(COMP) ${SD}/sndmailN.c
$(D)/uumailN.o   : uumailN.c   ; $(COMP) ${SD}/uumailN.c

#### EXTRACTING FROM RCS ####
$(R)cmd1.c:         $(RCS)/cmd1.c,v ;           co $(COFLAGS) cmd1.c
$(R)cmd2.c:         $(RCS)/cmd2.c,v ;           co $(COFLAGS) cmd2.c
$(R)cmd3.c:         $(RCS)/cmd3.c,v ;           co $(COFLAGS) cmd3.c
$(R)cmdtab.c:       $(RCS)/cmdtab.c,v ;         co $(COFLAGS) cmdtab.c
$(R)collect.c:      $(RCS)/collect.c,v ;        co $(COFLAGS) collect.c
$(R)config.c:       $(RCS)/config.c,v ;         co $(COFLAGS) config.c
$(R)confdefs.h:     $(RCS)/confdefs.h,v ;       co $(COFLAGS) confdefs.h
$(R)def.h:          $(RCS)/def.h,v ;            co $(COFLAGS) def.h
$(R)edit.c:         $(RCS)/edit.c,v ;           co $(COFLAGS) edit.c
$(R)fio.c:          $(RCS)/fio.c,v ;            co $(COFLAGS) fio.c
$(R)fmt.c:          $(RCS)/fmt.c,v ;            co $(COFLAGS) fmt.c
$(R)getconf.c:      $(RCS)/getconf.c,v ;        co $(COFLAGS) getconf.c
$(R)getname.c:      $(RCS)/getname.c,v ;        co $(COFLAGS) getname.c
$(R)glob.h:         $(RCS)/glob.h,v ;           co $(COFLAGS) glob.h
$(R)head.c:         $(RCS)/head.c,v ;           co $(COFLAGS) head.c
$(R)lex.c:          $(RCS)/lex.c,v ;            co $(COFLAGS) lex.c
$(R)list.c:         $(RCS)/list.c,v ;           co $(COFLAGS) list.c
$(R)local.c:        $(RCS)/local.c,v ;          co $(COFLAGS) local.c
$(R)local.h:        $(RCS)/local.h,v ;          co $(COFLAGS) local.h
$(R)lock.c:         $(RCS)/lock.c,v ;           co $(COFLAGS) lock.c
$(R)main.c:         $(RCS)/main.c,v ;           co $(COFLAGS) main.c
$(R)mail.c:         $(RCS)/mail.c,v ;           co $(COFLAGS) mail.c
$(R)maux.c:         $(RCS)/maux.c,v ;           co $(COFLAGS) maux.c
$(R)names.c:        $(RCS)/names.c,v ;          co $(COFLAGS) names.c
$(R)optim.c:        $(RCS)/optim.c,v ;          co $(COFLAGS) optim.c
#$(R)popen.c:        $(RCS)/popen.c,v ;          co $(COFLAGS) popen.c
$(R)quit.c:         $(RCS)/quit.c,v ;           co $(COFLAGS) quit.c
$(R)rcv.h:          $(RCS)/rcv.h,v ;            co $(COFLAGS) rcv.h
$(R)send.c:         $(RCS)/send.c,v ;           co $(COFLAGS) send.c
$(R)strings.c:      $(RCS)/strings.c,v ;        co $(COFLAGS) strings.c
$(R)temp.c:         $(RCS)/temp.c,v ;           co $(COFLAGS) temp.c
$(R)tty.c:          $(RCS)/tty.c,v ;            co $(COFLAGS) tty.c
$(R)vars.c:         $(RCS)/vars.c,v ;           co $(COFLAGS) vars.c
$(R)date.c:         $(RCS)/date.c,v ;           co $(COFLAGS) date.c
$(R)sndmailN.c:     $(RCS)/sndmailN.c,v ;       co $(COFLAGS) sndmailN.c
$(R)uumailN.c:      $(RCS)/uumailN.c,v ;        co $(COFLAGS) uumailN.c
$(R)Makefile:       $(RCS)/Makefile,v ;         co $(COFLAGS) Makefile

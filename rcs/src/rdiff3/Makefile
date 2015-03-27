
# $Header: /usr/users/korotay/DEMOS_2.2/rcs/src/rdiff3/RCS/Makefile,v 1.2 1988/09/21 22:25:58 avg Exp $
# Команда merge использует /usr/rcs/lib/rdiff3

CFLAGS        = -O
COFLAGS=

TOP           = $(HOME)/d22

DEST          = $(TOP)/lib/rcs

EXTHDRS       =

HDRS          =

LDFLAGS       =

LIBS          =

LINKER        = cc

MAKEFILE      = Makefile

OBJS          = diff3.o

PRINT         = pr

PROGRAM       = rdiff3

SRCS          = diff3.c

all:            $(PROGRAM)

$(PROGRAM):     $(OBJS) $(LIBS)
		@echo -n "Loading $(PROGRAM) ... "
		@$(LINKER) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)
		@echo "done"

clean clear:;   @rm -f $(OBJS)

CLRSRC:;        -rm -f *.[cob]

depend:;        @mkmf -f $(MAKEFILE) PROGRAM=$(PROGRAM) DEST=$(DEST)

install:        $(PROGRAM)
		@echo Installing $(PROGRAM) in $(DEST)
		-rm -f $(DEST)/$(PROGRAM)
		cp $(PROGRAM) $(DEST)/$(PROGRAM)

print:;         @$(PRINT) $(HDRS) $(SRCS)

program:        $(PROGRAM)

update:         $(DEST)/$(PROGRAM)

$(DEST)/$(PROGRAM): $(SRCS) $(LIBS) $(HDRS) $(EXTHDRS)
		@make -f $(MAKEFILE) DEST=$(DEST) install

diff3.c:        RCS/diff3.c,v ; co $(COFLAGS) diff3.c

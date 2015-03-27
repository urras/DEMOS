# $Header: /usr/users/korotay/DEMOS_2.2/rcs/src/rdiff/RCS/Makefile,v 1.6 1988/09/21 22:25:01 avg Exp $
# система RCS использует /usr/rcs/lib/rdiff и /usr/lib/diffh

CFLAGS = -O -DDIFF='"${DIFF}"' -DDIFFH='"${DIFFH}"' -DPR='"${PR}"'  ${TAGE}
COFLAGS=

TOP           = $(HOME)/d22
#TOP          = /usr

# Для машин типа Электроника 82, СМ 1700 и VAX снять комментарий!!!

TAGE          = -DE82		# here

DEST          = $(TOP)/lib/rcs

DIFF          = /usr/bin/diff

DIFFH         = $(TOP)/usr/lib/diffh

EXTHDRS       =

HDRS          = diff.h

LDFLAGS       =

LIBS          =

LINKER        = cc

MAKEFILE      = Makefile

OBJS          = diff.o \
		diffreg.o

PRINT         = pr

PROGRAM       = rdiff

SRCS          = diff.c \
		diffreg.c

all:            $(PROGRAM)

$(PROGRAM):     $(OBJS) $(LIBS)
		@echo -n "Loading $(PROGRAM) ... "
		@$(LINKER) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)
		@echo "done"

$(OBJS):        $(HDRS)

clean clear:;   @rm -f $(OBJS)

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

CLRSRC:;        -rm *.[cobh]

# Вытаскивание версии
diff.c:         RCS/diff.c,v;     co $(COFLAGS) diff.c
diffreg.c:      RCS/diffreg.c,v;  co $(COFLAGS) diffreg.c
diff.h:         RCS/diff.h,v;     co $(COFLAGS) diff.h

#
#  Makefile for RUBIN EQUEL precompiler
#
#	@(#)Makefile	7.2	10/27/81
#

RUBIN=	../..
H=	../h
LIB=	$(RUBIN)/lib
BIN=	$(RUBIN)/bin
SYSBIN=	/usr/local
SHELL=/bin/sh

OBJS= 	main.o yylex.o grammar.o cvar.o include.o name.o getkey.o \
	number.o operator.o prtout.o string.o comment.o retrieve.o display.o \
	getch.o cmap.o yyerror.o nalloc.o 
SRCS= 	main.c yylex.c grammar.y cvar.c include.c name.c getkey.c \
	number.c operator.c prtout.c string.c comment.c retrieve.c display.c \
	getch.c cmap.c yyerror.c nalloc.c constants.h globals.h tokens.y

GUTIL=	$(LIB)/gutil
# include -lm on the VAX
LIBS=	$(GUTIL) -lm

CFLAGS= -I$H -O
LDFLAGS=
# YFLAGS=
YFLAGS= -d
GET=	sccs get
REL=
PR=	pr
VGRIND=	csh /usr/ucb/vgrind
HEADER=	Equel Precompiler
CP=	cp
CHMOD=	chmod
CHOWN=	-chown
BINMODE=755

equel: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o equel
	$(CHMOD) $(BINMODE) equel
	ls -l equel; size equel

new:
	rm -f equel

clean:
	rm -f *.o
	rm -f a.out y.tab.c core equel

install: $(BIN)/equel

$(BIN)/equel: equel
	$(CP) equel $(BIN)/equel

sysinstall: equel
	$(CP) equel $(SYSBIN)/equel
	$(CHOWN) rubin $(SYSBIN)/equel

print: sources
	$(PR) Makefile TraceFlags *.[hm] *.[csy]

vgrind: sources
	cp /dev/null index
	$(VGRIND) -h "$(HEADER) (Release 7)" -n Makefile TraceFlags
	$(VGRIND) -h "$(HEADER) (Release 7)" *.[hm] *.[csy]
	$(VGRIND) -h "$(HEADER) (Release 7)" -x index

sources: $(SRCS)

# $(SRCS):
#         $(GET) $(REL) SCCS/s.$@

y.tab.h: grammar.c

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

main.o:constants.h
main.o:globals.h
yylex.o:constants.h
yylex.o:globals.h
yylex.o:y.tab.h
grammar.o:constants.h
grammar.o:globals.h
grammar.o:tokens.y
cvar.o:constants.h
cvar.o:globals.h
include.o:constants.h
include.o:globals.h
name.o:constants.h
name.o:globals.h
name.o:y.tab.h
getkey.o:constants.h
getkey.o:globals.h
number.o:constants.h
number.o:globals.h
number.o:y.tab.h
operator.o:constants.h
operator.o:globals.h
operator.o:y.tab.h
prtout.o:constants.h
prtout.o:globals.h
string.o:constants.h
string.o:globals.h
string.o:y.tab.h
comment.o:constants.h
comment.o:globals.h
retrieve.o:constants.h
retrieve.o:globals.h
display.o:constants.h
display.o:globals.h
getch.o:constants.h
getch.o:globals.h
cmap.o:constants.h
yyerror.o:constants.h
yyerror.o:globals.h
yyerror.o:y.tab.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above

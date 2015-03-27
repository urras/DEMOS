#
# $Log:	Makefile,v $
# Revision 1.5  90/09/24  12:57:34  root
# overlay cpp
# 
# Revision 1.4  88/09/29  22:15:51  dvolodin
# Вставлено вытаскивание _rofix из RCS.
#
# Revision 1.3  88/09/29  22:08:20  dvolodin
# Исправлена обработка rodata.c
#
# Revision 1.2  87/01/11  16:17:00  avg
# Правлено под RCS.
#
#
# Для версии текстов без архивов RCS установите R=X
R=

RCS=RCS
COFLAGS=
I=/usr/include

CFLAGS=-O -DFLEXNAMES
LDFLAGS=-n -Q
DEST=/lib/cpp
SRC=cpy.y cpp.c yylex.c _yyfix

cpp :   cpp.o cpy.o rodata.o
	$(CC) $(LDFLAGS) rodata.o cpp.o cpy.o -o cpp

install: cpp
	-strip cpp
	@-rm -f $(DEST).old
	mv $(DEST) $(DEST).old
	cp cpp $(DEST)
	-chmod 1755 $(DEST)

cpy.c rodata.c: cpy.y _yyfix
	$(YACC) cpy.y
	csh -f -x _yyfix yyexca yyact yypact yypgo yyr1 yyr2 yychk yydef
	mv y.tab.c cpy.c

cpy.o : cpy.c yylex.c
	$(CC) $(CFLAGS) -c cpy.c

rodata.o : rodata.c _rofix
	$(CC) $(CFLAGS) -S rodata.c
	ed rodata.s < _rofix
	$(CC) $(CFLAGS) -c rodata.s

cpp.o:  $(I)/setjmp.h $(I)/ediag.h $(I)/stdio.h
cpy.o:  $(I)/ediag.h

clear clean:
	-rm -f cpy.c rodata.c cpp y.tab.c yacc.act *.[sob] *.bak

source: $(SRC) Makefile

CLRSRC: clear
	-rm -f $(SRC)

$(R)cpy.y:     $(RCS)/cpy.y,v       ; co $(COFLAGS) cpy.y
$(R)cpp.c:     $(RCS)/cpp.c,v       ; co $(COFLAGS) cpp.c
$(R)yylex.c:   $(RCS)/yylex.c,v     ; co $(COFLAGS) yylex.c
$(R)_yyfix:    $(RCS)/_yyfix,v      ; co $(COFLAGS) _yyfix
$(R)_rofix:    $(RCS)/_rofix,v      ; co $(COFLAGS) _rofix
$(R)Makefile:  $(RCS)/Makefile,v    ; co $(COFLAGS) Makefile

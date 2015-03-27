#CFLAGS=-O -Dunix=1 -Dpdp11=1 -DFLEXNAMES -n
CFLAGS=-g -Dunix=1 -Dsun4=1 -DFLEXNAMES
DESTDIR=/lib/c+

cpp :   cpp.o cpy.o rodata.o
	$(CC) $(CFLAGS) rodata.o cpp.o cpy.o
	mv a.out cpp
#	ln cpp ../cpp

cpy.c : cpy.y
	$(YACC) cpy.y
	csh -f -x :yyfix yyexca yyact yypact yypgo yyr1 yyr2 yychk yydef
	mv y.tab.c cpy.c

cpy.o : cpy.c yylex.c
	$(CC) $(CFLAGS) -c cpy.c

rodata.o : cpy.c
	$(CC) $(CFLAGS) -c rodata.c

clear :
	-rm -f cpy.c rodata.c cpp y.tab.c yacc.act
	-rm -f *.[so] *.bak

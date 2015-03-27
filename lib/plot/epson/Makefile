#       Makefile      библиотеки plot для Epson-80
#
CFLAGS= -O
OBJS=   arc.o box.o circle.o close.o dot.o erase.o label.o \
	line.o linemod.o move.o open.o point.o space.o subr.o font.o\
	color.o colormap.o

../libepson:    ${OBJS}
	ar cu ../libepson ${OBJS}

clean clear:
	rm -f ${OBJS} errs a.out core

font.o : font.c
	cc -a -c font.c

font.c: font7x9
	echo 'char Font[] = {' >font.c
	awk 'NF > 0  { i = 0; \
		if( $$1 == "#" ) i += 1; \
		if( $$2 == "#" ) i += 2; \
		if( $$3 == "#" ) i += 4; \
		if( $$4 == "#" ) i += 8; \
		if( $$5 == "#" ) i += 16; \
		if( $$6 == "#" ) i += 32; \
		if( $$7 == "#" ) i += 64; \
		printf( "%04o,", i ); \
		if( ++n == 9 ) { n = 0; printf( "\n" );}  }'  < font7x9 >> font.c
	echo '};' >> font.c

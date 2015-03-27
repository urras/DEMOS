M=		../mip
LOAD1=          -n -Q
LOAD2=		-n
CC=		cc -V
CFLAGS=		-O

all:		head

cp:		head
		-strip lpass1 lpass2
		cp lpass1 ${DESTDIR}/usr/lib/lint1
		cp lpass2 ${DESTDIR}/usr/lib/lint2
		cp SHELL ${DESTDIR}/usr/bin/lint
		chmod +x ${DESTDIR}/usr/bin/lint

cmp:		head
		cmp lpass1 ${DESTDIR}/usr/lib/lint1
		cmp lpass2 ${DESTDIR}/usr/lib/lint2
		cmp SHELL ${DESTDIR}/usr/bin/lint

head:		lpass1 lpass2
lpass1:         cgram.o xdefs.o scan.o comm1.o pftn1.o pftn2.o \
		trees.o optim.o lint.o
		cc $(LOAD1) -o lpass1 \
		cgram.o xdefs.o scan.o comm1.o pftn1.o pftn2.o \
		trees.o optim.o lint.o
trees.o:	$M/manifest macdefs $M/mfile1 $M/trees.c
		${CC} -c $(CFLAGS) -I$M -I. $M/trees.c
optim.o:	$M/manifest macdefs $M/mfile1 $M/optim.c
		${CC} -c $(CFLAGS) -I$M -I. $M/optim.c
pftn1.o:	$M/manifest macdefs $M/mfile1 $M/pftn1.c
		${CC} -c $(CFLAGS) -I$M -I. $M/pftn1.c
pftn2.o:	$M/manifest macdefs $M/mfile1 $M/pftn2.c
		${CC} -c $(CFLAGS) -I$M -I. $M/pftn2.c
lint.o:		$M/manifest macdefs $M/mfile1 lmanifest
		${CC} -c $(CFLAGS) -I$M -I. lint.c
scan.o: 	$M/manifest macdefs $M/mfile1 $M/scan.c
		${CC} -c $(CFLAGS) -I$M -I. $M/scan.c
xdefs.o: 	$M/manifest $M/mfile1 macdefs $M/xdefs.c
		${CC} -c $(CFLAGS) -I$M -I. $M/xdefs.c
cgram.o: 	$M/manifest $M/mfile1 macdefs cgram.c
		${CC} -c $(CFLAGS) -I$M -I. cgram.c
cgram.c: 	$M/cgram.y
		yacc $M/cgram.y
		mv y.tab.c cgram.c
comm1.o: 	$M/manifest $M/mfile1 $M/common macdefs $M/comm1.c
		${CC} -c $(CFLAGS) -I. -I$M $M/comm1.c
lpass2: 	lpass2.o
		cc -o lpass2 $(LOAD2) lpass2.o
lpass2.o:	$M/manifest lmanifest
		cc -c $(CFLAGS) -I$M -I. lpass2.c
lintall:	cgram.c
		lint -hpv -I. -I$M  cgram.c $M/xdefs.c $M/scan.c $M/pftn1.c $M/pftn2.c $M/trees.c $M/optim.c lint.c $M/reader.c local2.c order.c $M/match.c $M/allo.c $M/comm1.c table.c
clean:
		rm -f *.o lpass?

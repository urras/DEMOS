#CFLAGS= $(C1) -O -DFORTY # $(SEPID)    (из за c7t10)
#C1= # -B./ -t0
CFLAGS=-g -DFORTY

OBJ= c00.o c01.o c02.o c03.o c04.o c05.o  # alloc.o

#c0:     $(OBJ) xs.o
#	$(CC) $(CFLAGS) $(OBJ) xs.o -o c0
#	c7t10 c0
#	ln c0 ../c0
c0:	$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o c0
	ln c0 ../c0

$(OBJ): c0.h

strings: $(OBJ)

xs.o:   strings
	xstr
	$(CC) -S xs.c
	sed 's/.data/.text/' <xs.s >xy.s
	-rm -f xs.s
	as -o xs.o xy.s
	-rm -f xy.s

#.c.o:
#	cc -C -P $(CFLAGS) $*.c
#	xstr  -c $*.i
#	$(CC) -c $(CFLAGS) x.c
#	mv x.o $*.o
#	-rm -f x.c $*.i
.c.o:
	$(CC) -c $(CFLAGS) $*.c

clear:
	-rm -f c0 *.bak  x.c strings xs.c
	-rm -f *.[os]

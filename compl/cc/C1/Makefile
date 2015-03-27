#          ####                                                                       #
#         #    #                                        ##
#         #                             #####          # #
#         #              ######         #   #            #
#         #                             #   #            #
#         #    #                        #   #            #
#          ####                         #   #           ###


#CFLAGS=-O -f
#LDFLAGS=
CFLAGS=-g
LDFLAGS=
OBJ=c10.o c11.o c12.o c13.o

#c1:     $(OBJ) table.o xs.o
#	$(CC) -f $(OBJ) table.o xs.o -o 0c1
#	c7t10 0c1
#	mv 0c1 c1
#	-rm -f ../c1
#	ln c1 ../c1
c1:     $(OBJ) table.o
	$(CC) $(LDFLAGS) $(OBJ) table.o -o c1
	ln c1 ../c1

$(OBJ): c1.h

c10.o:  c10.c
c11.o:  c11.c
c12.o:  c12.c

#.c.o:
#	xstr -c $*.c
#	$(CC) -c $(CFLAGS) x.c
#	mv x.o $*.o
#	-rm -f x.c
.c.o:
	$(CC) -c $(CFLAGS)  $*.c

#table.o: table.s cvopt
#	unexpand -a < table.s | cvopt  | sed 's/.data/.text/' > table.i
#	as -o table.o table.i
table.o: table.s cvopt
	cvopt < table.s | sed 's/data/text/' > table.i
	as -o table.o table.i

#c13.o:  c13.c
#	$(CC) -S $(CFLAGS) c13.c
#	sed 's/.data/.text/' <c13.s >x.s
#	as - -o c13.o x.s
#	-rm -f c13.s x.s
c13.o:  c13.c

strings: $(OBJ)

xs.o:   strings
	xstr
	$(CC) -S xs.c
	-rm -f xs.c
	sed <xs.s 's/.data/.text/' >xy.s
	-rm -f xs.s
	as -o xs.o xy.s
	-rm -f xy.s

#cvopt:  cvopt.c
#	$(CC) -s -O cvopt.c -o cvopt
cvopt:  cvopt.c
	$(CC) -O cvopt.c -o cvopt

clear:
	-rm -f c1 table.i .b cvopt strings xs.c x.c c13.s xy.s x.s xs.s
	-rm -f *.[ob]

#.DEFAULT :  RCS/$@.c
#	co $(COFLAGS) $@

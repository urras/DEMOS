#####################################################################
#                                                                   #
#                                             *                     #
#                                      *     **                     #
#        ****   ****  ****  ***  ** *  *      *  ***  * **          #
#       *   *  *     *     *   * * * * ****   * *   * **  *         #
#       *   *   ***   ***  ****  * * * *   *  * ****  *             #
#       *   *      *     * *     * * * *   *  * *     *             #
#        *** * ****  ****   ***  * * * ****   *  ***  *             #
#                                                                   #
#                         дмс PDP-11                                #
#                                                                   #
#                         нопу тм 1.2                               #
#                                                                   #
#####################################################################

TOP=$(HOME)/d22

INS=$(TOP)/bin/as
OUT=as
CC=cc
# On SPARC I cannot pull ediag.h from common place.
# -I$(TOP)/include delivers DEMOS' stdio ahead of SUN's.
#CFLAGS=-g -I$(TOP)/include
CFLAGS=-g -I.
#LDFLG=-n -s
LDFLG=
OBJ=pass2.o pass1.o expr.o lex.o symtab.o tmpfil.o ubrs.o

#.c.o:
#    xstr -c $*.c
#    $(CC) $(CFLAGS) -c x.c
##        f_optim < x.s > xx.ss
##        mv xx.ss x.s
##        $(CC) $(CFLAGS) -c x.s
#    mv x.o $*.o
#    -rm -f x.c xx.ss x.s

.c.o:
	$(CC) $(CFLAGS) -c $*.c

####
all: $(OUT)

#$(OUT): $(OBJ) stidtbl.o xs.o Makefile
#    $(CC) $(LDFLG) $(OBJ) stidtbl.o xs.o -o $(OUT)
$(OUT): $(OBJ) stditbl.o Makefile
	$(CC) $(LDFLG) $(OBJ) stditbl.o -o $(OUT)

$(OBJ): as.h

#stidtbl.o: as.h ROfix
#    $(CC) -S stidtbl.c
#    ed - stidtbl.s <ROfix
#    $(CC) -c stidtbl.s
#    -rm -f stidtbl.s
stditbl.o: as.h
	$(CC) -c stditbl.c

#strings: $(OBJ)

#xs.o:   strings ROfix
#    xstr
#    $(CC) -S xs.c
#    ed - xs.s <ROfix
#    $(CC) -c xs.s
#    -rm -f xs.s xs.c

######
clear:
	-rm -f $(OUT) strings x.c xs.c
	-rm -f *.[sbo] xx.ss

redo:   clear $(OUT)

########
install: $(OUT)
#	-chmod u-t $(INS)
#	-$(INS) /dev/null
	-rm a.out
	-rm -f $(INS)
#	-mv $(INS) $(INS).OLD
	cp $(OUT) $(INS)
	cmp $(OUT) $(INS)
#	chmod 1755 $(INS)

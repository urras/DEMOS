#          ####           ####  #####  #######
#         #    #         #    # #    #    #
#         #              #    # #    #    #
#         #              #    # #####     #
#         #              #    # #         #
#         #    #         #    # #         #
#          ####           ####  #         #


CFLAGS= -O -DSUPER -Qoption cpp -Y$(HOME)/d22/include -I/usr/include
SEPID=-n

OBJ= c20.o c21.o c22.o

#c2:     $(OBJ) xs.o
#	-if test x$(SEPID) = x-n ; \
#	then $(CC) $(OBJ) xs.o -o c2 ; \
#	     c7t10 c2 ; \
#	else $(CC) $(SEPID) $(OBJ) xs.o -o c2 ; \
#	fi
#	rm -f ../c2
#	ln c2 ../c2
c2:     $(OBJ)
	$(CC) $(OBJ) -o c2
	rm -f ../c2
	ln c2 ../c2

$(OBJ): c2.h
c20.o: c20.c
c21.o: c21.c

#.c.o:
#	xstr -c $*.c
#	$(CC) -c $(CFLAGS) x.c
#	mv x.o $*.o
#	-rm -f x.c
.c.o:
	$(CC) -c $(CFLAGS) $*.c

strings: $(OBJ)

c22.o: c22.c
	$(CC) -c $(CFLAGS) c22.c

xs.o:   strings
	xstr
	$(CC) -c xs.c
	-rm -f xs.c

#        $(CC) -S xs.c
#        -rm -f xs.c
#        sed <xs.s 's/.data/.text/' | as -s -o xs.o
#        -rm -f xs.s

clean clear:
	-rm -f c2 .b strings xs.c x.c
	-rm -f *.[obs]

#.DEFAULT :  RCS/$@.c
#	co $(COFLAGS) $@


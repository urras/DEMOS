FORTOBJ=	fortran.o	# shortran.o for compatibility with
				# two byte integer F77 libraries
FORTSRC=	fortran.c	# shortran.c for compatibility with
				# two byte integer F77 libraries
OBJS=		pointers.o msqrt.o $(FORTOBJ) gcd.o pow.o mout.o \
		mdiv.o util2.o min.o mult.o madd.o square.o util.o
SOURCE=		pointers.c msqrt.c $(FORTSRC) gcd.c pow.c mout.c \
		mdiv.c util2.c min.c mult.c madd.c square.c util.c
LIB=		libape.a
INCLUDE=	$(DESTDIR)/usr/include/ape.h
DEFS=		-I4		# to change this you must remake F77 also
CFLAGS=		-O $(DEFS)
LFLAGS=		-bhp

all:		$(OBJS)
		-ar vru  $(LIB) $?
		ranlib $(LIB)

$(OBJS):	$(INCLUDE)

clean:
		rm -f $(OBJS) $(LIB)

tags:		$(SOURCE)
		ctags $(SOURCE)

lint:		$(SOURCE)
		lint $(LFLAGS) *.c

cp:		all
		cp $(LIB) $(DESTDIR)/usr/lib/$(LIB)
		ranlib $(DESTDIR)/usr/lib/$(LIB)

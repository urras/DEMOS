###################################################
#                                                 #
#          #            ### ###### ######         #
#         ##  #   #      #       #      #         #
#          #      #      #      #      #          #
#          #  #   ####   #     #      #           #
#          #  #   #   #  #    #      #            #
#          #  #   #   #  #   #      #             #
#          #   #  ####  ###  #      #             #
#                                                 #
###################################################
#
#   ���������� �����/������ ��������-77
#
# $Header: Makefile,v 1.1 88/12/07 21:33:27 ache Exp $
# $Log:	Makefile,v $
# Revision 1.1  88/12/07  21:33:27  ache
# Initial revision
# 

DESTDIR=/usr/lib/f77

# compile, then strip unnecessary symbols
.c.o :
	cc -O -V -c $<
	-ld -r -x $*.o
	mv a.out $*.o

all: libI77.a

install: libI77.a
	cp libI77.a $(DESTDIR)
	ranlib $(DESTDIR)/libI77.a


HEADERS=fmt.h fio.h fiodefs.h f_errno.h lio.h

CSRCS=           \
	backspace.c\
	cdfe.c\
	close.c\
	dolio.c\
	due.c\
	endfile.c\
	err.c\
	f_errlist.c\
	fmt.c\
	fmtlib.c\
	iio.c\
	inquire.c\
	lib.c\
	lio.c\
	lread.c\
	open.c\
	rdfe.c\
	rdfmt.c\
	rewind.c\
	rsfe.c\
	sfe.c\
	sue.c\
	uio.c\
	util.c\
	util1.c\
	wdfe.c\
	wrtfmt.c\
	wsfe.c

OBJECTS=         \
	backspace.o\
	cdfe.o\
	close.o\
	dolio.o\
	due.o\
	endfile.o\
	err.o\
	f_errlist.o\
	fmt.o\
	fmtlib.o\
	iio.o\
	inquire.o\
	lib.o\
	lio.o\
	lread.o\
	open.o\
	rdfe.o\
	rdfmt.o\
	rewind.o\
	rsfe.o\
	sfe.o\
	sue.o\
	uio.o\
	util.o\
	util1.o\
	wdfe.o\
	wrtfmt.o\
	wsfe.o

SRCS=$(CSRCS) $(HEADERS)

$(SRCS): fio.h fiodefs.h
lio.o dolio.o lread.o: lio.h


libI77.a : $(OBJECTS)
	-rm -f libI77.a
	ar r libI77.a `lorder $(OBJECTS) | tsort`
	ranlib libI77.a

# $Log:	Makefile,v $
# Revision 1.1  88/04/13  15:31:06  andrew
# Initial revision
# 
COFLAGS=-q

.DEFAULT:
	co $(COFLAGS) $@

.s.o:
	as -V -o $@ /usr/include/sys.s $<
	ld -r -x $@ -o a.out.lib
	mv a.out.lib $@

all: aldiv.o almul.o alrem.o ldiv.o lmul.o lrem.o

clean:
	rm -f *.[ob]

CLRSRC: clean
	rm -f *.[cs]

aldiv.o: aldiv.s
almul.o: almul.s
alrem.o: alrem.s
ldiv.o: ldiv.s
lmul.o: lmul.s
lrem.o: lrem.s

#   Укороченная библиотека стандартного ввода/вывода для f77
#   При трансляции без оверлея заменить CFLAGS на -O -s
#
# CFLAGS=-O -s
CFLAGS=-O -s -V

# libio.a:doprnt.o flsbuf.o filbuf.o data.o fopen.o endopen.o fgetc.o fputc.o strout.o
libio.a:ovdoprnt.o findiop.o flsbuf.o filbuf.o printf.o freopen.o fprintf.o data.o fopen.o endopen.o fgetc.o fputc.o strout.o
	rm -f libio.a
	ar cr libio.a *.o
	ranlib libio.a
flsbuf.o filbuf.o findiop.o data.o fopen.o printf.o fprintf.o reopen.o endopen.o fgetc.o fputc.o strout.o: stdio.h
doprnt.o : doprnt.s
ovdoprnt.o : ovdoprnt.s
	cc -c $(CFLAGS) ovdoprnt.s
flsbuf.o : flsbuf.c
isatty.o:isatty.c

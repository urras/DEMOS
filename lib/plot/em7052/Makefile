#       Makefile      библиотеки plot для ЭМ-7052
#
CFLAGS= -O
OBJS=   arc.o box.o circle.o close.o dot.o erase.o label.o \
	line.o linemod.o move.o open.o point.o space.o subr.o\
	color.o colormap.o

../libem7052:    ${OBJS}
	ar cu ../libem7052 ${OBJS}

clean clear:
	rm -f ${OBJS} errs a.out core

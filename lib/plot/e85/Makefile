#       Makefile      библиотеки plot для e-85
#
CFLAGS= -O
OBJS=   arc.o box.o circle.o close.o dot.o erase.o label.o \
	line.o linemod.o move.o open.o point.o space.o subr.o \
	color.o colormap.o

../libe85:    ${OBJS}
	ar cu ../libe85 ${OBJS}

clean clear:
	rm -f ${OBJS} errs a.out core

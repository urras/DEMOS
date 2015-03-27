#       Makefile
#
CFLAGS= -O
OBJS=   arc.o box.o circle.o close.o dot.o erase.o label.o \
	line.o linemod.o move.o open.o point.o space.o subr.o \
	color.o colormap.o

../libs5400:    ${OBJS}
	ar cu ../libs5400 ${OBJS}

clean clear:
	rm -f ${OBJS} errs a.out core

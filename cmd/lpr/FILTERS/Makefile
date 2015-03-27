DEST          = /usr/lib

EXTHDRS       = /usr/include/ctype.h \
                /usr/include/signal.h \
                /usr/include/stdio.h \
                /usr/include/sys/lpio.h

HDRS          = chrtab.h

LDFLAGS       = -n

CFLAGS        = -O

LIBS          =

LINKER        = cc

MAKEFILE      = Makefile

OBJS          = lpf.o \
                ptxf.o \
                ptxplotf.o

PRINT         = pr -2 -w132

SRCS          = lpf.c \
                ptxf.c \
                ptxplotf.c

all:            lpf ptxf ptxplotf

lpf:            lpf.o
                @echo -n "Loading $@ ... "
                @$(LINKER) $(LDFLAGS) $@.o -o $@
                @echo "done"

ptxf:           ptxf.o
                @echo -n "Loading $@ ... "
                @$(LINKER) $(LDFLAGS) $@.o -o $@
                @echo "done"

ptxplotf:       ptxplotf.o
                @echo -n "Loading $@ ... "
                @$(LINKER) $(LDFLAGS) $@.o -o $@
                @echo "done"

clean:;         @rm -f $(OBJS)

depend:;        @mkmf -f $(MAKEFILE) DEST=$(DEST)

install:        i_lpf i_ptxplotf i_ptxf

i_lpf:          lpf
                @echo Installing lpf in $(DEST)
                @install -s lpf $(DEST)

i_ptxf:         ptx
                @echo Installing ptx in $(DEST)
                @install -s ptx $(DEST)

i_ptxplotf:     ptxplotf
                @echo Installing ptxplotf in $(DEST)
                @install -s ptxplotf $(DEST)

print:;         @$(PRINT) $(HDRS) $(SRCS)

###
lpf.o: /usr/include/stdio.h /usr/include/signal.h /usr/include/ctype.h \
        /usr/include/sys/lpio.h
ptxf.o: /usr/include/stdio.h /usr/include/signal.h /usr/include/ctype.h \
        /usr/include/sys/lpio.h chrtab.h
ptxplotf.o: /usr/include/stdio.h /usr/include/signal.h \
        /usr/include/sys/lpio.h /usr/include/ctype.h chrtab.h

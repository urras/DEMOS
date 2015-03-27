DEST          = /usr

EXTHDRS       = /usr/include/a.out.h \
                /usr/include/ar.h \
                /usr/include/ctype.h \
                /usr/include/dir.h \
                /usr/include/pwd.h \
                /usr/include/setjmp.h \
                /usr/include/signal.h \
                /usr/include/stdio.h \
                /usr/include/sys/stat.h \
                /usr/include/sys/types.h

HDRS          = lpc.h \
                lpd.h \
                lpq.h \
                lpr.h \
                lpr_def.h \
                lprm.h

LIBS          = LIB/liblpd.a

LDFLAGS       = -n

CFLAGS        = -O

LINKER        = cc

MAKEFILE      = Makefile

OBJS          = lpc.o \
                lpd.o \
                lpq.o \
                lpr.o \
                lprm.o

PRINT         = pr

SRCS          = lpc.c \
                lpd.c \
                lpq.c \
                lpr.c \
                lprm.c

SPOOLING      = spooling

ROOT          = root

ALL           = lpr lpq lpd lprm lpc

INSTALL       = i_lpr i_lpq i_lprm i_lpd i_lpc

all:            $(ALL)

lpr:            lpr.o $(LIBS)
                @echo -n "Loading $@ ... "
                @$(LINKER) $(LDFLAGS) lpr.o $(LIBS) -o $@
                @echo "done"

lpq:            lpq.o $(LIBS)
                @echo -n "Loading $@ ... "
                @$(LINKER) $(LDFLAGS) lpq.o $(LIBS) -o $@
                @echo "done"

lprm:           lprm.o $(LIBS)
                @echo -n "Loading $@ ... "
                @$(LINKER) $(LDFLAGS) lprm.o $(LIBS) -o $@
                @echo "done"

lpd:            lpd.o $(LIBS)
                @echo -n "Loading $@ ... "
                @$(LINKER) $(LDFLAGS) lpd.o $(LIBS) -o $@
                @echo "done"

lpc:            lpc.o $(LIBS)
                @echo -n "Loading $@ ... "
                @$(LINKER) $(LDFLAGS) lpc.o $(LIBS) -o $@
                @echo "done"

clean:;         @rm -f $(OBJS) $(ALL) a.out core

depend:;        @mkmf -f $(MAKEFILE) DEST=$(DEST)

index:;         @ctags -wx $(HDRS) $(SRCS)

print:;         @$(PRINT) $(HDRS) $(SRCS)

install:        $(INSTALL)

i_lpr:          lpr
                @echo -n "Installing lpr in $(DEST)/bin ... "
                @install -s lpr $(DEST)/bin
                @chown $(ROOT) $(DEST)/bin/lpr
                @chgrp $(SPOOLING) $(DEST)/bin/lpr
                @chmod a+rx-w,ug+s $(DEST)/bin/lpr
                @echo done

i_lprm:         lprm
                @echo -n "Installing lprm in $(DEST)/bin ... "
                @install -s lprm $(DEST)/bin
                @chown $(ROOT) $(DEST)/bin/lprm
                @chgrp $(SPOOLING) $(DEST)/bin/lprm
                @chmod a+rx-w,ug+s $(DEST)/bin/lprm
                @echo done

i_lpq:          lpq
                @echo -n "Installing lpq in $(DEST)/bin ... "
                @install -s lpq $(DEST)/bin
                @chown $(ROOT) $(DEST)/bin/lpq
                @chgrp $(SPOOLING) $(DEST)/bin/lpq
                @chmod a+rx-w,ug+s $(DEST)/bin/lpq
                @echo done

i_lpc:          lpc
                @echo -n "Installing lpc in $(DEST)/bin ... "
                @install -s lpc $(DEST)/bin
                @chown $(ROOT) $(DEST)/bin/lpc
                @chgrp $(SPOOLING) $(DEST)/bin/lpc
                @chmod a+rx-w,ug+s $(DEST)/bin/lpc
                @echo done

i_lpd:          lpd
                @echo -n "Installing lpd in $(DEST)/lib ... "
                @install -s lpd $(DEST)/lib
                @chown $(ROOT) $(DEST)/lib/lpd
                @chgrp $(SPOOLING) $(DEST)/lib/lpd
                @chmod a+rx-w,ug+s $(DEST)/lib/lpd
                @echo done

$(LIBS):
        cd LIB; make $(MFLAGS) library clean

###
lpc.o: lpr_def.h /usr/include/sys/types.h /usr/include/dir.h \
        /usr/include/sys/stat.h /usr/include/signal.h /usr/include/stdio.h \
        /usr/include/ctype.h lpc.h
lpd.o: lpr_def.h /usr/include/sys/types.h /usr/include/dir.h \
        /usr/include/sys/stat.h /usr/include/signal.h /usr/include/setjmp.h \
        /usr/include/stdio.h lpd.h
lpq.o: lpr_def.h /usr/include/sys/types.h /usr/include/dir.h \
        /usr/include/sys/stat.h /usr/include/signal.h /usr/include/stdio.h \
        /usr/include/ctype.h lpq.h
lpr.o: lpr_def.h /usr/include/sys/types.h /usr/include/dir.h \
        /usr/include/sys/stat.h /usr/include/signal.h /usr/include/a.out.h \
        /usr/include/ar.h /usr/include/pwd.h /usr/include/stdio.h \
        /usr/include/ctype.h lpr.h
lprm.o: lpr_def.h /usr/include/sys/types.h /usr/include/dir.h \
        /usr/include/sys/stat.h /usr/include/signal.h /usr/include/pwd.h \
        /usr/include/stdio.h /usr/include/ctype.h lprm.h

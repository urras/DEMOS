DEST          = .

EXTHDRS       = ../lpr_def.h \
                /usr/include/ctype.h \
                /usr/include/dir.h \
                /usr/include/pwd.h \
                /usr/include/stat.h \
                /usr/include/stdio.h \
                /usr/include/sys/types.h

HDRS          =

LIBRARY       = liblpd.a

MAKEFILE      = Makefile

OBJS          = basename.o \
                cpword.o \
                getqueue.o \
                getusrid.o \
                message.o \
                printcap.o

PRINT         = pr -2 -w132

SRCS          = basename.c \
                cpword.c \
                getqueue.c \
                getusrid.c \
                message.c \
                printcap.c

CFLAGS        = -I.. -O

$(LIBRARY):     $(OBJS)
                @echo -n "Loading $(LIBRARY) ... "
                @ar cru $(LIBRARY) $(OBJS)
                @ranlib $(LIBRARY)
                @echo "done"

library:        $(LIBRARY)

clean:;         @rm -f $(OBJS)

depend:;        @mkmf -f $(MAKEFILE) LIBRARY=$(LIBRARY) DEST=$(DEST)

extract:;       @ar x $(LIBRARY)
                @rm -f __.SYMDEF

index:;         @ctags -wx $(HDRS) $(SRCS)

print:;         @$(PRINT) $(HDRS) $(SRCS)

###
cpword.o: /usr/include/ctype.h
getqueue.o: ../lpr_def.h /usr/include/sys/types.h /usr/include/dir.h \
        /usr/include/stat.h
getusrid.o: /usr/include/pwd.h
message.o: /usr/include/stdio.h
printcap.o: /usr/include/ctype.h

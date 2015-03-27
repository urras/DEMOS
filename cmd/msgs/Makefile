CFLAGS=-O
msgs: msgs.c msgs.h
        cc $(CFLAGS) -o msgs msgs.c -ltermlib
install:
        install -s msgs ${DESTDIR}/usr/bin
clean:
        -rm -f msgs

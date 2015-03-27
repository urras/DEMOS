#       $Header: Makefile,v 1.1 88/09/22 12:09:32 korotaev Exp $
TOP=/home/mellorn/zaitcev/d22
DESTLIB=$(TOP)/lib
DESTBIN=$(TOP)/bin
OLIB=/usr/old/lib
#CFLAGS=-O -n -s
#CFLAGS=-O -I$(HOME)/d22/include
# Place the system stdio.h ahead of our own wich stucks witch ediag.h.
CFLAGS=-g -Qoption cpp -Y$(HOME)/d22/include -I/usr/include

all:    ccnew c0 c1 c2 #cpp

c0:
	cd C0;make c0

c1:
	cd C1;make c1

c2:
	cd C2;make c2

cpp:
	cd CPP; make cpp

ccnew:  cc.c
	$(CC) $(CFLAGS) cc.c -o ccnew

save:
	cp /lib/cpp ${OLIB}/cpp
	cp /lib/c0 ${OLIB}/c0
	cp /lib/c1 ${OLIB}/c1
	cp /lib/c2 $OLIB}/c2
	cp /bin/cc $OLIB}/cc

install: cp.cc cp.c0 cp.c1 cp.c2

cp.cpp: cpp
	-chmod u-t $(DESTLIB)/cpp
	-$(DESTLIB)/cpp < /dev/null > /dev/null
#       -mv $(DESTLIB)/cpp $(OLIB)/cpp
	 cp cpp $(DESTLIB)/cpp
	 cmp cpp $(DESTLIB)/cpp
#	-chmod 1755 $(DESTLIB)/cpp

cp.c0:  c0
	-chmod u-t $(DESTLIB)/c0
	-$(DESTLIB)/c0 < /dev/null > /dev/null
#       -mv $(DESTLIB)/c0 $(OLIB)/c0
	 cp  c0 $(DESTLIB)/c0
	 cmp c0 $(DESTLIB)/c0
#	-chmod 1755 $(DESTLIB)/c0

cp.c1:  c1
	-chmod u-t $(DESTLIB)/c1
	-$(DESTLIB)/c1 < /dev/null > /dev/null
#       -mv $(DESTLIB)/c1 $(OLIB)/c1
	 cp  c1 $(DESTLIB)/c1
	 cmp c1 $(DESTLIB)/c1
#	-chmod 1755 $(DESTLIB)/c1

cp.c2:  c2
	-chmod u-t $(DESTLIB)/c2
	-$(DESTLIB)/c2 < /dev/null > /dev/null
#       -mv $(DESTLIB)/c2 $(OLIB)/c2
	 cp  c2 $(DESTLIB)/c2
	 cmp c2 $(DESTLIB)/c2
#	-chmod 1755 $(DESTLIB)/c2

cp.cc:  ccnew
	-chmod u-t $(DESTBIN)/bin/cc
	-$(DESTBIN)/cc < /dev/null > /dev/null
#	-mv $(DESTBIN)/cc $(DESTBIN)/cc.old
	 cp  ccnew $(DESTBIN)/cc
	 cmp ccnew $(DESTBIN)/cc
#	-chmod 1755 $(DESTBIN)/cc

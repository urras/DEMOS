#
#       Makefile  ��� ���������� ��������
#
# $Log:	Makefile,v $
# Revision 1.3  90/11/12  19:28:36  root
# ��������� ��� �� 1425.
# 
# Revision 1.2  88/12/26  19:14:26  korotaev
# �������� RCSSRC.
# 
# Revision 1.1  86/07/14  17:18:39  avg
# Initial revision
#
#
#  ###  ���������� ��������� ������ �������� �����
#       48K ������ ����������� ������ ��� ����, �����
#       ��� ����� ������������ ��� �������� ���������.
#       ��� ������ �������� ��� restor.
#
#
# ��� ������ ���. ������� ��� ������� RCS ���������� R=X
R=
RCS=RCS
COFLAGS=

CC=$(HOME)/d22/bin/cc
LD=$(HOME)/d22/bin/ld
AS=$(HOME)/d22/bin/as
AR=$(HOME)/d22/bin/ar
SIZE=$(HOME)/d22/bin/size

SRC=c.c mtboot.s saio.h srt0.s tsboot.s maketape.c cat.c bcopy.c vcopy.c
DIRS=bootstrap libsa tapes

LIBSA = libsa
SRCRCS  = /usr/src/cmd/etc/RCS
INCLUDES=
BOOT = bootstrap

CFLAGS = -O -DSTANDALONE ${INCLUDES}

ALL=    restor cat mkfs mtboot tsboot maketape boot vcopy

all:    $(ALL) size

libsa.a: ${LIBSA} saio.h
	(cd libsa; make INCLUDES="${INCLUDES}" COFLAGS="${COFLAGS}" ../libsa.a)

restor: libsa.a srt0.o restor.c c.o dummyfont.o
	cc -c ${CFLAGS} restor.c
	ld -o restor srt0.o restor.o c.o dummyfont.o libsa.a -lc
	rm restor.o

vcopy: libsa.a srt0.o vcopy.c c.o
	cc -c ${CFLAGS} vcopy.c
	ld -o vcopy srt0.o vcopy.o c.o libsa.a -lc
	rm vcopy.o

cat:    libsa.a srt0.o c.o cat.c
	cc -c ${CFLAGS} cat.c
	ld -s -o cat srt0.o cat.o c.o libsa.a -lc
	chmod a-x cat
	rm cat.o

mkfs:   libsa.a srt0.o mkfs.c c.o
	cc -c ${CFLAGS} mkfs.c
	ld  -o mkfs srt0.o mkfs.o c.o libsa.a -lc
	rm mkfs.o

mtboot: mtboot.s
	as -o mtboot mtboot.s
	strip mtboot

tsboot: tsboot.s
	as -o tsboot.o tsboot.s
	strip tsboot.o
	dd if=tsboot.o of=tsboot bs=8w skip=1

maketape: maketape.c
	cc -o maketape maketape.c

#boot:   ${BOOT} libsa.a c.o saio.h
#	cd bootstrap;make INCLUDES="${INCLUDES}" COFLAGS="${COFLAGS}" ../boot
boot:   ${BOOT} libsa.a c.o saio.h
	cd bootstrap;make ../boot

tape: tape0 tape1 tape2 tape3 tape4
tape0:
	maketape $$TAPE tapes/tapedir;

tape1:
	dump 0 /mnt/root

tape2:
	s=`cat tapes/usrtar2`;cd /usr;tar cb 32  $$s
tape3:
	-mv boot tmboot tsboot mkfs restor cat vcopy ../standout
	s=`cat tapes/usrtar3`;cd /usr;tar cb 32  $$s
	-cd ../standout;mv * ../stand
tape4:
	s=`cat tapes/usrtar4`;cd /usr;tar cvb 32  $$s

tapeII:
	cd /usr/man/cat1;
	cd /mnt/sys/doc;        # �������� - ���� �� �����
	cd /usr;tar cibS 32 10000 \
		  man -C /mnt/sys doc -C /mnt/src src/cmd \
		  src/lib src/compl src/roff src/rcs \
		  src/unused src/games src/[A-Z]* -C /mnt/sys src/RCS_sys \
		  man  local
tapeIII:
	cd /mnt/avg/RUBIN;
	cd /mnt/avg;tar cb 32 RUBIN/R*  RUBIN/doc  RUBIN/bin RUBIN/files\
			RUBIN/lib/libq.a  RUBIN/demo RUBIN/source

size:
	size ${ALL}

boot.dsk:     boot boot.proto
	./mkfs boot.dsk boot.proto

clean clear:
	for i in $(DIRS) ; do cd $$i; make clear; cd .. ; done
	rm -f $(ALL) libsa.a
	rm -f *.[ob] *.bak

CLRSRC:
	for i in $(DIRS) ; do cd $$i; make CLRSRC; cd .. ; done
	rm -f $(ALL) $(SRC)
	rm -f *.[ob]   *.bak

source: $(SRC) Makefile
	for i in $(DIRS) ; do cd $$i; \
	make COFLAGS="${COFLAGS}" source; cd .. ; done

$(R)c.c:       ; co $(COFLAGS) c.c
$(R)mtboot.s:  ; co $(COFLAGS) mtboot.s
$(R)saio.h:    ; co $(COFLAGS) saio.h
$(R)srt0.s:    ; co $(COFLAGS) srt0.s
$(R)tsboot.s:  ; co $(COFLAGS) tsboot.s
$(R)maketape.c:; co $(COFLAGS) maketape.c
$(R)vcopy.c:   ; co $(COFLAGS) vcopy.c
$(R)cat.c:     ; co $(COFLAGS) cat.c
$(R)bcopy.c:   ; co $(COFLAGS) bcopy.c
$(R)Makefile:  ; co $(COFLAGS) Makefile
$(R)mkfs.c:    ; co $(COFLAGS) $(SRCRCS)/mkfs.c,v
$(R)restor.c:  ; co $(COFLAGS) $(SRCRCS)/restor.c,v

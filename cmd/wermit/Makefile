# CKUKER.MAK, Version 2.13, 24 January 1989
# 
PROC=proc
DIRECT=
NPROC=nproc
NPTYPE=int
BOOTFILE=/edition7
#
# ( For old Tandy TRS-80 Model 16A or 6000 V7-based Xenix, use PROC=_proc,
#   DIRECT=-DDIRECT, NPROC=_Nproc, NPTYPE=short, BOOTFILE=/xenix )
#
###########################################################################
#
#  Compile and Link variables:
#
LNKFLAGS=
SHAREDLIB=
CC= cc
CC2= cc
#
###########################################################################
#
# Dependencies Section:
#
make:
	@echo 'Make what?  You must tell which system to make C-Kermit for.'

wermit: ckcmai.o ckucmd.o ckuusr.o ckuus2.o ckuus3.o ckcpro.o ckcfns.o \
		 ckcfn2.o ckucon.o ckutio.o ckufio.o ckudia.o ckuscr.o
	$(CC2) $(LNKFLAGS) -o wermit ckcmai.o ckutio.o ckufio.o ckcfns.o \
		 ckcfn2.o ckcpro.o ckucmd.o ckuus2.o ckuus3.o ckuusr.o \
		 ckucon.o ckudia.o ckuscr.o $(LIBS)

ckcmai.o: ckcmai.c ckcker.h ckcdeb.h ckcsym.h

ckuusr.o: ckuusr.c ckucmd.h ckcker.h ckuusr.h ckcdeb.h

ckuus2.o: ckuus2.c ckucmd.h ckcker.h ckuusr.h ckcdeb.h

ckuus3.o: ckuus3.c ckucmd.h ckcker.h ckuusr.h ckcdeb.h

ckucmd.o: ckucmd.c ckucmd.h ckcdeb.h

ckcpro.o: ckcpro.c ckcker.h ckcdeb.h

ckcpro.c: ckcpro.w wart
	./wart ckcpro.w ckcpro.c

ckcfns.o: ckcfns.c ckcker.h ckcdeb.h ckcsym.h

ckcfn2.o: ckcfn2.c ckcker.h ckcdeb.h ckcsym.h

ckufio.o: ckufio.c ckcker.h ckcdeb.h

ckutio.o: ckutio.c ckcdeb.h

ckucon.o: ckucon.c ckcker.h ckcdeb.h

wart: ckwart.o
	$(CC) $(LNKFLAGS) -o wart ckwart.o

ckwart.o: ckwart.c

ckudia.o: ckudia.c ckcker.h ckcdeb.h ckucmd.h

ckuscr.o: ckuscr.c ckcker.h ckcdeb.h
#
###########################################################################
#
# Make commands for specific systems:
#
#
#Berkeley Unix 4.1 or 4.2, 4.3, also Ultrix-32 1.x, 2.0
bsd:
	make wermit "CFLAGS= -O -DBSD4 -DDEBUG -DTLOG"


#Berkeley Unix 2.8, 2.9 for PDP-11s with I&D space, maybe also Ultrix-11???
#If you have trouble with this, try removing "-l ndir".  If you still have
#trouble, remove "-DDEBUG -DTLOG".  Or try defining CC and/or CC2 as "pcc"
#instead of "cc".
bsd29:
	make wermit "CFLAGS= -O -DBSD29 -DTLOG" \
		"LNKFLAGS= -i -Q -lndir" "CC= cc " "CC2= cc"

#Berkeley Unix 2.10 (Stan Barber, sob@bcm.tmc.edu)
bsd210:
	make wermit "CFLAGS= -DBSD29 -DDEBUG -DTLOG" -DLCKDIR \
		"LNKFLAGS= -i " "CC= cc " "CC2= cc"

#SUN OS version 4.0 or later
sunos4:
	make wermit "CFLAGS= -DBSD4 -DSUNOS4 -DDEBUG -DTLOG"

#Version 7 Unix (see comments above)
v7:
	make wermit "CFLAGS=-DV7 -DDEBUG -DTLOG -DPROCNAME=\\\"$(PROC)\\\" \
	-DBOOTNAME=\\\"$(BOOTFILE)\\\" -DNPROCNAME=\\\"$(NPROC)\\\" \
	-DNPTYPE=$(NPTYPE) $(DIRECT)"

#System V R3, some things changed since Sys V R2...
sys5r3:
	make wermit "CFLAGS = -DSVR3 -DUXIII -DDEBUG -DTLOG -i -O" \
		"LNKFLAGS = -i"

#In case they type "make sys5"...
sys5:
	make sys3

#Generic ATT System III or System V (with I&D space)
sys3:
	make wermit "CFLAGS = -DUXIII -DDEBUG -DTLOG -i -O" "LNKFLAGS = -i"

#Generic ATT System III or System V (no I&D space)
sys3nid:
	make wermit "CFLAGS = -DUXIII -DDEBUG -DTLOG -O" "LNKFLAGS ="

#AT&T 7300/Unix PC systems, sys3 but define symbol ATT7300
sys3upc:
	make wermit "CFLAGS = -DUXIII -DATT7300 -DDEBUG -DTLOG -i -O" \
		"LNKFLAGS = -i"

#AT&T 3B2, 3B20-series computers running System V
#  Only difference from sys3 is lock file stuff...
att3bx:
	make wermit "CFLAGS = -DUXIII -DATT3BX -DDEBUG -DTLOG -i -O" \
		"LNKFLAGS = -i"

#HP 9000 series 300, 500, 800.
hpux:
	make wermit "CFLAGS = -DUXIII -DHPUX -DDEBUG -DTLOG -O" "LNKFLAGS ="

#Regulus on CIE Systems 680/20
cie:
	make wermit "CFLAGS = -DUXIII -DCIE -DDEBUG -DTLOG -O" "LNKFLAGS ="

#Microport Sys V for IBM PC/AT and clones
mpsysv:
	make wermit "CFLAGS= -O -DXENIX -DUXIII -DTLOG Ml -i" \
		"LNKFLAGS = -Ml -i"

#Microsoft "Xenix/286" e.g. for IBM PC/AT
xenix:
	make wermit "CFLAGS= -Ml2 -DXENIX -DUXIII -DDEBUG -DTLOG -F 3000 -i" \
		"LNKFLAGS = -Ml2 -F 3000 -i"

#SCO Xenix/286 2.2.1, e.g. for IBM PC/AT, PS/2 Model 50, etc.
sco286:
	make wermit "CFLAGS= -Ox -DXENIX -DUXIII -DDEBUG -DTLOG -F 3000 -i -M2le" \
		"LNKFLAGS = -F 3000 -i -M2le"

#SCO Xenix 2.2.1 for IBM PC, XT, PS2/30, or other 8088 or 8086 machine
sco86:
	make wermit "CFLAGS= -DXENIX -DUXIII -DDEBUG -DTLOG -F 3000 -i -M0me" \
		"LNKFLAGS = -F 3000 -i -M0me"

#SCO Xenix/386 2.2.2
sco386:
	make wermit "CFLAGS= -DXENIX -DUXIII -DDEBUG -DTLOG -Otcl  -i -M3e" \
		"LNKFLAGS = -i"

#PC/IX, Interactive Corp System III for IBM PC/XT
pcix:
	make wermit \
	"CFLAGS= -DPCIX -DUXIII -DISIII -DDEBUG -DTLOG -Dsdata=sdatax -O -i" \
		"LNKFLAGS = -i"


#Interactive Corp System III port in general --
is3:
	make wermit \
		"CFLAGS = -DISIII -DUXIII -DDEBUG -DTLOG -Ddata=datax -O -i" \
		"LNKFLAGS = -i"


#Masscomp System III
rtu:
	make wermit "CFLAGS= -UFIONREAD -DUXIII -DDEBUG -DTLOG -O" \
		"LNKFLAGS =" "LIBS= -ljobs"

#DEC Pro-3xx with Pro/Venix V1.0 or V1.1
# Requires code-mapping on non-I&D-space 11/23 processor, plus some
# fiddling to get interrupt targets into resident code section.
provx1:
	make wart "CFLAGS= -DPROVX1" "LNKFLAGS= "
	make wermit "CFLAGS = -DPROVX1 -DDEBUG -DTLOG -md780" \
		"LNKFLAGS= -u _sleep -lc -md780"

#NCR Tower 1632, OS 1.02
tower1:
	make wermit "CFLAGS= -DDEBUG -DTLOG -DTOWER1"

#Fortune 32:16, For:Pro 1.8 (mostly like 4.1bsd)
ft18:
	make wermit "CFLAGS= -DTLOG -DBSD4 -DFT18"

#Valid Scaldstar
#Berkeleyish, but need to change some variable names.
valid:
	make wermit "CFLAGS= -DBSD4 -Dcc=ccx -DFREAD=1"

#IBM IX/370 on IBM 370 Series mainframes
#Mostly like sys3, but should buffer packets.
ix370:
	make wermit "CFLAGS = -DIX370 -DUXIII -DDEBUG -DTLOG -i -O" \
	"LNKFLAGS = -i"

#Amdahl UTS 2.4 on IBM 370 series compatible mainframes.
#Mostly like V7, but can't do initrawq() buffer peeking.
uts24:
	make wermit "CFLAGS=-DV7 -DDEBUG -DTLOG -DPROCNAME=\\\"$(PROC)\\\" \
	-DUTS24 -DBOOTNAME=\\\"$(BOOTFILE)\\\" -DNPROCNAME=\\\"$(NPROC)\\\" \
	-DNPTYPE=$(NPTYPE) $(DIRECT)"

#BBN C/70 with IOS 2.0
#Mostly Berkeley-like, but with some ATTisms
c70:
	make wermit "CFLAGS= -DBSD4 -DC70 -DDEBUG -DTLOG"

#Zilog ZEUS 3.21
zilog:
	make wermit "CFLAGS = -DUXIII -DZILOG -DTLOG -i -O" "LNKFLAGS = -i"

#CDC VX/VE 5.2.1
vxve:
	make wermit "CFLAGS = -DUXIII -DVXVE -i -O" "LNKFLAGS = -i"

#Clean up intermediate and object files
clean:
	-rm -f ckcmai.o ckucmd.o ckuusr.o ckuus2.o ckuus3.o ckcpro.o \
	ckcfns.o ckcfn2.o ckucon.o ckutio.o ckufio.o ckudia.o ckuscr.o \
	ckwart.o ckcpro.c

#Run Lint on this mess for the BSD version.
lint:
	-lint -x -DBSD4 -DDEBUG -DTLOG ck[cu]*.[hc] > ck.lint.bsd4


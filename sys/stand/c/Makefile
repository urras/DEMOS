#
# $Header: Makefile,v 1.8 89/11/13 15:51:46 ache1 Exp $
#
CC=$(HOME)/d22/bin/cc
AS=$(HOME)/d22/bin/as
LD=$(HOME)/d22/bin/ld
AR=$(HOME)/d22/bin/ar

COFLAGS=-q
I=/usr/include/
IS=/usr/include/sys/

.DEFAULT:
	co $(COFLAGS) $@

#.s.o:
#	$(AS) -V -o $@ $(I)sys.s $<
#	$(LD) -r -x $@ -o a.out.lib
#	mv a.out.lib $@
.s.o:
	$(AS) -o $@ $<

#.c.o:
#	$(CC) -O -c $<
#	$(LD) -r -x $@ -o a.out.lib
#	mv a.out.lib $@
.c.o:
	$(CC) -c $<

COMM =  abort.o \
	abs.o \
	atof.o \
	atoi.o \
	atol.o \
	calloc.o \
	crypt.o \
	ctime.o \
	ctype_.o \
	cuexit.o \
	ecvt.o \
	errlst.o \
	execvp.o \
	fakcu.o \
	frexp11.o \
	getenv.o \
	gethostnam.o \
	getlogin.o \
	getopt.o \
	getwd.o \
	index.o \
	isapipe.o \
	isatty.o \
	l3.o \
	lcmp.o \
	ldexp11.o \
	ldfps.o \
	loadav.o \
	malloc.o \
	mktemp.o \
	modf11.o \
	mon.o \
	msgset.o \
	nlist.o \
	perror.o \
	psignal.o \
	qsort.o \
	rand.o \
	regex.o \
	rindex.o \
	semset.o \
	sigblock.o \
	sigcatch.o \
	sighold.o \
	sigign.o \
	siglist.o \
	sigold.o \
	sigpause.o \
	sigrelse.o \
	sigset.o \
	sigsmask.o \
	sleep.o \
	strcat.o \
	strcmp.o \
	strcpy.o \
	strlcmp.o \
	strlen.o \
	strncat.o \
	strncmp.o \
	strncpy.o \
	strnlcmp.o \
	stty.o \
	swab.o \
	tell.o \
	timezone.o \
	ttyname.o \
	ttyslot.o

all: $(COMM) setjmp.o

overlay: $(COMM)
	rm -f setjmp.o
	cd ../overlay; make setjmp.o

clean:
	rm -f *.[ob]

CLRSRC: clean
	rm -f *.[cs]

abs.o:  abs.c
atof.o: atof.c $(I)math.h $(I)ctype.h
atoi.o: atoi.c
atol.o: atol.c
calloc.o:       calloc.c
crypt.o:        crypt.c
ctime.o:        ctime.c $(I)ediag.h $(I)time.h $(IS)types.h $(IS)timeb.h
ctype_.o:       ctype_.c $(IS)code.h $(I)ctype.h
ecvt.o: ecvt.c
errlst.o:       errlst.c
execvp.o:       execvp.c $(I)errno.h
getenv.o:       getenv.c
gethostnam.o:   gethostnam.c $(I)errno.h $(IS)types.h $(IS)file.h
getlogin.o:     getlogin.c $(I)utmp.h
getopt:         getopt.c $(I)stdio.h $(I)ediag.h
getwd.o:        getwd.c $(IS)param.h $(IS)stat.h $(IS)dir.h $(IS)file.h $(I)ediag.h
index.o:        index.c
isapipe.o:      isapipe.c $(I)errno.h
isatty.o:       isatty.c $(I)sgtty.h
l3.o:   l3.c
lcmp.o:         lcmp.c $(IS)code.h
loadav.o:       loadav.c
malloc.o:       malloc.c
mktemp.o:       mktemp.c
mon.o:  mon.c
msgset.o:       msgset.c $(IS)msg.h
nlist.o:        nlist.c $(I)a.out.h
perror.o:       perror.c $(I)ediag.h
psignal.o:      psignal.c $(I)signal.h
qsort.o:        qsort.c
rand.o: rand.c
regex.o:        regex.c $(IS)code.h
rindex.o:       rindex.c
semset.o:       semset.c $(IS)sem.h
sigblock.o:     sigblock.c $(I)signal.h
sigcatch.o:     sigcatch.c $(I)signal.h
sighold.o:      sighold.c $(I)signal.h
sigign.o:       sigign.c $(I)signal.h
siglist.o:      siglist.c $(IS)localopts.h $(I)signal.h
sigold.o:       sigold.c $(I)signal.h $(I)errno.h
sigpause.o:     sigpause.c $(I)signal.h
sigrelse.o:     sigrelse.c $(I)signal.h
sigset.o:       sigset.c $(I)signal.h $(I)errno.h
sigsmask.o:     sigsmask.c $(I)signal.h
sleep.o:        sleep.c $(I)signal.h
strcat.o:       strcat.c
strcmp.o:       strcmp.c
strcpy.o:       strcpy.c
strlcmp.o:      strlcmp.c $(IS)code.h $(I)ctype.h
strlen.o:       strlen.c
strncat.o:      strncat.c
strncmp.o:      strncmp.c
strncpy.o:      strncpy.c
strnlcmp.o:     strnlcmp.c $(IS)code.h $(I)ctype.h
stty.o: stty.c $(I)sgtty.h
swab.o: swab.c
tell.o: tell.c
timezone.o:     timezone.c
ttyname.o:      ttyname.c $(IS)types.h $(IS)dir.h $(IS)stat.h
ttyslot.o:      ttyslot.c

abort.o:        abort.s
cuexit.o:       cuexit.s
fakcu.o:        fakcu.s
frexp11.o:      frexp11.s
ldexp11.o:      ldexp11.s
ldfps.o:        ldfps.s
modf11.o:       modf11.s
setjmp.o:       setjmp.s

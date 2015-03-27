#
#       $Log$
#

CFLAGS  = -O
DEST    = /usr/ucb
I       = /usr/include
OBJS    = Mkmf.o buildlist.o depend.o editmf.o hash.o iolin.o macro.o misc.o \
	  suffix.o fatal.o getcwp.o getproject.o mustfopen.o pathcat.o \
	  pperror.o slappend.o slinit.o slsort.o strpcpy.o strsav.o warn.o
SRCS    = l.Makefile p.Makefile t.Makefile
RCS     = RCS


all: mkmf ${SRCS}

mkmf: ${OBJS}
	cc ${OBJS} -o mkmf -lndir
#        strip mkmf

CLEAN CLEAR:
	rm -f *.[ob]

CLRSRC: CLEAN
	rm -f *.[ch] ?.Makefile

install: mkmf ${SRCS}
	install -s mkmf ${DEST}/bin
	cp ${SRCS} ${DEST}/lib

Mkmf.o: ${I}/sys/types.h ${I}/sys/localopts.h ${I}/sys/file.h Mkmf.h \
	getarg.h hash.h null.h path.h target.h slist.h suffix.h \
	system.h yesno.h defaultsfx.h
buildlist.o: ${I}/sys/types.h ${I}/sys/localopts.h ${I}/sys/dir.h \
	     ${I}/stdio.h ${I}/sys/file.h Mkmf.h hash.h null.h path.h \
	     slist.h suffix.h system.h yesno.h ${I}/ndir.h
depend.o: ${I}/sys/types.h ${I}/sys/localopts.h ${I}/ctype.h ${I}/stdio.h \
	  ${I}/sys/file.h Mkmf.h hash.h macro.h null.h path.h slist.h system.h \
	  yesno.h
editmf.o: ${I}/sys/types.h ${I}/sys/localopts.h ${I}/signal.h ${I}/stdio.h \
	  ${I}/sys/file.h Mkmf.h hash.h macro.h null.h slist.h system.h yesno.h
hash.o: ${I}/sys/types.h ${I}/sys/localopts.h null.h hash.h macro.h
iolin.o: ${I}/sys/types.h ${I}/sys/localopts.h ${I}/stdio.h yesno.h
macro.o: ${I}/sys/types.h ${I}/sys/localopts.h ${I}/ctype.h ${I}/stdio.h \
	 Mkmf.h hash.h macro.h null.h slist.h yesno.h
misc.o: ${I}/sys/types.h ${I}/sys/localopts.h ${I}/ctype.h ${I}/stdio.h \
	${I}/sys/file.h Mkmf.h hash.h macro.h null.h path.h target.h spms.h \
	system.h yesno.h
suffix.o: ${I}/stdio.h ${I}/sys/types.h ${I}/sys/localopts.h Mkmf.h hash.h \
	  macro.h null.h suffix.h yesno.h
fatal.c: ${I}/stdio.h
getcwp.c: ${I}/sys/types.h null.h
getproject.c: ${I}/sys/types.h null.h
mustfopen.c: ${I}/stdio.h
pathcat.c: ${I}/stdio.h path.h
pperror.c: ${I}/stdio.h
slappend.c: macro.h null.h slist.h
slinit.c: macro.h null.h slist.h
slsort.c: macro.h null.h slist.h yesno.h
strsav.c: null.h
warn.c: ${I}/stdio.h

Mkmf.h:       ${RCS}/Mkmf.h,v;       co Mkmf.h,v
defaultsfx.h: ${RCS}/defaultsfx.h,v; co defaultsfx.h,v
getarg.h:     ${RCS}/getarg.h,v;     co getarg.h,v
hash.h:       ${RCS}/hash.h,v;       co hash.h,v
macro.h:      ${RCS}/macro.h,v;      co macro.h,v
null.h:       ${RCS}/null.h,v;       co null.h,v
path.h:       ${RCS}/path.h,v;       co path.h,v
slist.h:      ${RCS}/slist.h,v;      co slist.h,v
spms.h:       ${RCS}/spms.h,v;       co spms.h,v
suffix.h:     ${RCS}/suffix.h,v;     co suffix.h,v
system.h:     ${RCS}/system.h,v;     co system.h,v
target.h:     ${RCS}/target.h,v;     co target.h,v
yesno.h:      ${RCS}/yesno.h,v;      co yesno.h,v
Mkmf.c:       ${RCS}/Mkmf.c,v;       co Mkmf.c
buildlist.c:  ${RCS}/buildlist.c,v;  co buildlist.c
depend.c:     ${RCS}/depend.c,v;     co depend.c
editmf.c:     ${RCS}/editmf.c,v;     co editmf.c
hash.c:       ${RCS}/hash.c,v;       co hash.c
iolin.c:      ${RCS}/iolin.c,v;      co iolin.c
macro.c:      ${RCS}/macro.c,v;      co macro.c
misc.c:       ${RCS}/misc.c,v;       co misc.c
suffix.c:     ${RCS}/suffix.c,v;     co suffix.c
l.Makefile:   ${RCS}/l.Makefile,v;   co l.Makefile
p.Makefile:   ${RCS}/p.Makefile,v;   co p.Makefile
t.Makefile:   ${RCS}/t.Makefile,v;   co t.Makefile
fatal.c:      ${RCS}/fatal.c,v;      co fatal.c
getcwp.c:     ${RCS}/getcwp.c,v;     co getcwp.c
getproject.c: ${RCS}/getproject.c,v; co getproject.c
mustfopen.c:  ${RCS}/mustfopen.c,v;  co mustfopen.c
pathcat.c:    ${RCS}/pathcat.c,v;    co pathcat.c
pperror.c:    ${RCS}/pperror.c,v;    co pperror.c
slappend.c:   ${RCS}/slappend.c,v;   co slappend.c
slinit.c:     ${RCS}/slinit.c,v;     co slinit.c
slsort.c:     ${RCS}/slsort.c,v;     co slsort.c
strpcpy.c:    ${RCS}/strpcpy.c,v;    co strpcpy.c
strsav.c:     ${RCS}/strsav.c,v;     co strsav.c
warn.c:       ${RCS}/warn.c,v;       co warn.c

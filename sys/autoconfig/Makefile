#
# Makefile for the system autoconfiguration program
#

OBJS=           args.o attach.o do_config.o main.o read_dtab.o read_nlist.o \
		uprobe1.o uprobe2.o uprobe3.c
CFLAGS=         -O
I=              /usr/include
I1=             ../include
RCS=            RCS

all:            autoconfig

autoconfig:     $(OBJS)
		cc $(LDFLAGS) $(OBJS) -o autoconfig

install:        all
		cp autoconfig ${DESTDIR}/etc
		-strip ${DESTDIR}/etc/autoconfig

clean:
		rm -f autoconfig *.o

args.o: ${I}/stdio.h args.h

attach.o: ${I}/a.out.h ${I}/stdio.h ${I}/sys/autoconfig.h \
	  ${I1}/psw.h dtab.h ivec.h

do_config.o: ${I}/a.out.h ${I}/errno.h ${I}/stdio.h ${I}/sys/autoconfig.h \
	     ${I1}/psw.h ${I1}/trap.h dtab.h ivec.h

main.o: ${I}/stdio.h ${I}/sys/autoconfig.h args.h

read_dtab.o: ${I}/ctype.h ${I}/stdio.h ${I}/sys/autoconfig.h dtab.h uprobe.h

read_nlist.o: ${I}/a.out.h ${I}/stdio.h ${I}/sys/autoconfig.h ${I}/sys/param.h \
	      args.h dtab.h

uprobe1.o: ${I}/sys/autoconfig.h ${I1}/hkreg.h ${I1}/hpreg.h \
	   ${I1}/hsreg.h ${I}/sys/param.h ${I1}/rkreg.h ${I1}/rlreg.h \
	   ${I1}/rpreg.h uprobe.h

uprobe2.o: ${I}/sgtty.h ${I}/sys/autoconfig.h ${I1}/dhreg.h \
	   ${I1}/dnreg.h ${I1}/dzreg.h ${I1}/klreg.h ${I1}/lpreg.h \
	   ${I}/sys/param.h uprobe.h ${I1}/djreg.h

uprobe3.o: uprobe.h ${I1}/htreg.h ${I1}/tmreg.h ${I1}/tsreg.h

args.c:         ${RCS}/args.c,v          ; co args.c
attach.c:       ${RCS}/attach.c,v        ; co attach.c
do_config.c:    ${RCS}/do_config.c,v     ; co do_config.c
main.c:         ${RCS}/main.c,v          ; co main.c
read_dtab.c:    ${RCS}/read_dtab.c,v     ; co read_dtab.c
read_nlist.c:   ${RCS}/read_nlist.c,v    ; co read_nlist.c
uprobe1.c:      ${RCS}/uprobe1.c,v       ; co uprobe1.c
uprobe2.c:      ${RCS}/uprobe2.c,v       ; co uprobe2.c
uprobe3.c:      ${RCS}/uprobe3.c,v       ; co uprobe3.c
args.h:         ${RCS}/args.h,v          ; co args.h
dtab.h:         ${RCS}/dtab.h,v          ; co dtab.h
ivec.h:         ${RCS}/ivec.h,v          ; co ivec.h
uprobe.h:       ${RCS}/uprobe.h,v        ; co uprobe.h

#       Makefile for kernel, overlay version
#
# $Log:	Makefile,v $
# Revision 1.12  88/10/18  14:47:14  korotaev
# Вставлен вход install: копирует include-файлы + cd ../ps ; make install
# 
# Revision 1.11  88/09/26  22:29:06  korotaev
# Стираются ненужные ключи -Z.
#
# Revision 1.10  88/09/26  12:17:12  korotaev
# При включенном IPK_XKOV всегда насильственно втсавляется 444 MAGIC_NUMBER.
#
# Revision 1.9  88/07/15  22:17:21  korotaev
# Убран лишний include у genassym.c
#
# Revision 1.8  88/06/26  15:17:24  korotaev
# Создан единый исходный Makefile для разных машин и для создания
# оверлейного и неоверлейного ядра. Соответсвенно "разбух" и
# стал более интеллектуальным config (следующий шаг для него -
# это создание командного файла задающего вопросы по конфигурации).
#
# Revision 1.7  88/06/13  15:32:31  korotaev
# Изменения связаны с автоматическим построением оверлеев.
#
# Revision 1.8  88/06/13  15:08:54  korotaev
# Глупая, грубая орфографическая (читай синтаксическая) апшибочка.
#
# Revision 1.7  88/06/13  14:09:37  korotaev
# Вставлены изменения в связи с появлением возможности построения
# автоматических списков разбиения на оверлейные куски.
#
# Revision 1.6  88/03/23  14:16:59  korotaev
# Состояние после слияния с АЗЛК, Э-85 и Бурковским планировщиком
#
# Revision 1.6  88/01/13  15:42:07  korotaev
# Исправления ошибок при слиянии ядер ИПК и АЗЛК.
#
# Revision 1.5  86/12/11  20:59:34  alex
#
# Revision 1.4  86/11/23  22:47:13  alex
# Добавлен dj
#
# Revision 1.3  86/11/05  03:53:28  alex
# Взят Makefile имени Антонова.
#
# Revision 1.2  86/11/05  02:23:10  alex
# Изменено размещение разделов для того, чтобы
# ввести kl, tty, ttynew в один раздел.
#
# Revision 1.1  86/04/19  15:01:44  avg
# Initial revision
#
#
#       Makefile Configuration:
#               First, edit the depend-aim to include any optional
#               device drivers you need to include.
#               Then check the global definitions, MAXUSERS, and include file
#               search paths below.
#               INCLUDE is the directory with the normal complement of
#               include files, normally /usr/include.
#               Finally, run "make depend" and stand back.
#
#   For overlay kernel:
#       After having made a demos binary the first time, the overlay
#       structure specified below will probably have to change.
#       The base segment must be between 8194 and 16384 inclusive,
#       and each overlay (31 maximum) must be <= 8192.
#

# global definitions
DEFS =  -DKERNEL

# maxusers is used to scale the size of kernel data structures.
MAXUSERS=%MAXUSERS%

# SPLFIX should be one of :splfix.movb, :splfix.mtps or :splfix.spl
SPLFIX= %SPLFIX%

# include file search path
INCLUDE= /usr/include

# whoami & localopts files location
WLOC=.
LLOC=.

CPPFLAGS= ${DEFS} -I. -I${INCLUDE}
CFLAGS= -O ${CPPFLAGS}
CC =    cc -V
CPP =   /lib/cpp -P
AS =    as -V
LD =    ld

#  shorthand to avoid running make out of space
C =     ${CC} -S ${CFLAGS}
E =     ed - < ${SPLFIX}
A =     ${AS} - -o
I =     ${INCLUDE}

OFILE = demos


# move nami.o into the base segment if possible
# font1.o and video.o (for 85) should be in the same segment
# по возможности надо переместить kl.o в базовый сегмент
OV1 = -Z nami.o fio.o alloc.o iget.o
OV2 = -Z font1.o video.o
VARS= sys4.o text.o ureg.o malloc.o sys1.o mem.o bio.o dkbad.o dkleave.o \
      subr.o rm.o machdep.o sys3.o syslocal.o pipe.o rubin.o ts.o xp.o \
      dh.o dhu.o hs.o ht.o rp.o dsort.o hk.o rl.o fd.o rw.o ildl.o acct.o \
      prf.o rk.o dz.o dj.o tm.o lp.o ttynew.o tty.o partab.o sys.o ioctl.o \
      vd.o main.o rx.o vsys.o pty.o dn.o dvhp.o pc.o uda.o graph.o kx.o \
      sys2.o ky.o vikey.o kl.o
BASE= sigjcl.o slp.o sysent.o clock.o trap.o rdwri.o prim.o font2.o

#       boot.o must be loaded in the base well above 0 data
CONFOBJ = l.o mch.o c.o ioconf.o boot.o


#       l.o MUST be loaded first, param.o MUST be loaded last

## FOR OVERLAY KERNEL:
#${OFILE}: ${OV1} ${OV2} ${BASE} ${CONFOBJ} ${VARS} param.o Makefile \
#          ../conf/o2x checksys
#        sh newvers.sh
#        ${CC} -c vers.c
#        ${LD} -X -n -v ${CONFOBJ} -Q ${VARS} ${OV1}\
#                       ${OV2} -L ${BASE} vers.o param.o
#        -if [ `nm mch.o|sed -n 's/\([^ ]*\).*IPK_XKOV/\1/p;q'` != "000000" ] ;\
#                then ../conf/o2x < a.out > $(OFILE) ; \
#                else mv a.out $(OFILE) ; \
#        fi
#        -rm -f a.out
#        size ${OFILE}
#        ./checksys ${OFILE}
#
#tdemos:
#        ${LD} -X -n -v ${CONFOBJ} -Q ${VARS} ${OV1}\
#                       ${OV2} -L ${BASE} vers.o param.o
#        -if [ `nm mch.o|sed -n 's/\([^ ]*\).*IPK_XKOV/\1/p;q'` != "000000" ] ;\
#                then ../conf/o2x < a.out > $(OFILE) ; \
#                else mv a.out $(OFILE) ; \
#        fi
#        -rm -f a.out
#        size ${OFILE}
#        ./checksys ${OFILE}
#
#../conf/o2x: ../conf/o2x.c ${I}/a.out.h ${I}/stdio.h
#        ${CC} -I. -I${INCLUDE} -O -o ../conf/o2x ../conf/o2x.c
#
#-Z:

## FOR NONOVERLAY KERNEL:
#${OFILE}: ${OV1} ${OV2} ${BASE} ${CONFOBJ} ${VARS} param.o Makefile checksys
#        sh newvers.sh
#        ${CC} -c vers.c
#        ${LD} -X -i -o ${OFILE} ${CONFOBJ} ${VARS} ${OV1} ${OV2}\
#                       ${BASE} vers.o param.o
#        size ${OFILE}
#        ./checksys ${OFILE}
#
#tdemos:
#        ${LD} -X -i -o ${OFILE} ${CONFOBJ} ${VARS} ${OV1} ${OV2}\
#                       ${BASE} vers.o param.o
#        size ${OFILE}
#        ./checksys ${OFILE}

install: demos
	cp localopts.h param.h whoami.h /usr/include/sys
	cd ../ps ; make install clean

param.h: $(WLOC)/whoami.h $(LLOC)/localopts.h ${I}/signal.h ${I}/sys/psw.h
param.h: ${I}/sys/types.h ${I}/sys/iopage.h
	touch param.h

mch.o: assym.s ../sys/mch1.s ../sys/mch2.s $(WLOC)/whoami.h ts.h dz.h tm.h
mch.o: $(LLOC)/localopts.h ${I}/a.out.h ${I}/sys/cpu.m ${I}/sys/koverlay.h
mch.o: ${I}/sys/trap.h ${I}/sys/reboot.h ${I}/sys/iopage.m ht.h
	cat ../sys/mch1.s ../sys/mch2.s | ${CPP} ${CPPFLAGS} >mch.i
	${AS} -o mch.o assym.s mch.i
	-rm mch.i

l.o: l.s $(WLOC)/whoami.h $(LLOC)/localopts.h ${I}/sys/trap.h
l.o: ${I}/sys/iopage.m ${I}/sys/koverlay.h dh.h dhu.h dj.h dn.h dz.h fd.h
l.o: hk.h hp.h hs.h ht.h kl.h kx.h ky.h lp.h pc.h ra.h rk.h rl.h rm.h rp.h
l.o: rw.h rx.h tm.h ts.h video.h xp.h
	${CPP} ${CPPFLAGS} l.s >l.i
	${AS} -o l.o l.i
	-rm l.i

boot.o: ../sys/boot.s $(LLOC)/localopts.h $(WLOC)/whoami.h ${I}/sys/iopage.m
	${CPP} ${CPPFLAGS} ../sys/boot.s >boot.i
	${AS} -o boot.o boot.i
	-rm boot.i

assym.s: genassym.c param.h ${I}/stdio.h ${I}/sys/proc.h ${I}/sys/dir.h
assym.s: ${I}/sys/inode.h ${I}/sys/user.h ${I}/sys/seg.h ${I}/sys/vm.h dz.h
	cc ${CFLAGS} genassym.c; ./a.out >assym.s; rm -f a.out

param.o: param.c param.h Makefile ${I}/sys/systm.h ${I}/sys/buf.h
param.o: ${I}/sys/tty.h ${I}/sys/conf.h ${I}/sys/proc.h ${I}/sys/text.h
param.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/file.h ${I}/sys/inode.h
param.o: ${I}/sys/filsys.h ${I}/sys/mount.h ${I}/sys/callout.h
param.o: ${I}/sys/acct.h ${I}/sys/map.h ${I}/sys/seg.h video.h
	${CC} -S -DMAXUSERS=${MAXUSERS} ${CFLAGS} param.c
	ed - param.s <:comm-to-bss
	${AS} -o param.o param.s
	-rm param.s

checksys: checksys.c param.h ${I}/a.out.h ${I}/stdio.h ${I}/sys/tty.h
	${CC} -I. -I${INCLUDE} -O -o checksys checksys.c

font1.o: ../dev/font1.cm ../dev/font1.mm ../dev/font1fix.c
	mm ../dev/font1.mm < ../dev/font1.cm  >font1.c
	cc -S font1.c
	mv font1.s f.s
	sed 's/data/text/' < f.s >font1.s
	cc -c font1.s ../dev/font1fix.c
	ld -r -o f.o font1.o font1fix.o
	mv f.o font1.o
	-rm -f f.s font1.s font1.c font1fix.o

font2.o: ../dev/font2.cm ../dev/font2.mm
	mm ../dev/font2.mm < ../dev/font2.cm  >font2.c
	cc -c font2.c
	-rm -f font2.c

clean:
	rm -f *.o *.i *.b

#
# Устройства разбиты по шинам. Если одно устройство м.б. на разных шинах
# или это обычный машинно-независимый файл, то его файл сидит в DCOMMON
#
# Если вы хотите добавить новый файл, то вписываете его в OPTIONAL.
#

OPTIONAL=
CONF= c.c ioconf.c
SYS= ../sys/acct.c ../sys/alloc.c ../sys/clock.c ../sys/fio.c ../sys/iget.c ../sys/ioctl.c ../sys/machdep.c ../sys/main.c ../sys/malloc.c \
     ../sys/nami.c ../sys/pipe.c ../sys/prf.c ../sys/prim.c ../sys/rdwri.c ../sys/sigjcl.c ../sys/slp.c ../sys/subr.c ../sys/sys1.c ../sys/sys2.c \
     ../sys/sys3.c ../sys/sys4.c ../sys/sysent.c ../sys/syslocal.c ../sys/text.c ../sys/trap.c ../sys/ureg.c ../sys/vsys.c
DCOMMON= ../dev/bio.c ../dev/dj.c ../dev/dkbad.c ../dev/dkleave.c ../dev/dsort.c ../dev/kl.c ../dev/lp.c ../dev/mem.c ../dev/partab.c ../dev/pty.c ../dev/rl.c ../dev/rubin.c ../dev/sys.c ../dev/ts.c ../dev/tty.c ../dev/ttynew.c ../dev/vd.c
D85= ../dev/fd.c ../dev/graph.c ../dev/kx.c ../dev/ky.c ../dev/rw.c ../dev/video.c ../dev/vikey.c ../dev/ildl.c
DUNIBUS= ../dev/dh.c ../dev/dn.c ../dev/dvhp.c ../dev/dz.c ../dev/hk.c ../dev/hp.c ../dev/hs.c ../dev/ht.c ../dev/pc.c ../dev/rk.c ../dev/rm.c ../dev/rp.c ../dev/rx.c ../dev/tm.c ../dev/xp.c
DQBUS= ../dev/dhu.c ../dev/uda.c
DEV= ${DCOMMON} ${DUNIBUS} ${DQBUS} ${D85}

depend:
	grep '^#include' ${CONF} ${SYS} ${DEV} ${OPTIONAL} |\
	awk 'BEGIN {FMT1="%s\n\t\t$${C} %sc\n\t\t$${E} %ss\n\t\t$${A}";\
		     FMT2=" %so %ss\n\t\t-rm %ss\n";}\
	{FNM=substr($$1,1,length($$1)-10);\
	  if(FNM!=PFN){\
	      if(PFN!=""){printf FMT1,STR,PFN,F;printf FMT2,F,F,F;}\
	      if(substr(FNM,1,3)=="../")F=substr(FNM,8,length(FNM)-7);\
	      else F=FNM;\
	      printf "\n%so: %sc",F,FNM;\
	      STND=sprintf "\n%so: ",F;STR=STND;PFN=FNM;}\
	  if(substr($$2,1,1)=="<")O="$${I}/%s ";else O="%s ";\
	  T=sprintf O,substr($$2,2,length($$2)-2);\
	  if(length(STR T)>77){printf "%s",STR;STR=STND;}\
	  STR=STR T;\
	} END {printf FMT1,STR,PFN,F;printf FMT2,F,F,F;\
	  printf "\n";}' > \#makedep
	echo '/^# DO NOT DELETE THIS LINE/+2,$$d' > \#eddep
	echo '$$r #makedep' >> \#eddep
	echo 'w' >> \#eddep
	cp Makefile Makefile.b
	ed - Makefile < \#eddep
	rm \#eddep \#makedep
	echo '# DEPENDENCIES MUST END HERE' >> Makefile
	echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	echo '# see make depend above' >> Makefile


# DO NOT DELETE THIS LINE


c.o: c.c
c.o: param.h ${I}/sys/systm.h ${I}/sys/buf.h ${I}/sys/tty.h ${I}/sys/conf.h
c.o: ${I}/sys/proc.h ${I}/sys/text.h ${I}/sys/dir.h ${I}/sys/user.h
c.o: ${I}/sys/file.h ${I}/sys/inode.h ${I}/sys/acct.h ${I}/sys/map.h
c.o: ${I}/sys/filsys.h ${I}/sys/mount.h bk.h dh.h dhu.h dj.h dn.h dz.h hk.h
c.o: hp.h hs.h ht.h lp.h pc.h pty.h rk.h rw.h fd.h rl.h rm.h rp.h tm.h ts.h
c.o: xp.h rw.h rx.h ra.h vd.h kl.h rubin.h kx.h ky.h video.h
		${C} c.c
		${E} c.s
		${A} c.o c.s
		-rm c.s

ioconf.o: ioconf.c
ioconf.o: param.h ${I}/sys/systm.h fd.h hk.h hp.h hs.h kx.h ky.h ra.h rk.h
ioconf.o: rl.h rm.h rp.h rw.h rx.h xp.h ${I}/sys/hpreg.h
		${C} ioconf.c
		${E} ioconf.s
		${A} ioconf.o ioconf.s
		-rm ioconf.s

acct.o: ../sys/acct.c
acct.o: param.h ${I}/sys/systm.h ${I}/sys/acct.h ${I}/sys/dir.h
acct.o: ${I}/sys/user.h ${I}/sys/inode.h ${I}/sys/proc.h ${I}/sys/seg.h
		${C} ../sys/acct.c
		${E} acct.s
		${A} acct.o acct.s
		-rm acct.s

alloc.o: ../sys/alloc.c
alloc.o: param.h ${I}/sys/systm.h ${I}/sys/filsys.h ${I}/sys/mount.h
alloc.o: ${I}/sys/fblk.h ${I}/sys/conf.h ${I}/sys/buf.h ${I}/sys/inode.h
alloc.o: ${I}/sys/ino.h ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/quota.h
		${C} ../sys/alloc.c
		${E} alloc.s
		${A} alloc.o alloc.s
		-rm alloc.s

clock.o: ../sys/clock.c
clock.o: param.h ${I}/sys/systm.h ${I}/sys/callout.h ${I}/sys/seg.h
clock.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/proc.h ${I}/sys/reg.h
clock.o: ${I}/sys/text.h ${I}/sys/vm.h
		${C} ../sys/clock.c
		${E} clock.s
		${A} clock.o clock.s
		-rm clock.s

fio.o: ../sys/fio.c
fio.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
fio.o: ${I}/sys/filsys.h ${I}/sys/file.h ${I}/sys/conf.h ${I}/sys/inode.h
fio.o: ${I}/sys/reg.h ${I}/sys/acct.h
		${C} ../sys/fio.c
		${E} fio.s
		${A} fio.o fio.s
		-rm fio.s

iget.o: ../sys/iget.c
iget.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
iget.o: ${I}/sys/inode.h ${I}/sys/ino.h ${I}/sys/filsys.h ${I}/sys/mount.h
iget.o: ${I}/sys/conf.h ${I}/sys/buf.h ${I}/sys/quota.h ${I}/sys/inline.h
		${C} ../sys/iget.c
		${E} iget.s
		${A} iget.o iget.s
		-rm iget.s

ioctl.o: ../sys/ioctl.c
ioctl.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
ioctl.o: ${I}/sys/tty.h ${I}/sys/proc.h ${I}/sys/inode.h ${I}/sys/file.h
ioctl.o: ${I}/sys/reg.h ${I}/sys/conf.h
		${C} ../sys/ioctl.c
		${E} ioctl.s
		${A} ioctl.o ioctl.s
		-rm ioctl.s

machdep.o: ../sys/machdep.c
machdep.o: param.h ${I}/sys/systm.h ${I}/sys/acct.h ${I}/sys/dir.h
machdep.o: ${I}/sys/user.h ${I}/sys/inode.h ${I}/sys/proc.h ${I}/sys/seg.h
machdep.o: ${I}/sys/map.h ${I}/sys/reg.h ${I}/sys/buf.h ${I}/sys/tty.h
machdep.o: ${I}/sys/reboot.h ${I}/sys/uba.h ${I}/sys/iopage.h video.h
		${C} ../sys/machdep.c
		${E} machdep.s
		${A} machdep.o machdep.s
		-rm machdep.s

main.o: ../sys/main.c
main.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
main.o: ${I}/sys/filsys.h ${I}/sys/mount.h ${I}/sys/map.h ${I}/sys/proc.h
main.o: ${I}/sys/inode.h ${I}/sys/seg.h ${I}/sys/conf.h ${I}/sys/buf.h
		${C} ../sys/main.c
		${E} main.s
		${A} main.o main.s
		-rm main.s

malloc.o: ../sys/malloc.c
malloc.o: param.h ${I}/sys/systm.h ${I}/sys/map.h ${I}/sys/vm.h
		${C} ../sys/malloc.c
		${E} malloc.s
		${A} malloc.o malloc.s
		-rm malloc.s

nami.o: ../sys/nami.c
nami.o: param.h ${I}/sys/systm.h ${I}/sys/inode.h ${I}/sys/filsys.h
nami.o: ${I}/sys/mount.h ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/buf.h
nami.o: ${I}/sys/quota.h ${I}/sys/seg.h
		${C} ../sys/nami.c
		${E} nami.s
		${A} nami.o nami.s
		-rm nami.s

pipe.o: ../sys/pipe.c
pipe.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
pipe.o: ${I}/sys/inode.h ${I}/sys/file.h ${I}/sys/reg.h ${I}/sys/inline.h
		${C} ../sys/pipe.c
		${E} pipe.s
		${A} pipe.o pipe.s
		-rm pipe.s

prf.o: ../sys/prf.c
prf.o: param.h ${I}/sys/systm.h ${I}/sys/filsys.h ${I}/sys/mount.h
prf.o: ${I}/sys/seg.h ${I}/sys/buf.h ${I}/sys/conf.h ${I}/sys/inline.h
prf.o: ${I}/sys/reboot.h
		${C} ../sys/prf.c
		${E} prf.s
		${A} prf.o prf.s
		-rm prf.s

prim.o: ../sys/prim.c
prim.o: param.h dh.h dz.h dhu.h ${I}/sys/tty.h ${I}/sys/systm.h
prim.o: ${I}/sys/conf.h ${I}/sys/buf.h ${I}/sys/seg.h ${I}/sys/map.h
prim.o: ${I}/sys/uba.h
		${C} ../sys/prim.c
		${E} prim.s
		${A} prim.o prim.s
		-rm prim.s

rdwri.o: ../sys/rdwri.c
rdwri.o: param.h ${I}/sys/systm.h ${I}/sys/inode.h ${I}/sys/dir.h
rdwri.o: ${I}/sys/user.h ${I}/sys/buf.h ${I}/sys/conf.h
		${C} ../sys/rdwri.c
		${E} rdwri.s
		${A} rdwri.o rdwri.s
		-rm rdwri.s

sigjcl.o: ../sys/sigjcl.c
sigjcl.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
sigjcl.o: ${I}/sys/proc.h ${I}/sys/inode.h ${I}/sys/reg.h ${I}/sys/text.h
sigjcl.o: ${I}/sys/seg.h ${I}/sys/vm.h
		${C} ../sys/sigjcl.c
		${E} sigjcl.s
		${A} sigjcl.o sigjcl.s
		-rm sigjcl.s

slp.o: ../sys/slp.c
slp.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
slp.o: ${I}/sys/proc.h ${I}/sys/text.h ${I}/sys/map.h ${I}/sys/file.h
slp.o: ${I}/sys/inode.h ${I}/sys/buf.h ${I}/sys/seg.h ${I}/sys/vm.h
slp.o: ${I}/sys/inline.h
		${C} ../sys/slp.c
		${E} slp.s
		${A} slp.o slp.s
		-rm slp.s

subr.o: ../sys/subr.c
subr.o: param.h ${I}/sys/systm.h ${I}/sys/conf.h ${I}/sys/inode.h
subr.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/buf.h ${I}/sys/quota.h
		${C} ../sys/subr.c
		${E} subr.s
		${A} subr.o subr.s
		-rm subr.s

sys1.o: ../sys/sys1.c
sys1.o: param.h ${I}/sys/systm.h ${I}/sys/map.h ${I}/sys/dir.h
sys1.o: ${I}/sys/user.h ${I}/sys/proc.h ${I}/sys/buf.h ${I}/sys/reg.h
sys1.o: ${I}/sys/inode.h ${I}/sys/seg.h ${I}/sys/acct.h ${I}/sys/file.h
sys1.o: ${I}/wait.h rubin.h
		${C} ../sys/sys1.c
		${E} sys1.s
		${A} sys1.o sys1.s
		-rm sys1.s

sys2.o: ../sys/sys2.c
sys2.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
sys2.o: ${I}/sys/reg.h ${I}/sys/file.h ${I}/sys/inode.h ${I}/sys/quota.h
sys2.o: ${I}/sys/proc.h ${I}/sys/inline.h
		${C} ../sys/sys2.c
		${E} sys2.s
		${A} sys2.o sys2.s
		-rm sys2.s

sys3.o: ../sys/sys3.c
sys3.o: param.h ${I}/sys/systm.h ${I}/sys/ino.h ${I}/sys/reg.h
sys3.o: ${I}/sys/buf.h ${I}/sys/filsys.h ${I}/sys/mount.h ${I}/sys/dir.h
sys3.o: ${I}/sys/user.h ${I}/sys/inode.h ${I}/sys/file.h ${I}/sys/conf.h
sys3.o: ${I}/sys/stat.h ${I}/sys/inline.h
		${C} ../sys/sys3.c
		${E} sys3.s
		${A} sys3.o sys3.s
		-rm sys3.s

sys4.o: ../sys/sys4.c
sys4.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
sys4.o: ${I}/sys/reg.h ${I}/sys/inode.h ${I}/sys/proc.h ${I}/sys/timeb.h
sys4.o: ${I}/sys/quota.h ${I}/sys/reboot.h ${I}/sys/filsys.h
		${C} ../sys/sys4.c
		${E} sys4.s
		${A} sys4.o sys4.s
		-rm sys4.s

sysent.o: ../sys/sysent.c
sysent.o: param.h ${I}/sys/systm.h ${I}/sys/quota.h
		${C} ../sys/sysent.c
		${E} sysent.s
		${A} sysent.o sysent.s
		-rm sysent.s

syslocal.o: ../sys/syslocal.c
syslocal.o: param.h ${I}/sys/dir.h ${I}/sys/proc.h ${I}/sys/seg.h
syslocal.o: ${I}/sys/user.h ${I}/sys/systm.h ${I}/sys/inode.h
syslocal.o: ${I}/sys/ino.h ${I}/sys/file.h ${I}/sys/conf.h ${I}/sys/tty.h
syslocal.o: ${I}/sys/autoconfig.h
		${C} ../sys/syslocal.c
		${E} syslocal.s
		${A} syslocal.o syslocal.s
		-rm syslocal.s

text.o: ../sys/text.c
text.o: param.h ${I}/sys/systm.h ${I}/sys/map.h ${I}/sys/dir.h
text.o: ${I}/sys/user.h ${I}/sys/proc.h ${I}/sys/text.h ${I}/sys/inode.h
text.o: ${I}/sys/buf.h ${I}/sys/seg.h
		${C} ../sys/text.c
		${E} text.s
		${A} text.o text.s
		-rm text.s

trap.o: ../sys/trap.c
trap.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
trap.o: ${I}/sys/proc.h ${I}/sys/reg.h ${I}/sys/seg.h ${I}/sys/vm.h
trap.o: ${I}/sys/trap.h ${I}/sys/inline.h ${I}/sys/iopage.h
		${C} ../sys/trap.c
		${E} trap.s
		${A} trap.o trap.s
		-rm trap.s

ureg.o: ../sys/ureg.c
ureg.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
ureg.o: ${I}/sys/proc.h ${I}/sys/text.h ${I}/sys/seg.h
		${C} ../sys/ureg.c
		${E} ureg.s
		${A} ureg.o ureg.s
		-rm ureg.s

vsys.o: ../sys/vsys.c
vsys.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
vsys.o: ${I}/sys/proc.h
		${C} ../sys/vsys.c
		${E} vsys.s
		${A} vsys.o vsys.s
		-rm vsys.s

bio.o: ../dev/bio.c
bio.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
bio.o: ${I}/sys/buf.h ${I}/sys/conf.h ${I}/sys/proc.h ${I}/sys/seg.h
bio.o: ${I}/sys/vm.h ${I}/sys/uba.h
		${C} ../dev/bio.c
		${E} bio.s
		${A} bio.o bio.s
		-rm bio.s

dj.o: ../dev/dj.c
dj.o: dj.h param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h
dj.o: ${I}/sys/tty.h ${I}/sys/djreg.h
		${C} ../dev/dj.c
		${E} dj.s
		${A} dj.o dj.s
		-rm dj.s

dkbad.o: ../dev/dkbad.c
dkbad.o: param.h ${I}/sys/buf.h ${I}/sys/dkbad.h
		${C} ../dev/dkbad.c
		${E} dkbad.s
		${A} dkbad.o dkbad.s
		-rm dkbad.s

dkleave.o: ../dev/dkleave.c
dkleave.o: param.h ${I}/sys/buf.h
		${C} ../dev/dkleave.c
		${E} dkleave.s
		${A} dkleave.o dkleave.s
		-rm dkleave.s

dsort.o: ../dev/dsort.c
dsort.o: param.h ${I}/sys/systm.h ${I}/sys/buf.h
		${C} ../dev/dsort.c
		${E} dsort.s
		${A} dsort.o dsort.s
		-rm dsort.s

kl.o: ../dev/kl.c
kl.o: kl.h param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h
kl.o: ${I}/sys/tty.h ${I}/sys/systm.h ${I}/sys/klreg.h video.h
kl.o: ${I}/sys/psw.h
		${C} ../dev/kl.c
		${E} kl.s
		${A} kl.o kl.s
		-rm kl.s

lp.o: ../dev/lp.c
lp.o: lp.h param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
lp.o: ${I}/sys/tty.h ${I}/sys/file.h ${I}/sys/lpreg.h
		${C} ../dev/lp.c
		${E} lp.s
		${A} lp.o lp.s
		-rm lp.s

mem.o: ../dev/mem.c
mem.o: param.h ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/conf.h
mem.o: ${I}/sys/seg.h
		${C} ../dev/mem.c
		${E} mem.s
		${A} mem.o mem.s
		-rm mem.s

partab.o: ../dev/partab.c
partab.o: param.h ${I}/sys/tty.h
		${C} ../dev/partab.c
		${E} partab.s
		${A} partab.o partab.s
		-rm partab.s

pty.o: ../dev/pty.c
pty.o: pty.h param.h ${I}/sys/systm.h ${I}/sys/tty.h ${I}/sys/dir.h
pty.o: ${I}/sys/user.h ${I}/sys/conf.h ${I}/sys/buf.h ${I}/sys/file.h
pty.o: ${I}/sys/proc.h
		${C} ../dev/pty.c
		${E} pty.s
		${A} pty.o pty.s
		-rm pty.s

rl.o: ../dev/rl.c
rl.o: rl.h param.h ${I}/sys/buf.h ${I}/sys/dir.h ${I}/sys/user.h
rl.o: ${I}/sys/systm.h ${I}/sys/conf.h ${I}/sys/rlreg.h
		${C} ../dev/rl.c
		${E} rl.s
		${A} rl.o rl.s
		-rm rl.s

rubin.o: ../dev/rubin.c
rubin.o: param.h ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/proc.h rubin.h
rubin.o: ${I}/sys/ildr.h
		${C} ../dev/rubin.c
		${E} rubin.s
		${A} rubin.o rubin.s
		-rm rubin.s

sys.o: ../dev/sys.c
sys.o: param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h
sys.o: ${I}/sys/tty.h ${I}/sys/proc.h
		${C} ../dev/sys.c
		${E} sys.s
		${A} sys.o sys.s
		-rm sys.s

ts.o: ../dev/ts.c
ts.o: ts.h param.h ${I}/sys/buf.h ${I}/sys/dir.h ${I}/sys/conf.h
ts.o: ${I}/sys/file.h ${I}/sys/user.h ${I}/sys/tsreg.h ${I}/sys/mtio.h
		${C} ../dev/ts.c
		${E} ts.s
		${A} ts.o ts.s
		-rm ts.s

tty.o: ../dev/tty.c
tty.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
tty.o: ${I}/sys/tty.h ${I}/sys/proc.h ${I}/sys/inode.h ${I}/sys/file.h
tty.o: ${I}/sys/reg.h ${I}/sys/conf.h ${I}/sys/buf.h bk.h
		${C} ../dev/tty.c
		${E} tty.s
		${A} tty.o tty.s
		-rm tty.s

ttynew.o: ../dev/ttynew.c
ttynew.o: param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
ttynew.o: ${I}/sys/tty.h ${I}/sys/proc.h ${I}/sys/inode.h ${I}/sys/file.h
ttynew.o: ${I}/sys/reg.h ${I}/sys/conf.h ${I}/sys/buf.h
		${C} ../dev/ttynew.c
		${E} ttynew.s
		${A} ttynew.o ttynew.s
		-rm ttynew.s

vd.o: ../dev/vd.c
vd.o: vd.h param.h ${I}/sys/systm.h ${I}/sys/buf.h ${I}/sys/conf.h
vd.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/seg.h ${I}/sys/iopage.h
vd.o: ${I}/sys/map.h
		${C} ../dev/vd.c
		${E} vd.s
		${A} vd.o vd.s
		-rm vd.s

dh.o: ../dev/dh.c
dh.o: dh.h param.h ${I}/sys/conf.h ${I}/sys/systm.h ${I}/sys/dir.h
dh.o: ${I}/sys/user.h ${I}/sys/file.h ${I}/sys/tty.h ${I}/sys/dhreg.h
dh.o: ${I}/sys/uba.h
		${C} ../dev/dh.c
		${E} dh.s
		${A} dh.o dh.s
		-rm dh.s

dn.o: ../dev/dn.c
dn.o: dn.h param.h ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/dnreg.h
		${C} ../dev/dn.c
		${E} dn.s
		${A} dn.o dn.s
		-rm dn.s

dvhp.o: ../dev/dvhp.c
dvhp.o: dvhp.h param.h ${I}/sys/systm.h ${I}/sys/buf.h ${I}/sys/conf.h
dvhp.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/hpreg.h ${I}/sys/inline.h
dvhp.o: ${I}/sys/uba.h
		${C} ../dev/dvhp.c
		${E} dvhp.s
		${A} dvhp.o dvhp.s
		-rm dvhp.s

dz.o: ../dev/dz.c
dz.o: dz.h param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
dz.o: ${I}/sys/tty.h ${I}/sys/file.h ${I}/sys/conf.h ${I}/sys/dzreg.h
dz.o: ${I}/sys/pdma.h
		${C} ../dev/dz.c
		${E} dz.s
		${A} dz.o dz.s
		-rm dz.s

hk.o: ../dev/hk.c
hk.o: hk.h param.h ${I}/sys/systm.h ${I}/sys/buf.h ${I}/sys/conf.h
hk.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/uba.h ${I}/sys/inline.h
hk.o: ${I}/sys/hkreg.h ${I}/sys/dkbad.h ${I}/sys/psw.h
		${C} ../dev/hk.c
		${E} hk.s
		${A} hk.o hk.s
		-rm hk.s

hp.o: ../dev/hp.c
hp.o: hp.h param.h ${I}/sys/systm.h ${I}/sys/buf.h ${I}/sys/conf.h
hp.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/seg.h ${I}/sys/hpreg.h
hp.o: ${I}/sys/inline.h ${I}/sys/uba.h
		${C} ../dev/hp.c
		${E} hp.s
		${A} hp.o hp.s
		-rm hp.s

hs.o: ../dev/hs.c
hs.o: hs.h param.h ${I}/sys/systm.h ${I}/sys/buf.h ${I}/sys/conf.h
hs.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/seg.h ${I}/sys/hsreg.h
		${C} ../dev/hs.c
		${E} hs.s
		${A} hs.o hs.s
		-rm hs.s

ht.o: ../dev/ht.c
ht.o: ht.h param.h ${I}/sys/systm.h ${I}/sys/buf.h ${I}/sys/conf.h
ht.o: ${I}/sys/dir.h ${I}/sys/file.h ${I}/sys/user.h ${I}/sys/htreg.h
ht.o: ${I}/sys/mtio.h
		${C} ../dev/ht.c
		${E} ht.s
		${A} ht.o ht.s
		-rm ht.s

pc.o: ../dev/pc.c
pc.o: pc.h param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
pc.o: ${I}/sys/tty.h ${I}/sys/file.h ${I}/sys/pcreg.h
		${C} ../dev/pc.c
		${E} pc.s
		${A} pc.o pc.s
		-rm pc.s

rk.o: ../dev/rk.c
rk.o: rk.h param.h ${I}/sys/systm.h ${I}/sys/buf.h ${I}/sys/conf.h
rk.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/rkreg.h ${I}/sys/psw.h
		${C} ../dev/rk.c
		${E} rk.s
		${A} rk.o rk.s
		-rm rk.s

rm.o: ../dev/rm.c
rm.o: rm.h param.h ${I}/sys/systm.h ${I}/sys/buf.h ${I}/sys/conf.h
rm.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/hpreg.h ${I}/sys/inline.h
rm.o: ${I}/sys/seg.h ${I}/sys/uba.h
		${C} ../dev/rm.c
		${E} rm.s
		${A} rm.o rm.s
		-rm rm.s

rp.o: ../dev/rp.c
rp.o: rp.h param.h ${I}/sys/systm.h ${I}/sys/buf.h ${I}/sys/dir.h
rp.o: ${I}/sys/conf.h ${I}/sys/user.h ${I}/sys/rpreg.h ${I}/sys/psw.h
		${C} ../dev/rp.c
		${E} rp.s
		${A} rp.o rp.s
		-rm rp.s

rx.o: ../dev/rx.c
rx.o: rx.h param.h ${I}/sys/buf.h ${I}/sys/conf.h ${I}/sys/dir.h
rx.o: ${I}/sys/user.h ${I}/sys/rxreg.h ${I}/sys/seg.h
		${C} ../dev/rx.c
		${E} rx.s
		${A} rx.o rx.s
		-rm rx.s

tm.o: ../dev/tm.c
tm.o: tm.h param.h ${I}/sys/buf.h ${I}/sys/dir.h ${I}/sys/conf.h
tm.o: ${I}/sys/file.h ${I}/sys/user.h ${I}/sys/systm.h ${I}/sys/tmreg.h
tm.o: ${I}/sys/mtio.h
		${C} ../dev/tm.c
		${E} tm.s
		${A} tm.o tm.s
		-rm tm.s

xp.o: ../dev/xp.c
xp.o: xp.h param.h ${I}/sys/systm.h ${I}/sys/buf.h ${I}/sys/conf.h
xp.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/seg.h ${I}/sys/hpreg.h
xp.o: ${I}/sys/inline.h ${I}/sys/uba.h
		${C} ../dev/xp.c
		${E} xp.s
		${A} xp.o xp.s
		-rm xp.s

dhu.o: ../dev/dhu.c
dhu.o: dhu.h param.h ${I}/sys/conf.h ${I}/sys/systm.h ${I}/sys/dir.h
dhu.o: ${I}/sys/user.h ${I}/sys/file.h ${I}/sys/tty.h ${I}/sys/dhureg.h
dhu.o: ${I}/sys/uba.h
		${C} ../dev/dhu.c
		${E} dhu.s
		${A} dhu.o dhu.s
		-rm dhu.s

uda.o: ../dev/uda.c
uda.o: ra.h param.h ${I}/sys/systm.h ${I}/sys/buf.h ${I}/sys/dir.h
uda.o: ${I}/sys/conf.h ${I}/sys/file.h ${I}/sys/user.h ${I}/sys/map.h
uda.o: ${I}/sys/inline.h ${I}/sys/ra_info.h
		${C} ../dev/uda.c
		${E} uda.s
		${A} uda.o uda.s
		-rm uda.s

fd.o: ../dev/fd.c
fd.o: fd.h param.h ${I}/sys/systm.h ${I}/sys/buf.h ${I}/sys/conf.h
fd.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/seg.h ${I}/sys/iopage.h
		${C} ../dev/fd.c
		${E} fd.s
		${A} fd.o fd.s
		-rm fd.s

graph.o: ../dev/graph.c
graph.o: video.h
		${C} ../dev/graph.c
		${E} graph.s
		${A} graph.o graph.s
		-rm graph.s

kx.o: ../dev/kx.c
kx.o: kx.h param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h
kx.o: ${I}/sys/tty.h ${I}/sys/systm.h ${I}/sys/iopage.h
		${C} ../dev/kx.c
		${E} kx.s
		${A} kx.o kx.s
		-rm kx.s

ky.o: ../dev/ky.c
ky.o: ky.h param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h
ky.o: ${I}/sys/tty.h ${I}/sys/systm.h ${I}/sys/iopage.h
		${C} ../dev/ky.c
		${E} ky.s
		${A} ky.o ky.s
		-rm ky.s

rw.o: ../dev/rw.c
rw.o: rw.h param.h ${I}/sys/systm.h ${I}/sys/buf.h ${I}/sys/conf.h
rw.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/seg.h ${I}/sys/iopage.h
		${C} ../dev/rw.c
		${E} rw.s
		${A} rw.o rw.s
		-rm rw.s

video.o: ../dev/video.c
video.o: video.h param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h
video.o: ${I}/sys/tty.h ${I}/sys/systm.h ${I}/sys/iopage.h
		${C} ../dev/video.c
		${E} video.s
		${A} video.o video.s
		-rm video.s

vikey.o: ../dev/vikey.c
vikey.o: video.h param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h
vikey.o: ${I}/sys/tty.h
		${C} ../dev/vikey.c
		${E} vikey.s
		${A} vikey.o vikey.s
		-rm vikey.s

ildl.o: ../dev/ildl.c
ildl.o: video.h param.h ${I}/sys/seg.h
		${C} ../dev/ildl.c
		${E} ildl.s
		${A} ildl.o ildl.s
		-rm ildl.s

# DEPENDENCIES MUST END HERE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above

#       Makefile for kernel (overlay version)
#

# global definitions
DEFS =  -DKERNEL

# SPLFIX should be one of :splfix.movb, :splfix.mtps or :splfix.spl
SPLFIX=%SPLFIX%

# include file search path
INCLUDE= /usr/include

CPPFLAGS= ${DEFS} -I. -I${INCLUDE}
CFLAGS= -O ${CPPFLAGS} -V
CC =    cc
CPP =   /lib/cpp -P
AS =    as -V
LD =    ld

#  shorthand to avoid running make out of space
C =     ${CC} -S ${CFLAGS}
E =     ed - < ../conf/${SPLFIX}
A =     ${AS} - -o
I =     ${INCLUDE}

OFILE = demos

# move nami.o into the base segment if possible
# font1.o and video.o (for 85) should be in the same segment
#
OV1 = font1.o video.o
VARS= fd.o dj.o rp.o kx.o hk.o rk.o tm.o lp.o main.o rm.o sys5.o ipc5.o\
      sem.o ky.o dh.o pty.o fcntl.o msg.o ts.o xp.o dhu.o hs.o ht.o rl.o\
      rw.o ildl.o dz.o vd.o rx.o dn.o dvhp.o pc.o uda.o graph.o vikey.o\
      sigjcl.o mpy.o wl.o my.o dw.o
VARS1=ureg.o sys1.o mem.o dkbad.o dkleave.o sys3.o pipe.o dsort.o acct.o\
      prf.o partab.o sys.o vsys.o machdep.o nami.o fio.o alloc.o iget.o\
      kl.o tty.o ioctl.o ttynew.o text.o malloc.o bio.o subr.o rdwri.o\
      sys2.o syslocal.o sys4.o slp.o sysent.o clock.o trap.o prim.o mk.o ks.o
BASE= font2.o

#       boot.o must be loaded in the base well above 0 data
CONFOBJ = l.o mch.o c.o ioconf.o boot.o


#       l.o MUST be loaded first, param.o MUST be loaded last

# FOR OVERLAY KERNEL:
${OFILE}: ${BASE} ${CONFOBJ} ${VARS} ${VARS1} ${OV1} param.o Makefile\
	  ../conf/o2x checksys
	sh newvers.sh
	${CC} -c vers.c
	${LD} -X -n -v ${CONFOBJ} -Q ${VARS1} -Q ${VARS} -Z ${OV1} -L ${BASE} vers.o param.o
	-if [ `nm mch.o|sed -n 's/^\([^ ]*\) a IPK_XKOV/\1/p'` != "000000" ] ;\
		then ../conf/o2x < a.out > $(OFILE) ; \
		else mv a.out $(OFILE) ; \
	fi
	-rm -f a.out
	size ${OFILE}
	./checksys ${OFILE}

d${OFILE}: vers.o ${BASE} ${CONFOBJ} ${VARS} ${VARS1} ${OV1} param.o Makefile ../conf/o2x checksys
	${LD} -X -n -v ${CONFOBJ} -Q ${VARS1} -Q ${VARS} -Z ${OV1} -L ${BASE} vers.o param.o
	-if [ `nm mch.o|sed -n 's/^\([^ ]*\) a IPK_XKOV/\1/p'` != "000000" ] ;\
		then ../conf/o2x < a.out > $(OFILE) ; \
		else mv a.out $(OFILE) ; \
	fi
	-rm -f a.out
	size ${OFILE}
	./checksys ${OFILE}

../conf/o2x: ../conf/o2x.c ${I}/a.out.h ${I}/stdio.h
	${CC} -I. -I${INCLUDE} -O -o ../conf/o2x ../conf/o2x.c

## FOR NONOVERLAY KERNEL:
#${OFILE}: ${BASE} ${CONFOBJ} ${VARS} ${OV1} param.o Makefile checksys
#        sh newvers.sh
#        ${CC} -c vers.c
#        ${LD} -X -i -o ${OFILE} ${CONFOBJ} ${VARS} ${OV1} ${BASE} vers.o param.o
#        size ${OFILE}
#        ./checksys ${OFILE}
#
#tdemos:
#        ${LD} -X -i -o ${OFILE} ${CONFOBJ} ${VARS} ${OV1} ${BASE} vers.o param.o
#        size ${OFILE}
#        ./checksys ${OFILE}

install: demos
	cp h/localopts.h h/whoami.h /usr/include/sys
	cd ../ps ; make install clean
	mv /demos /demos.old
	cp demos /demos

h/whoami.h: h/localopts.h
	touch $@

h/space.h: h/video.h
	touch $@

${I}/sys/sem.h: ${I}/sys/ipc.h
	touch $@

${I}/sys/tty.h: ${I}/sys/ioctl.h ${I}/sgtty.h
	touch $@

${I}/sys/user.h: ${I}/errno.h ${I}/sys/fperr.h ${I}/sys/dir.h ${I}/a.out.h
	touch $@

h/param.h: h/whoami.h ${I}/signal.h ../include/psw.h ${I}/sys/types.h
h/param.h: ../include/iopage.h ${I}/sys/param.h
	touch $@

vers.o: version newvers.sh
	sh newvers.sh
	${CC} -c vers.c

version:
	cat /dev/null > version

mch.o: assym.s ../sys/mch1.s ../sys/mch2.s
	cat ../sys/mch1.s ../sys/mch2.s | ${CPP} ${CPPFLAGS} >mch.i
	${AS} -o mch.o assym.s mch.i
	-rm -f mch.i

l.o:
	${CPP} ${CPPFLAGS} ../conf/l.s >l.i
	${AS} -o l.o l.i
	-rm -f l.i

boot.o: ../sys/boot.s
	${CPP} ${CPPFLAGS} ../sys/boot.s >boot.i
	${AS} -o boot.o boot.i
	-rm -f boot.i

assym.s: ../conf/genassym.c h/whoami.h h/param.h ${I}/stdio.h
assym.s: ${I}/sys/dir.h ${I}/sys/inode.h ${I}/sys/user.h ${I}/sys/seg.h
assym.s: ${I}/sys/vm.h h/dz.h ${I}/sys/proc.h
	cc ${CFLAGS} ../conf/genassym.c
	./a.out > assym.s
	-rm -f a.out

param.o:
	${CC} -S ${CFLAGS} ../sys/param.c
	ed - param.s <../conf/:comm-to-bss
	${AS} -o param.o param.s
	-rm -f param.s

font1.o RULE1: ../dev/font1.cm ../dev/font1.mm
	mm ../dev/font1.mm < ../dev/font1.cm  >font1.c
	$(CC) -S font1.c
	echo '1s/_/__/%2a%__Font1: _Font1%.text%.%w%q' | tr % '\012' | ed - font1.s
	$(CC) -c font1.s
	-rm -f font1.s font1.c

font2.o RULE2: ../dev/font2.cm ../dev/font2.mm
	mm ../dev/font2.mm < ../dev/font2.cm  >font2.c
	cc -c font2.c
	-rm -f font2.c

checksys: ../conf/checksys.c h/param.h ${I}/a.out.h ${I}/stdio.h
checksys: ${I}/sys/tty.h ${I}/sys/fcntl.h ${I}/ediag.h
	${CC} -I. -I${INCLUDE} -O -o checksys ../conf/checksys.c

clean:
	-rm -f *.[oibs]

#
# Устройства разбиты по шинам. Если одно устройство м.б. на разных шинах
# или это обычный машинно-независимый файл, то его файл сидит в DCOMMON.
# Если вы хотите добавить новый файл, то вписываете его в OPTIONAL.
#

OPTIONAL=
CONF= ../conf/c.c ioconf.c ../conf/l.s
SYS= ../sys/acct.c ../sys/alloc.c ../sys/clock.c ../sys/fio.c ../sys/iget.c\
 ../sys/ioctl.c ../sys/machdep.c ../sys/main.c ../sys/malloc.c ../sys/nami.c\
 ../sys/pipe.c ../sys/prf.c ../sys/prim.c ../sys/rdwri.c ../sys/sigjcl.c\
 ../sys/slp.c ../sys/subr.c ../sys/sys1.c ../sys/sys2.c ../sys/sys3.c\
 ../sys/sys4.c ../sys/sysent.c ../sys/syslocal.c ../sys/text.c ../sys/trap.c\
 ../sys/ureg.c ../sys/vsys.c ../sys/sys5.c ../sys/ipc5.c ../sys/fcntl.c\
 ../sys/msg.c ../sys/mch1.s ../sys/mch2.s ../sys/boot.s ../sys/sem.c\
 ../sys/param.c
DEV= ../dev/bio.c ../dev/dh.c ../dev/dj.c ../dev/dn.c ../dev/dz.c ../dev/hk.c\
 ../dev/dkbad.c ../dev/hp.c ../dev/hs.c ../dev/ht.c ../dev/lp.c ../dev/pc.c\
 ../dev/dkleave.c ../dev/rk.c ../dev/rm.c ../dev/rp.c ../dev/rx.c ../dev/tm.c\
 ../dev/dsort.c ../dev/dvhp.c ../dev/kl.c ../dev/ts.c ../dev/xp.c ../dev/fd.c\
 ../dev/sys.c ../dev/ildl.c ../dev/kx.c ../dev/ky.c ../dev/rw.c ../dev/wl.c\
 ../dev/tty.c ../dev/mpy.c ../dev/dw.c ../dev/ks.c ../dev/mk.c ../dev/my.c\
 ../dev/partab.c ../dev/pty.c ../dev/rl.c ../dev/dhu.c ../dev/uda.c\
 ../dev/mem.c ../dev/graph.c ../dev/vikey.c ../dev/vd.c ../dev/video.c\
 ../dev/ttynew.c

depend:
	grep '^#include' ${CONF} ${SYS} ${DEV} ${OPTIONAL} |\
	awk -f ../conf/Awk.depend > \#makedep
	echo '/^# DO NOT DELETE THIS LINE/+2,$$d' > \#eddep
	echo '$$r #makedep' >> \#eddep
	echo 'w' >> \#eddep
	cp Makefile Makefile.b
	ed - Makefile < \#eddep
	-rm -f \#eddep \#makedep
	echo '# DEPENDENCIES MUST END HERE' >> Makefile
	echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	echo '# see make depend above' >> Makefile

c.o: ../conf/c.c
		cp ../conf/c.c c.c
		${C} c.c
		${E} c.s
		${A} c.o c.s
		-rm -f c.[cs]

ioconf.o:
	${C} ioconf.c
	${E} ioconf.s
	${A} ioconf.o ioconf.s
	-rm -f ioconf.s

acct.o alloc.o clock.o fio.o iget.o ioctl.o machdep.o main.o malloc.o nami.o \
pipe.o prf.o prim.o rdwri.o sem.o sigjcl.o slp.o subr.o sys1.o sys2.o sys3.o \
sys4.o sysent.o syslocal.o text.o trap.o ureg.o vsys.o sys5.o ipc5.o fcntl.o \
msg.o:
	${C} ../sys/$(@@:.o=.c)
	${E} $(@@:.o=.s)
	${A} $@ $(@@:.o=.s)
	rm -f $(@@:.o=.s)

bio.o dkbad.o dkleave.o dsort.o dvhp.o kl.o mem.o partab.o pty.o rl.o \
tty.o ttynew.o mpy.o vd.o dh.o dj.o dn.o dz.o hk.o hp.o ks.o mk.o:
	${C} ../dev/$(@@:.o=.c)
	${E} $(@@:.o=.s)
	${A} $@ $(@@:.o=.s)
	rm -f $(@@:.o=.s)

sys.o hs.o ht.o lp.o pc.o rk.o rm.o rp.o rx.o tm.o ts.o xp.o fd.o graph.o \
ildl.o kx.o ky.o rw.o video.o vikey.o dhu.o uda.o wl.o dw.o my.o:
	${C} ../dev/$(@@:.o=.c)
	${E} $(@@:.o=.s)
	${A} $@ $(@@:.o=.s)
	rm -f $(@@:.o=.s)


# DO NOT DELETE THIS LINE
# DEPENDENCIES MUST END HERE
c.o: ../conf/c.c
c.o: h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/tty.h ${I}/sys/conf.h
c.o: ${I}/sys/proc.h ${I}/sys/text.h ${I}/sys/dir.h ${I}/sys/user.h
c.o: ${I}/sys/file.h ${I}/sys/inode.h ${I}/sys/acct.h ../include/map.h
c.o: ${I}/sys/filsys.h ../include/mount.h h/bk.h h/dh.h h/dhu.h h/dj.h h/dn.h
c.o: h/dz.h h/hk.h h/hp.h h/hs.h h/ht.h h/lp.h h/pc.h h/pty.h h/mpy.h h/rk.h
c.o: h/rw.h h/dw.h h/my.h h/fd.h h/rl.h h/rm.h h/rp.h h/tm.h h/ts.h h/xp.h
c.o: h/rx.h h/ra.h h/vd.h h/ks.h h/kl.h h/wl.h h/kx.h h/ky.h h/video.h h/mk.h
ioconf.o: ioconf.c
ioconf.o: h/param.h ${I}/sys/systm.h h/dw.h h/fd.h h/hk.h h/hp.h h/hs.h h/kx.h
ioconf.o: h/ky.h h/my.h h/ra.h h/rk.h h/rl.h h/rm.h h/rp.h h/rw.h h/rx.h h/xp.h
ioconf.o: ../include/hpreg.h ../include/ra_info.h
l.o: ../conf/l.s
l.o: h/whoami.h ../include/trap.h ../include/iopage.m ../include/koverlay.h
l.o: h/dh.h h/dhu.h h/dj.h h/dn.h h/dw.h h/dz.h h/fd.h h/hk.h h/hp.h h/hs.h
l.o: h/ht.h h/kl.h h/ks.h h/kx.h h/ky.h h/lp.h h/mk.h h/my.h h/pc.h h/ra.h
l.o: h/rk.h h/rl.h h/rm.h h/rp.h h/rw.h h/rx.h h/tm.h h/ts.h h/video.h h/wl.h
l.o: h/xp.h
acct.o: ../sys/acct.c
acct.o: h/param.h ${I}/sys/systm.h ${I}/sys/acct.h ${I}/sys/dir.h
acct.o: ${I}/sys/user.h ${I}/sys/inode.h ${I}/sys/proc.h ${I}/sys/seg.h
alloc.o: ../sys/alloc.c
alloc.o: h/param.h ${I}/sys/systm.h ${I}/sys/filsys.h ../include/mount.h
alloc.o: ${I}/sys/fblk.h ${I}/sys/conf.h ../include/buf.h ${I}/sys/inode.h
alloc.o: ${I}/sys/ino.h ${I}/sys/dir.h ${I}/sys/user.h ../include/quota.h
clock.o: ../sys/clock.c
clock.o: h/param.h ${I}/sys/systm.h ../include/callout.h ${I}/sys/seg.h
clock.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/proc.h ../include/reg.h
clock.o: ../include/psw.h ${I}/sys/text.h ${I}/sys/vm.h
fio.o: ../sys/fio.c
fio.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
fio.o: ${I}/sys/filsys.h ${I}/sys/file.h ${I}/sys/conf.h ${I}/sys/inode.h
fio.o: ../include/reg.h ${I}/sys/acct.h
iget.o: ../sys/iget.c
iget.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
iget.o: ${I}/sys/inode.h ${I}/sys/ino.h ${I}/sys/filsys.h ../include/mount.h
iget.o: ${I}/sys/conf.h ../include/buf.h ../include/quota.h ../include/inline.h
ioctl.o: ../sys/ioctl.c
ioctl.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
ioctl.o: ${I}/sys/tty.h ${I}/sys/proc.h ${I}/sys/inode.h ${I}/sys/file.h
ioctl.o: ../include/reg.h ${I}/sys/conf.h ${I}/sys/ioctl.h ${I}/sys/fcntl.h
machdep.o: ../sys/machdep.c
machdep.o: h/param.h ${I}/sys/systm.h ${I}/sys/acct.h ${I}/sys/dir.h
machdep.o: ${I}/sys/user.h ${I}/sys/inode.h ${I}/sys/proc.h ${I}/sys/seg.h
machdep.o: ../include/map.h ../include/reg.h ../include/buf.h ../include/psw.h
machdep.o: ${I}/sys/tty.h ${I}/sys/reboot.h ../include/uba.h
machdep.o: ../include/iopage.h ${I}/sys/fcntl.h h/video.h
main.o: ../sys/main.c
main.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
main.o: ${I}/sys/filsys.h ../include/mount.h ../include/map.h ${I}/sys/proc.h
main.o: ${I}/sys/inode.h ${I}/sys/seg.h ${I}/sys/conf.h ../include/buf.h
main.o: ${I}/sys/file.h h/space.h ${I}/sys/sem.h ${I}/sys/msg.h
malloc.o: ../sys/malloc.c
malloc.o: h/param.h ${I}/sys/systm.h ../include/map.h ${I}/sys/vm.h
nami.o: ../sys/nami.c
nami.o: h/param.h ${I}/sys/systm.h ${I}/sys/inode.h ${I}/sys/filsys.h
nami.o: ../include/mount.h ${I}/sys/dir.h ${I}/sys/user.h ../include/buf.h
nami.o: ../include/quota.h ${I}/sys/seg.h
pipe.o: ../sys/pipe.c
pipe.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
pipe.o: ${I}/sys/inode.h ${I}/sys/file.h ../include/reg.h ../include/inline.h
prf.o: ../sys/prf.c
prf.o: h/param.h ${I}/sys/systm.h ${I}/sys/filsys.h ../include/mount.h
prf.o: ${I}/sys/seg.h ../include/buf.h ${I}/sys/conf.h ../include/inline.h
prf.o: ${I}/sys/user.h ${I}/sys/proc.h ${I}/sys/reboot.h
prim.o: ../sys/prim.c
prim.o: h/param.h h/dh.h h/dz.h h/dhu.h ${I}/sys/tty.h ${I}/sys/systm.h
prim.o: ${I}/sys/conf.h ../include/buf.h ${I}/sys/seg.h ../include/map.h
prim.o: ../include/uba.h
rdwri.o: ../sys/rdwri.c
rdwri.o: h/param.h ${I}/sys/systm.h ${I}/sys/inode.h ${I}/sys/dir.h
rdwri.o: ${I}/sys/user.h ../include/buf.h ${I}/sys/conf.h ${I}/sys/file.h
sigjcl.o: ../sys/sigjcl.c
sigjcl.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
sigjcl.o: ${I}/sys/proc.h ${I}/sys/inode.h ../include/reg.h ../include/psw.h
sigjcl.o: ${I}/sys/text.h ${I}/sys/seg.h ${I}/sys/vm.h
slp.o: ../sys/slp.c
slp.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
slp.o: ${I}/sys/proc.h ${I}/sys/text.h ../include/map.h ${I}/sys/file.h
slp.o: ${I}/sys/inode.h ../include/buf.h ${I}/sys/seg.h ${I}/sys/vm.h
slp.o: ../include/inline.h
subr.o: ../sys/subr.c
subr.o: h/param.h ${I}/sys/systm.h ${I}/sys/conf.h ${I}/sys/inode.h
subr.o: ${I}/sys/dir.h ${I}/sys/user.h ../include/buf.h ../include/quota.h
sys1.o: ../sys/sys1.c
sys1.o: h/param.h ${I}/sys/systm.h ../include/map.h ${I}/sys/dir.h
sys1.o: ${I}/sys/user.h ${I}/sys/proc.h ../include/buf.h ../include/reg.h
sys1.o: ../include/psw.h ${I}/sys/inode.h ${I}/sys/seg.h ${I}/sys/acct.h
sys1.o: ${I}/sys/file.h ${I}/sys/wait.h h/space.h
sys2.o: ../sys/sys2.c
sys2.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
sys2.o: ../include/reg.h ${I}/sys/file.h ${I}/sys/inode.h ../include/quota.h
sys2.o: ${I}/sys/proc.h ../include/inline.h ${I}/sys/fcntl.h
sys3.o: ../sys/sys3.c
sys3.o: h/param.h ${I}/sys/systm.h ${I}/sys/ino.h ../include/reg.h
sys3.o: ../include/buf.h ${I}/sys/filsys.h ../include/mount.h ${I}/sys/dir.h
sys3.o: ${I}/sys/user.h ${I}/sys/inode.h ${I}/sys/file.h ${I}/sys/conf.h
sys3.o: ${I}/sys/stat.h ../include/inline.h ${I}/sys/ustat.h
sys4.o: ../sys/sys4.c
sys4.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
sys4.o: ../include/reg.h ${I}/sys/inode.h ${I}/sys/proc.h ${I}/sys/timeb.h
sys4.o: ../include/quota.h ${I}/sys/reboot.h ${I}/sys/filsys.h
sysent.o: ../sys/sysent.c
sysent.o: h/param.h ${I}/sys/systm.h ../include/quota.h h/space.h
syslocal.o: ../sys/syslocal.c
syslocal.o: h/param.h ${I}/sys/dir.h ${I}/sys/proc.h ${I}/sys/seg.h
syslocal.o: ${I}/sys/user.h ${I}/sys/systm.h ${I}/sys/inode.h ${I}/sys/ino.h
syslocal.o: ${I}/sys/file.h ${I}/sys/conf.h ${I}/sys/tty.h
syslocal.o: ${I}/sys/autoconfig.h
text.o: ../sys/text.c
text.o: h/param.h ${I}/sys/systm.h ../include/map.h ${I}/sys/dir.h
text.o: ${I}/sys/user.h ${I}/sys/proc.h ${I}/sys/text.h ${I}/sys/inode.h
text.o: ../include/buf.h ${I}/sys/seg.h
trap.o: ../sys/trap.c
trap.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
trap.o: ${I}/sys/proc.h ../include/reg.h ../include/psw.h ${I}/sys/seg.h
trap.o: ${I}/sys/vm.h ../include/trap.h ../include/inline.h ../include/iopage.h
trap.o: ../include/koverlay.h
ureg.o: ../sys/ureg.c
ureg.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
ureg.o: ${I}/sys/proc.h ${I}/sys/text.h ${I}/sys/seg.h
vsys.o: ../sys/vsys.c
vsys.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
vsys.o: ${I}/sys/proc.h
sys5.o: ../sys/sys5.c
sys5.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/inode.h
sys5.o: ${I}/sys/user.h ../include/quota.h
ipc5.o: ../sys/ipc5.c
ipc5.o: h/param.h ${I}/errno.h ${I}/sys/types.h ${I}/signal.h ${I}/sys/proc.h
ipc5.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/ipc.h ${I}/sys/sem.h
ipc5.o: ${I}/sys/msg.h h/space.h
fcntl.o: ../sys/fcntl.c
fcntl.o: h/param.h ${I}/errno.h ${I}/sys/inode.h ${I}/sys/fcntl.h
fcntl.o: ${I}/sys/seg.h ${I}/sys/systm.h ${I}/sys/user.h ${I}/sys/file.h
fcntl.o: ${I}/sys/proc.h
msg.o: ../sys/msg.c
msg.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/types.h ${I}/signal.h
msg.o: ${I}/sys/user.h ${I}/sys/proc.h ../include/buf.h ${I}/errno.h
msg.o: ../include/map.h h/space.h ${I}/sys/ipc.h ${I}/sys/msg.h ${I}/sys/seg.h
msg.o: ../include/buf.h
mch1.o: ../sys/mch1.s
mch1.o: h/whoami.h h/ht.h h/tm.h h/ts.h ${I}/a.out.h ../include/cpu.m
mch1.o: ../include/trap.h ${I}/sys/reboot.h ../include/iopage.m
mch1.o: ../include/koverlay.h h/dz.h
boot.o: ../sys/boot.s
boot.o: h/whoami.h ../include/iopage.m
sem.o: ../sys/sem.c
sem.o: h/param.h ${I}/sys/types.h ${I}/sys/dir.h ../include/map.h ${I}/errno.h
sem.o: ${I}/signal.h h/space.h ${I}/sys/sem.h ${I}/sys/user.h ${I}/sys/proc.h
sem.o: ../include/buf.h ${I}/sys/ipc.h
param.o: ../sys/param.c
param.o: h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/tty.h
param.o: ${I}/sys/conf.h ${I}/sys/proc.h ${I}/sys/text.h ${I}/sys/dir.h
param.o: ${I}/sys/user.h ${I}/sys/file.h ${I}/sys/inode.h ${I}/sys/filsys.h
param.o: ../include/mount.h ../include/callout.h ${I}/sys/acct.h
param.o: ../include/map.h ${I}/sys/seg.h h/space.h
bio.o: ../dev/bio.c
bio.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
bio.o: ../include/buf.h ${I}/sys/conf.h ${I}/sys/proc.h ${I}/sys/seg.h
bio.o: ${I}/sys/vm.h ../include/uba.h
dh.o: ../dev/dh.c
dh.o: h/dh.h h/param.h ${I}/sys/conf.h ${I}/sys/systm.h ${I}/sys/dir.h
dh.o: ${I}/sys/user.h ${I}/sys/file.h ${I}/sys/tty.h ../include/dhreg.h
dh.o: ../include/uba.h
dj.o: ../dev/dj.c
dj.o: h/dj.h h/param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h
dj.o: ${I}/sys/tty.h ../include/djreg.h
dn.o: ../dev/dn.c
dn.o: h/dn.h h/param.h ${I}/sys/dir.h ${I}/sys/user.h ../include/dnreg.h
dz.o: ../dev/dz.c
dz.o: h/dz.h h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
dz.o: ${I}/sys/tty.h ${I}/sys/file.h ${I}/sys/conf.h ../include/dzreg.h
dz.o: ../include/pdma.h
hk.o: ../dev/hk.c
hk.o: h/hk.h h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/conf.h
hk.o: ${I}/sys/dir.h ${I}/sys/user.h ../include/uba.h ../include/inline.h
hk.o: ../include/hkreg.h ../include/dkbad.h ../include/psw.h
dkbad.o: ../dev/dkbad.c
dkbad.o: h/param.h ../include/buf.h ../include/dkbad.h
hp.o: ../dev/hp.c
hp.o: h/hp.h h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/conf.h
hp.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/seg.h ../include/hpreg.h
hp.o: ../include/inline.h ../include/uba.h
hs.o: ../dev/hs.c
hs.o: h/hs.h h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/conf.h
hs.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/seg.h ../include/hsreg.h
ht.o: ../dev/ht.c
ht.o: h/ht.h h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/conf.h
ht.o: ${I}/sys/dir.h ${I}/sys/file.h ${I}/sys/user.h ../include/htreg.h
ht.o: ${I}/sys/mtio.h
lp.o: ../dev/lp.c
lp.o: h/lp.h h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
lp.o: ${I}/sys/tty.h ${I}/sys/file.h ../include/lpreg.h
pc.o: ../dev/pc.c
pc.o: h/pc.h h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
pc.o: ${I}/sys/tty.h ${I}/sys/file.h ../include/pcreg.h
dkleave.o: ../dev/dkleave.c
dkleave.o: h/param.h ../include/buf.h
rk.o: ../dev/rk.c
rk.o: h/rk.h h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/conf.h
rk.o: ${I}/sys/dir.h ${I}/sys/user.h ../include/rkreg.h ../include/psw.h
rm.o: ../dev/rm.c
rm.o: h/rm.h h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/conf.h
rm.o: ${I}/sys/dir.h ${I}/sys/user.h ../include/hpreg.h ../include/inline.h
rm.o: ${I}/sys/seg.h ../include/uba.h
rp.o: ../dev/rp.c
rp.o: h/rp.h h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/dir.h
rp.o: ${I}/sys/conf.h ${I}/sys/user.h ../include/rpreg.h ../include/psw.h
rx.o: ../dev/rx.c
rx.o: h/rx.h h/param.h ../include/buf.h ${I}/sys/conf.h ${I}/sys/dir.h
rx.o: ${I}/sys/user.h ../include/rxreg.h ${I}/sys/seg.h
tm.o: ../dev/tm.c
tm.o: h/tm.h h/param.h ../include/buf.h ${I}/sys/dir.h ${I}/sys/conf.h
tm.o: ${I}/sys/file.h ${I}/sys/user.h ${I}/sys/systm.h ../include/tmreg.h
tm.o: ${I}/sys/mtio.h
dsort.o: ../dev/dsort.c
dsort.o: h/param.h ${I}/sys/systm.h ../include/buf.h
dvhp.o: ../dev/dvhp.c
dvhp.o: h/dvhp.h h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/conf.h
dvhp.o: ${I}/sys/dir.h ${I}/sys/user.h ../include/hpreg.h ../include/inline.h
dvhp.o: ../include/uba.h
kl.o: ../dev/kl.c
kl.o: h/kl.h h/param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h
kl.o: ${I}/sys/tty.h ${I}/sys/systm.h ${I}/sys/file.h ../include/klreg.h
kl.o: h/video.h
ts.o: ../dev/ts.c
ts.o: h/ts.h h/param.h ../include/buf.h ${I}/sys/dir.h ${I}/sys/conf.h
ts.o: ${I}/sys/file.h ${I}/sys/user.h ../include/tsreg.h ${I}/sys/mtio.h
xp.o: ../dev/xp.c
xp.o: h/xp.h h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/conf.h
xp.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/seg.h ../include/hpreg.h
xp.o: ../include/inline.h ../include/uba.h
fd.o: ../dev/fd.c
fd.o: h/fd.h h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/conf.h
fd.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/seg.h ../include/iopage.h
sys.o: ../dev/sys.c
sys.o: h/param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/tty.h
sys.o: ${I}/sys/proc.h
ildl.o: ../dev/ildl.c
ildl.o: h/video.h h/param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h
ildl.o: ${I}/sys/tty.h ${I}/sys/systm.h ../include/iopage.h ${I}/sys/seg.h
kx.o: ../dev/kx.c
kx.o: h/kx.h h/param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h
kx.o: ${I}/sys/tty.h ${I}/sys/systm.h ../include/iopage.h
ky.o: ../dev/ky.c
ky.o: h/ky.h h/param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h
ky.o: ${I}/sys/tty.h ${I}/sys/systm.h ../include/iopage.h
rw.o: ../dev/rw.c
rw.o: h/rw.h h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/conf.h
rw.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/seg.h ../include/iopage.h
wl.o: ../dev/wl.c
wl.o: h/wl.h h/param.h ${I}/sys/conf.h ${I}/sys/user.h ${I}/sys/tty.h
wl.o: ${I}/sys/systm.h
tty.o: ../dev/tty.c
tty.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/tty.h
tty.o: ${I}/sys/proc.h ${I}/sys/inode.h ${I}/sys/file.h ../include/reg.h
tty.o: ${I}/sys/conf.h ../include/buf.h h/bk.h
mpy.o: ../dev/mpy.c
mpy.o: h/mpy.h h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
mpy.o: ${I}/sys/tty.h ${I}/sys/conf.h ${I}/sys/file.h
dw.o: ../dev/dw.c
dw.o: h/dw.h h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/conf.h
dw.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/seg.h ../include/iopage.h
dw.o: ../include/dwreg.h
ks.o: ../dev/ks.c
ks.o: h/ks.h h/bk.h h/param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/file.h
ks.o: ../include/klreg.h ${I}/sys/systm.h ${I}/sys/tty.h ${I}/sys/user.h
mk.o: ../dev/mk.c
mk.o: h/mk.h h/param.h ${I}/sys/conf.h ${I}/sys/file.h ${I}/sys/dir.h
mk.o: ${I}/sys/user.h ${I}/sys/tty.h ${I}/sys/systm.h ../include/klreg.h
my.o: ../dev/my.c
my.o: h/my.h h/param.h ../include/buf.h ${I}/sys/conf.h ${I}/sys/dir.h
my.o: ../include/iopage.m ../include/myreg.h ${I}/sys/systm.h ${I}/sys/user.h
partab.o: ../dev/partab.c
partab.o: h/param.h ${I}/sys/tty.h
pty.o: ../dev/pty.c
pty.o: h/pty.h h/param.h ${I}/sys/systm.h ${I}/sys/tty.h ${I}/sys/dir.h
pty.o: ${I}/sys/user.h ${I}/sys/conf.h ../include/buf.h ${I}/sys/file.h
pty.o: ${I}/sys/proc.h
rl.o: ../dev/rl.c
rl.o: h/rl.h h/param.h ../include/buf.h ${I}/sys/dir.h ${I}/sys/user.h
rl.o: ${I}/sys/systm.h ${I}/sys/conf.h ../include/rlreg.h
dhu.o: ../dev/dhu.c
dhu.o: h/dhu.h h/param.h ${I}/sys/conf.h ${I}/sys/systm.h ${I}/sys/dir.h
dhu.o: ${I}/sys/user.h ${I}/sys/file.h ${I}/sys/tty.h ../include/dhureg.h
dhu.o: ../include/uba.h
uda.o: ../dev/uda.c
uda.o: h/ra.h h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/dir.h
uda.o: ${I}/sys/conf.h ${I}/sys/file.h ${I}/sys/user.h ../include/map.h
uda.o: ../include/inline.h ../include/ra_info.h
mem.o: ../dev/mem.c
mem.o: h/param.h ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/conf.h ${I}/sys/seg.h
graph.o: ../dev/graph.c
graph.o: h/video.h
vikey.o: ../dev/vikey.c
vikey.o: h/video.h h/param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h
vikey.o: ${I}/sys/tty.h
vd.o: ../dev/vd.c
vd.o: h/vd.h h/param.h ${I}/sys/systm.h ../include/buf.h ${I}/sys/conf.h
vd.o: ${I}/sys/dir.h ${I}/sys/user.h ${I}/sys/seg.h ../include/iopage.h
vd.o: ../include/map.h
video.o: ../dev/video.c
video.o: h/video.h h/param.h ${I}/sys/conf.h ${I}/sys/dir.h ${I}/sys/user.h
video.o: ${I}/sys/tty.h ${I}/sys/systm.h ../include/iopage.h
ttynew.o: ../dev/ttynew.c
ttynew.o: h/param.h ${I}/sys/systm.h ${I}/sys/dir.h ${I}/sys/user.h
ttynew.o: ${I}/sys/tty.h ${I}/sys/proc.h ${I}/sys/inode.h ${I}/sys/file.h
ttynew.o: ../include/reg.h ${I}/sys/conf.h ../include/buf.h

# DEPENDENCIES MUST END HERE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above

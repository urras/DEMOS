###################################################################
###################################################################
###								###
###								###
###     ****     *   *    *****    *   *    *   *               ###
###     *   *    *   *    *        *  **    *   *               ###
###     *   *    *   *    *****    * ***    *****               ###
###     ****      ****    *   *    * * *    *   *               ###
###     *            *    *   *    **  *    *   *               ###
###     *         ***     *****    *   *    *   *               ###
###								###
###           Реляционная база данных РУБИН                     ###
###								###
###                                                             ###
###								###
###################################################################
###################################################################
#
#
#
#  Основной Маке-файл для сборки СУБД Рубин
#  Кроме того, здесь содержатся модули, определяющие конфигурацию
#  самой СУБД. Утилиты, работающие автономно, а также сама команда
#  "rubin" содержатся в справочнике 'support'
#
#
#       Основные задания, которые выполняет данный Makefile:
#               allclean        чистка всех обэектных и промежуточных
#                               файлов
#               binclean        чистка всех остальных получаемых файлов
#
#               allinstall      установка СУБД в справочники самой системы
#                               RUBIN
#                       INSTALL=список действий
#               sysinstall      установка компонент rubin в системные
#                               справочники
#                       SYSBIN= bin справочник для выполняемых команд
#                       SYSLIB= справочник для библиотек (/usr/lib).
#       Для всего этого определено:
#               CPUTYPE=тип процессора. Он устанавливается также в файле
#                       ../h/local.h
#
#       Примеры использования:
#               Перетрансляция всей системы:
#			make allclean allinstall
#               Установка rubin для всех пользователей:
#			make allinstall sysinstall
#               Установка на MC68000 основного и отладочного выриантов
#               системы:
#                       make insall
#
#  Информация о системе:

# Корневой справочник RUBIN
RUBIN=	../..
# размещение файлов вставок
H=	../h
# размещение системных библиотек
LIB=	$(RUBIN)/lib
# размещение исполняемых компонент (должно соответствовать описанию
# в таблице процессов)
BIN=	$(RUBIN)/bin
CCM = /bin/cc286 -M2m
CC    = cc
CPU=pdp11
CFLAGS= -I$H -O
LDFLAGS=-i
RANLIB="ranlib"

dfl:
	@echo "make {apdp ademos avax amc68 ax286 ax386}"

#
# Set up for Xenix-286:
#
CC2:    $(RUBIN)/bin/cc

$(RUBIN)/bin/cc:   cc.286
	rm -f cc-STD
	make allclean
	cp cc.286 $(RUBIN)/bin/cc

#
# Set up for other systems
#
CC3:    cc-STD

cc-STD:
	make allclean
	rm -f $(RUBIN)/bin/cc
	cp /dev/null cc-STD

all:    $(CPU)
allall: a$(CPU)
install: i$(CPU)

pdp= monitor xparser qrymod decomp dbu01 dbu02
vax= monitor vaxrubin
mc68=monitor vaxrubin
full=$(pdp) vaxrubin
x386=monitor vaxrubin
x286=monitor xparser qrymod xdecomp xdbu01 dbu02
ipdp = $(BIN)/monitor $(BIN)/xparser $(BIN)/qrymod $(BIN)/decomp $(BIN)/dbu01 \
       $(BIN)/dbu02
ivax=$(BIN)/monitor $(BIN)/vaxrubin
ix386=$(ivax)
imc68 = $(ivax)
ix286 = $(BIN)/monitor $(BIN)/xparser $(BIN)/qrymod $(BIN)/decomp $(BIN)/xdbu01 \
	$(BIN)/dbu02
ifull=$(ipdp) $(BIN)/vaxrubin

pdp: CC3 pttype $(pdp)
vax: CC3 pttype $(vax)
mc68:CC3 pttype $(mc68)
full:pttype $(full)
x286:CC2 pttype $(x286)
x386:CC3 pttype $(x386)

ipdp:  CC3  $(ipdp)
idemos:CC3  $(ivax)
ivax:  CC3  $(ivax)
imc68: CC3  $(imc68)
ifull: CC3  $(ifull)
ix286: CC2 $(ix286)
isql:  $(BIN)/sqlmon
ix386: CC3  $(ix386)

apdp:   CC3 alllib isupport $(ipdp)
ademos: CC3 alllib isupport ildr $(ivax)
avax:   CC3 alllib isupport $(ivax)
amc68:  CC3 alllib isupport $(imc68)
afull:  CC3 alllib isupport $(ifull)
ax286:  CC2 malllib isupport $(ix286)
ax386:  CC3 alllib isupport $(ix386)

ildr:
	cd ../support;make instlock

SRCS=   monitor.c xparser.c parser.c qrymod.c xdecomp.c decomp.c alldbu.c vaxrubin.c \
	dbu1.c sqlmon.c dbu2.c dbu01.c dbu02.c sysdump.c version.c

#
#  Библиотеки
#
#       Порядок их следования не менять
#
SQL   = $(LIB)/sql
MCTLMOD= $(LIB)/Mctlmod
MIUTIL=  $(LIB)/Miutil
MGUTIL=  $(LIB)/Mgutil
CTLMOD=	$(LIB)/ctlmod
IUTIL=	$(LIB)/iutil
GUTIL=	$(LIB)/gutil
MATH=	-lm

ALLLIB= $(CTLMOD) $(IUTIL) $(GUTIL) $(CTLMOD)
MALLLIB= $(MCTLMOD) $(MIUTIL) $(MGUTIL) $(MCTLMOD)

#
# Печать типа ЭВМ
#

alllib malllib pdp vax mc68 x286: pttype

pttype: typeMASH
	typeMASH
typeMASH: typeMASH.c /lib/cpp
	cc -o typeMASH typeMASH.c

#
#  Об`ектные файлы
#
MONITOR=$(LIB)/monitor
PARSER=	$(LIB)/parser
QRYMOD=	$(LIB)/qrymod
DECOMP=	$(LIB)/decomp
OVQP=	$(LIB)/ovqp
DBULIB=	$(LIB)/dbulib
MDECOMP= $(LIB)/Mdecomp
MOVQP=   $(LIB)/Movqp

#
#  Всякие команды
#
CHMOD=	chmod
BINMODE=751
SUMODE=	4740
CP=     ln
MAKE=   make -$(MAKEFLAGS) CPU=$(CPU) CP=$(CP) LDFLAGS=$(LDFLAGS) RANLIB=$(RANLIB)
INSTALL=install

#
#       Полная перетрансляция
#

#
#  And the processes....
#
#	Proper loading order is:
#		conf table
#		library(s) for modules
#		ctlmod library
#		iutil library
#		gutil library
#

#  Sysdump

sysdump: version.o sysdump.o $(CTLMOD) $(ALLLIB)
	$(CC) $(LDFLAGS) version.o $@.o $(OTHERS) $(ALLLIB) -o $@
	$(CHMOD) $(SUMODE) $@
	ls -l $@; size $@

#  Monitor

sqlmon: version.o sqlmon.o $(MONITOR) $(SQL) $(CTLMOD) $(IUTIL) $(GUTIL)
	$(CC) $(LDFLAGS) version.o $@.o $(OTHERS) $(MONITOR) $(SQL) $(CTLMOD) $(IUTIL) $(GUTIL) -ll -o $@
	$(CHMOD) $(BINMODE) $@
	ls -l $@; size $@

#  Monitor

monitor: version.o monitor.o $(MONITOR) $(CTLMOD) $(IUTIL) $(GUTIL)
	$(CC) $(LDFLAGS) version.o $@.o $(OTHERS) $(MONITOR) $(CTLMOD) $(IUTIL) $(GUTIL) -o $@
	$(CHMOD) $(BINMODE) $@
	ls -l $@; size $@

#  Parser

parser: version.o parser.o $(PARSER) $(DBULIB) $(ALLLIB)
	$(CC) $(LDFLAGS) version.o $@.o $(OTHERS) $(PARSER) $(ALLLIB) $(MATH) -o $@
	$(CHMOD) $(SUMODE) $@
	ls -l $@; size $@

#  x-Parser

xparser: version.o xparser.o $(PARSER) $(DBULIB) $(ALLLIB)
	$(CC) $(LDFLAGS) version.o $@.o $(OTHERS) $(PARSER) $(DBULIB) $(ALLLIB) $(MATH) -o $@
	$(CHMOD) $(SUMODE) $@
	ls -l $@; size $@

#  Query Modification

qrymod: version.o qrymod.o $(QRYMOD) $(ALLLIB)
	$(CC) $(LDFLAGS) version.o $@.o $(OTHERS) $(QRYMOD) $(ALLLIB) $(MATH) -o $@
	$(CHMOD) $(SUMODE) $@
	ls -l $@; size $@


#
# Xenix 286 modules for MIDDLE MEMORY MODEL
#
xdecomp.o:      xdecomp.c
	$(CCM) -c $(CFLAGS) xdecomp.c

xversion.o:     version.c
	rm -f xversion.c
	ln version.c xversion.c
	$(CCM) -c $(CFLAGS) xversion.c
	rm -f xversion.c

#  Decomp/OVQP           -F 3500        $(GUTIL)

xdecomp: xversion.o xdecomp.o $(MDECOMP) $(MOVQP) $(MALLLIB)
	$(CCM)  -F 3500   xversion.o xdecomp.o $(OTHERS) $(MDECOMP) $(MIUTIL) $(MOVQP) $(MCTLMOD) $(MIUTIL) $(MGUTIL) $(MATH) -o $@
	$(CHMOD) $(SUMODE) $@
	ls -l $@; size $@
decomp: version.o decomp.o $(DECOMP) $(OVQP) $(ALLLIB)
	$(CC) $(LDFLAGS) version.o $@.o $(OTHERS) $(DECOMP) $(OVQP) $(ALLLIB) $(MATH) -o $@
	$(CHMOD) $(SUMODE) $@
	ls -l $@; size $@

#  All DBU routines.

alldbu: version.o alldbu.o $(DBULIB) $(ALLLIB)
	$(CC) $(LDFLAGS) version.o $@.o $(OTHERS) $(DBULIB) $(ALLLIB) $(MATH) -o $@
	$(CHMOD) $(SUMODE) $@
	ls -l $@; size $@

Tdbu01: version.o dbu01.o $(DBULIB) $(ALLLIB)
	$(CC) $(LDFLAGS) version.o dbu01.o $(OTHERS) $(DBULIB) $(ALLLIB) $(MATH) -o $@
	$(CHMOD) $(SUMODE) $@
	ls -l $@; size $@

xdbu01: version.o dbu01.o $(DBULIB) $(ALLLIB)
	$(CC) $(LDFLAGS) -F 3000 version.o $@.o $(OTHERS) $(DBULIB) $(ALLLIB) $(MATH) -o $@
	$(CHMOD) $(SUMODE) $@
	ls -l $@; size $@

dbu01: version.o dbu01.o $(DBULIB) $(ALLLIB)
	$(CC) $(LDFLAGS) version.o $@.o $(OTHERS) $(DBULIB) $(ALLLIB) $(MATH) -o $@
	$(CHMOD) $(SUMODE) $@
	ls -l $@; size $@

dbu02: version.o dbu02.o $(DBULIB) $(ALLLIB)
	$(CC) $(LDFLAGS) version.o $@.o $(OTHERS) $(DBULIB) $(ALLLIB) $(MATH) -o $@
	$(CHMOD) $(SUMODE) $@
	ls -l $@; size $@

dbu2: version.o dbu2.o $(DBULIB) $(ALLLIB)
	$(CC) $(LDFLAGS) version.o $@.o $(OTHERS) $(DBULIB) $(ALLLIB) $(MATH) -o $@
	$(CHMOD) $(SUMODE) $@
	ls -l $@; size $@

dbu1: version.o dbu1.o $(DBULIB) $(ALLLIB)
	$(CC) $(LDFLAGS) version.o $@.o $(OTHERS) $(DBULIB) $(ALLLIB) $(MATH) -o $@
	$(CHMOD) $(SUMODE) $@
	ls -l $@; size $@

#  Full BIG System

ALLLIBS=$(PARSER) $(QRYMOD) $(DECOMP) $(OVQP) $(DBULIB) $(ALLLIB)

vaxrubin: version.o vaxrubin.o $(ALLLIBS)
	$(CC) -s $(LDFLAGS) version.o $@.o $(OTHERS) $(ALLLIBS) $(MATH) -o $@
	$(CHMOD) $(SUMODE) $@
	ls -l $@; size $@

#################################################################
#
#	Install the world
#

$(BIN)/monitor: monitor
	rm -f $@
	$(CP) monitor $(BIN)/monitor

$(BIN)/sqlmon: sqlmon
	rm -f $@
	$(CP) sqlmon $(BIN)/sqlmon

$(BIN)/parser: parser
	rm -f $@
	$(CP) parser $(BIN)/parser; $(CHMOD) $(SUMODE) $(BIN)/parser
$(BIN)/xparser: xparser
	rm -f $@
	$(CP) xparser $(BIN)/xparser; $(CHMOD) $(SUMODE) $(BIN)/xparser
$(BIN)/qrymod: qrymod
	rm -f $@
	$(CP) qrymod $(BIN)/qrymod; $(CHMOD) $(SUMODE) $(BIN)/qrymod
$(BIN)/xdecomp: xdecomp
	rm -f $@
	$(CP) xdecomp $(BIN)/xdecomp; $(CHMOD) $(SUMODE) $(BIN)/xdecomp
	rm -f $(BIN)/decomp;ln $(BIN)/xdecomp $(BIN)/decomp
$(BIN)/decomp: decomp
	rm -f $@
	$(CP) decomp $(BIN)/decomp; $(CHMOD) $(SUMODE) $(BIN)/decomp
$(BIN)/alldbu: alldbu
	rm -f $@
	$(CP) alldbu $(BIN)/alldbu; $(CHMOD) $(SUMODE) $(BIN)/alldbu
$(BIN)/xdbu01: xdbu01
	rm -f $@
	$(CP) xdbu01 $(BIN)/xdbu01; $(CHMOD) $(SUMODE) $(BIN)/xdbu01
$(BIN)/dbu01: dbu01
	rm -f $@
	$(CP) dbu01 $(BIN)/dbu01; $(CHMOD) $(SUMODE) $(BIN)/dbu01
$(BIN)/dbu02: dbu02
	rm -f $@
	$(CP) dbu02 $(BIN)/dbu02; $(CHMOD) $(SUMODE) $(BIN)/dbu02
$(BIN)/dbu1: dbu1
	rm -f $@
	$(CP) dbu1 $(BIN)/dbu1; $(CHMOD) $(SUMODE) $(BIN)/dbu1
$(BIN)/dbu2: dbu2
	rm -f $@
	$(CP) dbu2 $(BIN)/dbu2; $(CHMOD) $(SUMODE) $(BIN)/dbu2
$(BIN)/vaxrubin: vaxrubin
	rm -f $@
	$(CP) vaxrubin $(BIN)/vaxrubin; $(CHMOD) $(SUMODE) $(BIN)/vaxrubin
$(BIN)/sysdump: sysdump
	rm -f $@
	$(CP) sysdump $(BIN)/sysdump; $(CHMOD) $(SUMODE) $(BIN)/sysdump


alllib:
	cd ../h; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../gutil; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../iutil; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../ctlmod; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../dbu; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../decomp; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../equel; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../libq; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../monitor; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../ovqp; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../parser; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../qrymod; $(MAKE) $(MFLAGS) $(INSTALL)
#       cd ../support; $(MAKE) $(MFLAGS)

malllib:
	cd ../h; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../gutil; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../Mgutil; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../iutil; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../Miutil; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../ctlmod; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../Mctlmod; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../dbu; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../Mdecomp; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../equel; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../libq; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../Mlibq; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../Llibq; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../monitor; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../Movqp; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../parser; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../qrymod; $(MAKE) $(MFLAGS) $(INSTALL)
# install the user programs
isupport:
	cd ../support; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../equel; $(MAKE) $(MFLAGS) $(INSTALL)
	cd ../libq; $(MAKE) $(MFLAGS) $(INSTALL)

sysinstall:
	@echo 'you may have to be root for this to work....'
	cd ../support; $(MAKE) $(MFLAGS) SYSBIN=$(SYSBIN) SYSLIB=$(SYSLIB) sysinstall
	cd ../equel; $(MAKE) $(MFLAGS) SYSBIN=$(SYSBIN) SYSLIB=$(SYSLIB) sysinstall
	cd ../libq; $(MAKE) $(MFLAGS) SYSBIN=$(SYSBIN) SYSLIB=$(SYSLIB) sysinstall

#################################################################
#
#	Miscellaneous entries
#

new newPDP:
	rm -f $(full)
newMC68000 newVAX:
	rm -f $(vax)

print: sources
	$(PR) Makefile *.[csy]

vgrind: sources
	cp /dev/null index
	$(VGRIND) -h "$(HEADER) (Release 7)" -n Makefile
	$(VGRIND) -h "$(HEADER) (Release 7)" *.[csy]
	$(VGRIND) -h "$(HEADER) (Release 7)" -x index

clean: new
	rm -f *.o
	rm -f a.out core
	rm -f typeMASH  monitor parser xparser xdecomp decomp qrymod dbu02 dbu02

allclean: clean
	cd ../h; $(MAKE) $(MFLAGS) clean
	cd ../gutil; $(MAKE) $(MFLAGS) clean
	-cd ../Mgutil; $(MAKE) $(MFLAGS) clean
	cd ../iutil; $(MAKE) $(MFLAGS) clean
	-cd ../Miutil; $(MAKE) $(MFLAGS) clean
	cd ../ctlmod; $(MAKE) $(MFLAGS) clean
	-cd ../Mctlmod; $(MAKE) $(MFLAGS) clean
	cd ../dbu; $(MAKE) $(MFLAGS) clean
	cd ../decomp; $(MAKE) $(MFLAGS) clean
	-cd ../Mdecomp; $(MAKE) $(MFLAGS) clean
	cd ../equel; $(MAKE) $(MFLAGS) clean
	cd ../libq; $(MAKE) $(MFLAGS) clean
	-cd ../Mlibq; $(MAKE) $(MFLAGS) clean
	-cd ../Llibq; $(MAKE) $(MFLAGS) clean
	cd ../monitor; $(MAKE) $(MFLAGS) clean
	cd ../ovqp; $(MAKE) $(MFLAGS) clean
	-cd ../Movqp; $(MAKE) $(MFLAGS) clean
	cd ../parser; $(MAKE) $(MFLAGS) clean
	cd ../qrymod; $(MAKE) $(MFLAGS) clean
	cd ../support; $(MAKE) $(MFLAGS) clean

binclean:       allclean
	cd ../..;rm -f lib/* bin/*

sources: $(SRCS)

$(SRCS):
	$(GET) $(REL) SCCS/s.$@

depend:
	grep '^#[ 	]*include' $(SRCS) | sed -n -f ../h/depend.sed > makedep
	echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile Makefile.bak
	ed - Makefile < eddep
	rm eddep makedep
	echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	echo '# see make depend above' >> Makefile

# DO NOT DELETE THIS LINE -- make depend uses it

monitor.o:$H/func.h
parser.o:$H/defines.h
parser.o:$H/aux.h
parser.o:$H/access.h
parser.o:$H/func.h
qrymod.o:$H/func.h
decomp.o:$H/func.h
decompx.o:$H/func.h
alldbu.o:$H/func.h
vaxrubin.o:$H/defines.h
vaxrubin.o:$H/aux.h
vaxrubin.o:$H/access.h
vaxrubin.o:$H/func.h
sysdump.o:$H/func.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above

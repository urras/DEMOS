#>#######################################################################
#>#                                                                     #
#>#                                                                     #
#>#                             *       *                               #
#>#                             * ****                                  #
#>#                          **** *   * *                               #
#>#                         *   * ****  *                               #
#>#                         *   * *     *                               #
#>#                          **** *      *                              #
#>#                                                                     #
#>#          БИБЛИОТЕКА ФУНКЦИЙ ДЛЯ РАБОТЫ В ЭКРАННОМ РЕЖИМЕ            #
#>#                                                                     #
#>#######################################################################

SHELL=/bin/sh

#SYSTEM  = BSD
#SYSTEM  = VENIX
#SYSTEM  = XENIX
SYSTEM  = SYSV

CC      = cc
P       = ..
H       = \"/usr/local/lib/minihelp\"
CFLAGS  = -O -D$(SYSTEM) -DHELPDIR=$H -I/usr/include/sys -I.
N       = dpi
E       = $P/distribute/$N


##### Функции библиотеки

LS      = help.c  hdps.c  dpr.c  dpt.c  dpn.c  dps.c  dpi.c  empty.c
LS_TERMCAP = tcinit.c tparm.c

.c.o:             # трансляция с оптимизацией и убиранием локальных имен
	 $(CC) $(CFLAGS) -c $*.c
#         $O               < $*.s > .$*.s
#         as -o    $*.o -           .$*.s
#         rm -f              $*.s   .$*.s
#       -ld -r -i $*.o
#        mv a.out $*.o

####

lib$N.a:  lib_$(SYSTEM)

lib_BSD:   ;  make -k LS="$(LS) $(LS_BSD) $(LS_TERMCAP)"  O=${OPT} .libdpi
lib_VENIX: ;  make -k LS="$(LS) $(LS_V7) $(LS_TERMCAP)"   O=cat    .libdpi
lib_XENIX: ;  make -k LS="$(LS) $(LS_V7) $(LS_TERMCAP)"   O=cat    .libdpi
lib_SYSV:  ;  make -k LS="$(LS) $(LS_V7) $(LS_TERMCAP)"   O=cat    .libdpi



##### Генерация библиотеки #####

.libdpi: $(LS)
	@echo $? | sed s/\\.cm/.o/g | sed s/\\.\[cs\]/.o/g > .list
	@echo make  -k O=$O `cat .list`
	@     make  -k O=$O `cat .list`
	@echo ar cr lib$N.a `cat .list`
	@     ar cr lib$N.a `cat .list`
	@echo rm -f         `cat .list` .list
	-@    rm -f         `cat .list` .list
	 touch $@

tty_descr.h :: tty_codes.h  ;  touch $@
dpi.c       :: tty_descr.h  ;  touch $@
$(LS)       :: tty_codes.h  ;  touch $@


##### Включение библиотеки в систему #####

distr = lib$N.a tty_codes.h tty_descr.h ttys/[a-z]*

install: /lib/Slib$N.a  lib$N.a
	 cp lib$N.a /lib/Slib$N.a

$E:      $(distr) Makefile
	-mkdir $E
	-mkdir $E/ttys
	 for i in $? ; do  rm -f $E/$$i ; ln $$i $E/$$i ; done
	 rm -f $E/Makefile
	 grep '^#>' Makefile | colrm 1 2 | unexpand > $E/Makefile


######
clear:
	-rm -f *.[obO] ttys/*.[obO] .[a-z]* lib$N.a gendescr tty_codes.h



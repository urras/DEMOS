#          #####                                              ###### ######
#         #  #  #                                                  #      #
#         #  #  #  ###  ####  ##### ####   ###   #   #            #      #
#         #  #  # #   # #   #   #   #   #     #  #   # ######    #      #
#          #####  #   # ####    #   ####   ####  #####          #      #
#            #    #   # #       #   #     #   #  #   #         #      #
#            #     ###  #       #   #      ### # #   #         #      #
#
#       на ЭВМ СМ-4, pdp 11/40 и т.п.
#       f77p1 - для машин с FPP, f77p1f - для машин без FPP.
#       Makefile для машин с раздельным I/D называется Makefile.ID
#
F77LIB=/usr/lib/f77
INSTALL=f77 f77pass0 f77p1f Messages

all:    $(INSTALL)

#
# Установка Фортрана-77
# Для СМ-4.
#
install: $(INSTALL)
	-strip $(INSTALL)
	cd  $(F77LIB); rm -f $(INSTALL)
	cp  $(INSTALL) $(F77LIB)
	cd  $(F77LIB); rm -f f77p1; ln f77p1f f77p1
	rm -f /usr/bin/f77
	ln -s $(F77LIB)/f77 /usr/bin/f77

#
# Флаги команды f77 (DEBF77 - брать все из текущего каталога)
#     -DDEBF77
CF77F=
#
CO=co
XSTR = tools/xstr
TOOLS = tools/
CC = cc
YACC=yacc

CFL = -DTARGET=PDP11 -DFAMILY=DMR -DHERE=PDP11 -DOUTPUT=BINARY -DPOLISH=POSTFIX
OBJECTS0 = main0.o init0.o gram0.o lex.o error0.o put0.o misc0.o error0.o
CFLAGS  = -O -DP0F ${CFL}
CFLAGS1 = -O -DP1F ${CFL}  -V
CCX = ccxstr
#
# Сегментация первого прохода
#


BAZE1 =  misc.o error.o malloc.o gram1.o
OVER1 =  init.o initp.o put.o putdmr.o putcdmr.o\
	 pdp11m.o pdp11p.o pdp11x.o main.o
OVER2 =  proc1.o proc2.o gets1.o procm.o equiv.o data.o
OVER3 =  expr.o exec.o intr1.o intr2.o
OVER4 =  io.o execdo.o exec.o execctl.o
OVER5 =
OBJECTS1 = $(BAZE1) $(OVER1) $(OVER2) $(OVER3) $(OVER4) $(OVER5)

f77 : driver.o pdp11x0.o
	cc -n  driver.o pdp11x0.o -o f77
	@size f77

driver.o : drivedefs
driver.o : driver.c Makefile
	  ${CC} $(CF77F) ${CFL} -DPASS0 -c driver.c

pdp11x0.o : pdp11x.c
	  rm -f pdp11x0.c
	  ln pdp11x.c pdp11x0.c
	  ${CC} ${CFL} -c pdp11x0.c
	  rm -f pdp11x0.c

f77pass0 : ${OBJECTS0}
	cc -n -f ${OBJECTS0} -o f77pass0

${OBJECTS0} : defs defines locdefs ftypes p0defs.h

lex.o : tokdefs lex.c
	  ${CC} ${CFLAGS} -c lex.c

init0.o : init.c
	  rm -f init0.c
	  ln init.c init0.c
	  ${CC} ${CFLAGS} -c init0.c
	  rm -f init0.c

main0.o : main.c
	  rm -f main0.c
	  ln main.c main0.c
	  ${CC} ${CFLAGS} -c main0.c
	  rm -f main0.c

put0.o: put0.c


error0.o : error.c
	  rm -f error0.c
	  ln error.c error0.c
	  ${CC} ${CFLAGS} -c error0.c
	  rm -f error0.c

misc0.o : misc0.c
	  ${CC} ${CFLAGS} -c misc0.c

gram1.h gram0.c : gram.c
	${TOOLS}yparopt -O gram.c gram1.h >gram0.c

gram.c: gram.head gram.dcl gram.expr gram.exec gram.io tokdefs  ${TOOLS}newparser
	( rpl <tokdefs "#define" "%token" ;\
		cat gram.head gram.dcl gram.expr gram.exec gram.io ) >gram.in
	$(YACC)  $(YFLAGS) -f ${TOOLS}newparser gram.in
	mv y.tab.c gram.c

gramh1.h : gram.head     yactoh.ed
	ed gram.head < yactoh.ed

${OBJECTS1} driver.o : defs defines locdefs ftypes ctype.h libio/stdio.h
tokdefs: tokens
	grep -n . <tokens | sed "s/\([^:]*\):\(.*\)/#define \2 \1/" >tokdefs

gram.o :
	cc -c ${CFL} gram.c
driver.o $(OBJECTS)  : defs defines locdefs ftypes

locdefs : pdp11defs
	cp pdp11defs locdefs

put.o putdmr.o pdp11.o : dmrdefs
io.o : fio.h


main.o : main.c
	${CCX}  main ${CFLAGS1}
execctl.o : execctl.c
	${CCX}  execctl ${CFLAGS1}
malloc.o : malloc.c
	${CCX}  malloc ${CFLAGS1}
init.o : init.c
	${CCX}  init ${CFLAGS1}
initp.o : initp.c
	${CCX}  initp ${CFLAGS1}
procm.o : procm.c
	${CCX}  procm ${CFLAGS1}
proc1.o : proc1.c
	${CCX}  proc1 ${CFLAGS1}
proc2.o : proc2.c
	${CCX}  proc2 ${CFLAGS1}
equiv.o : equiv.c
	${CCX}  equiv ${CFLAGS1}
data.o : data.c
	${CCX}  data ${CFLAGS1}
expr.o : expr.c
	${CCX}  expr ${CFLAGS1}
intr2.o : intr2.c
	${CCX}  intr2 ${CFLAGS1}
exec.o : exec.c
	${CCX}  exec ${CFLAGS1}
execdo.o : execdo.c
	${CCX}  execdo ${CFLAGS1}
gram1.o : gram1.h gramh1.h parsr1.c
	${CCX}  parsr1 -Btools/c -t2 ${CFLAGS1}
	mv parsr1.o gram1.o
intr1.o : intr1.c
	${CCX}  intr1 ${CFLAGS1}
io.o : io.c
	${CCX}  io ${CFLAGS1}
misc.o : misc.c
	${CCX}  misc ${CFLAGS1}
error.o : error.c
	${CCX}  error ${CFLAGS1}
put.o putdmr.o putcdmr.o pdp11m.o pdp11p.o: dmrdefs
put.o : put.c
	${CCX}  put ${CFLAGS1}
putcdmr.o : putcdmr.c
	${CCX}  putcdmr ${CFLAGS1}
putdmr.o : putdmr.c
	${CCX}  putdmr ${CFLAGS1}
pdp11m.o : pdp11m.c
	${CCX}  pdp11m ${CFLAGS1}
pdp11p.o : pdp11p.c cdebdefs
	${CCX}  pdp11p ${CFLAGS1}
pdp11x.o : pdp11x.c
	${CC} ${CFL} -O  -c pdp11x.c
gets1.o : gets1.c
	${CCX}  gets1 ${CFLAGS1}
Xsline.o:  Xsline.c
	   cc -c $(CFLAGS1) Xsline.c

f77p1:  $(BAZE1) \
	$(OVER1) $(OVER2) $(OVER3) $(OVER4) $(OVER5) \
	Xsline.o libio/libio.a Makefile
	   cc -n -V -o f77p1 $(BAZE1) \
	-Z $(OVER1) -Z $(OVER2) -Z $(OVER3) -Z $(OVER4) -Z $(OVER5) \
	-L libio/libio.a Xsline.o
f77p1f:  $(BAZE1) \
	$(OVER1) $(OVER2) $(OVER3) $(OVER4) $(OVER5) \
	Xsline.o libio/libio.a Makefile
	   cc -n -f -V -o f77p1f $(BAZE1) \
	-Z $(OVER1) -Z $(OVER2) -Z $(OVER3) -Z $(OVER4) -Z $(OVER5) \
	-L libio/libio.a Xsline.o
#
# Пока все
#
lint:
	@echo "LINT DRIVER" >Lintout
	@lint -p $(CFL) driver.c pdp11x.c >>Lintout
	echo "LINT PASS1" >>Lintout
	@nohup lint -p $(CFL) error.c exec.c intr.c expr.c gram.c init.c io.c\
		lex.c main.c proc.c misc.c put.c putdmr.c pdp11.c pdp11x.c\
		equiv.c data.c >>Lintout&

cleanup clean:
	-rm -f gram.c gram.in *.o f77 f77p1 f77p1f gram0.c gram1h.h gram1.h
	du
/usr/bin/f77 : f77
	strip f77
	@size f77 /usr/bin/f77
	cp f77 /usr/bin/f77

/usr/lib/f77p1 : f77pass1
	strip f77pass1
	@size f77pass1 /usr/lib/f77pass1
	-chmod u-t /usr/lib/f77pass1
	@/usr/lib/f77pass1 2>/dev/null
	cp f77pass1 /usr/lib/f77pass1
	echo "***Now do a    chmod u+t /usr/lib/f77pass1    in su mode"

# putscj.o :putscj.c scjdefs;cc ${CFLAGS} -DFAMILY=SCJ putscj.c -c

.DEFAULT: RCS/$@,v
	$(CO) $(GETF) $@

#####################################################
#   Создание справочника по компонентам ОС ДЕМОС.   #
#####################################################
PIF=    pif
PIFMAC= -mcps cps.mac
FILES=  title.cps intro.cps quick.cps
OBJS=   title.lp intro.lp quick.lp
DESDIR= /mnt/man/tools/
.SUFFIXES: .lp .cps

.cps.lp:
        ${PIF} ${PIFMAC} $< >$@

quick: ${OBJS}
        -cat ${OBJS} | ${DESDIR}fmt >$@

${OBJS}: cps.mac

quick.cps: quickreff
        sort +1 -3 -o $@ quickreff

vocab: quickreff
        tr -cs Ю-Чю-ч\' '\012' <$< | sort | uniq -c >$@

#quickreff:
#        ${DESDIR}qref </usr/lib/whatis >quickreff

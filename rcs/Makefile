# Общий Makefile для RCS

SUBDIR        = src/rcs src/rdiff src/rdiff3
DESTDIR       = /usr/bin

all:            ${SUBDIR}
####

${SUBDIR}:      FRC
		cd $@; make ${MFLAGS}

install:
########
	for i in ${SUBDIR}; do \
		(cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); done

clean clear:
###########
	for i in ${SUBDIR}; do (echo $$i; cd $$i; make ${MFLAGS} clean); done

CLRSRC:
	for i in ${SUBDIR}; do (echo $$i; cd $$i; make ${MFLAGS} CLRSRC); done

depend:
	for i in ${SUBDIR}; do (echo $$i; cd $$i; make ${MFLAGS} depend); done

FRC:

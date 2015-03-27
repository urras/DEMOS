#
# Библиотека plot
#
#       Makefile
#
ALL=    libplot libt4014 libt300 libt300s libt450 libvt0 libe85 libepson libs5400 libem7052
SUBDIRS=plot t4014 t300 t300s t450 vt0 e85 epson s5400 em7052
DESTDIR=/usr/lib

all:    ${ALL}

install: i_s5400 i_plot i_t4014 i_t300 i_t300s i_t450 i_vt0 i_e85 i_epson i_em7052

libplot: plot
	cd plot; make

libe85: e85
	cd e85; make

libs5400: s5400
	cd s5400; make

libt4014: t4014
	cd t4014; make

libt300: t300
	cd t300; make

libt300s: t300s
	cd t300s; make

libt450: t450
	cd t450; make

libvt0: vt0
	cd vt0; make

libepson: epson
	cd epson; make

libem7052: em7052
	cd em7052; make

i_plot: libplot
	cp libplot ${DESTDIR}/libplot.a
	ranlib ${DESTDIR}/libplot.a

i_t4014: libt4014
	cp libt4014 ${DESTDIR}/libt4014.a
	ranlib ${DESTDIR}/libt4014.a

i_t300: libt300
	cp libt300 ${DESTDIR}/libt300.a
	ranlib ${DESTDIR}/libt300.a

i_t300s: libt300s
	cp libt300s ${DESTDIR}/libt300s.a
	ranlib ${DESTDIR}/libt300s.a

i_t450: libt450
	cp libt450 ${DESTDIR}/libt450.a
	ranlib ${DESTDIR}/libt450.a

i_vt0: libvt0
	cp libvt0 ${DESTDIR}/libvt0.a
	ranlib ${DESTDIR}/libvt0.a

i_e85: libe85
	cp libe85 ${DESTDIR}/libe85.a
	ranlib ${DESTDIR}/libe85.a

i_s5400: libs5400
	cp libs5400 ${DESTDIR}/libs5400.a
	ranlib ${DESTDIR}/libs5400.a

i_epson: libepson
	cp libepson ${DESTDIR}/libepson.a
	ranlib ${DESTDIR}/libepson.a

i_em7052: libem7052
	cp libem7052 ${DESTDIR}/libem7052.a
	ranlib ${DESTDIR}/libem7052.a

clean clear:
	rm -f ${ALL} errs a.out core
	for i in ${SUBDIRS}; do \
		(cd $$i; make clean); \
	done

DESTDIR=/
DIV=nonfpldiv
LIB=libbasec.a
D=

all: lib crtx

lib:
	cd overlay; make clean
	@rm -f G_crt G_gen G_sys
	make ORDER "DIV=$(DIV)" "D="
	make splitlib "LIB=libbasec.a"

base:
	cd overlay; make clean
	@rm -f G_crt G_gen G_sys
	make ORDER "DIV=$(DIV)" "D="
	make SPLIT
	make libbasec.a "LIB=libbasec.a" "ORD=BASELIB"

ovbase:
	@rm -f G_crt G_gen G_sys
	make ORDER "DIV=$(DIV)" "D=overlay"
	make SPLIT
	make libovbasec.a "LIB=libovbasec.a" "ORD=BASELIB"

ovlib:
	@rm -f G_crt G_gen G_sys
	make ORDER "DIV=$(DIV)" "D=overlay"
	make splitlib "LIB=libovbasec.a"

fplib:
	make lib "DIV=ldiv"

fpovlib:
	make ovlib "DIV=ldiv"

SPLIT OVLIB BASELIB:
	echo '1,/zzzz/-1w TMPLIB%/zzzz/+1,$$w BASELIB%q' | \
		tr '%' '\012' | ed - ORDER
	echo 'stdio/flsbuf.o%gen/malloc.o%gen/ecvt.o%stdio/gcvt.o%stdio/strout.o' | \
		tr '%' '\012' >> TMPLIB
	lorder `sort -t/ +1 -2 TMPLIB` | tsort > OVLIB

splitlib: ORDER SPLIT
	make $(LIB) "LIB=$(LIB)" "ORD=BASELIB"
	make libsplitc.a "LIB=libsplitc.a" "ORD=OVLIB"

install: inst instcrt

inst: splitinst
	make instlib "LIB=libbasec.a" "ORD=BASELIB"
	cp fake/libc.a $(DESTDIR)/lib

ovinst: splitinst
	make instlib "LIB=libovbasec.a" "ORD=BASELIB"
	cp fake/libovc.a $(DESTDIR)/lib

splitinst:
	make instlib "LIB=libsplitc.a"  "ORD=OVLIB"

libcom:: sys gen stdio $(DIV)
	@echo ">>>>> Generate $(D) library modules"

libcom:: G_crt G_ldiv G_sys G_gen G_stdio
	@echo "">libcom

G_sys:  sys
	@echo '##### Syscalls emulators'
	cd sys ; make $(D)
	@echo "">G_sys

G_gen:  gen
	@echo '##### General purpose routines'
	cd gen ; make $(D)
	@echo "">G_gen

G_stdio:   stdio
	@echo '##### Standard I/O library'
	cd stdio  ; make
	@echo "">G_stdio

G_ldiv:  $(DIV)
	@echo "##### $(DIV) routines"
	cd $(DIV) ; make
	@echo "">G_ldiv

G_crt:  crt
	@echo "##### Standard $(D) routines"
	cd crt ; make $(D)
	@echo "">G_crt

t:      libcom
	@echo ">>>>> Build up $(D) library ordering"
	ls -1 $(DIV)/*.o > t
	ls -1 crt/*.o >> t
	ls -1 sys/*.o >> t
	ls -1 gen/*.o >> t
	ls -1 stdio/*.o >> t
	-if [ "$(D)" = overlay ] ; then ls -1 overlay/*.o >> t ; fi

t1:     t
	sort -t/ +1 -2 t > t1

ZAV:    t1
	lorder `cat t1` | \
sed 's?gen/cuexit\.o stdio/flsbuf\.o?stdio/flsbuf.o gen/cuexit.o?;s?stdio/doprnt\.o stdio/fltpr\.o?stdio/fltpr.o stdio/doprnt.o?' > ZAV
# s?stdio/doprnt\.o stdio/strout\.o?stdio/strout.o stdio/doprnt.o?'  >ZAV

ORDER:  ZAV
	tsort < ZAV > ORDER

$(LIB): $(ORD)
	@echo ">>>>> Make $(D) archive from object modules"
	-rm $(LIB)
	ar r $(LIB) `cat $(ORD)`

crtx:
	@echo ">>>>> Compile C startup routines"
	cd csu; make

clear clean:
	@echo '>>>>> Clear all'
	cd crt ; make clean
	cd csu ; make clean
	cd sys ; make clean
	cd gen ; make clean
	cd stdio  ; make clean
	cd ldiv ; make clean
	cd nonfpldiv ; make clean
	rm -f t1 t ZAV ORDER *.a libcom OVLIB BASELIB TMPLIB
	rm -f G_*
	rm -f *.[ob]

CLRSRC:
	@echo '>>>>> Clear all SRC'
	cd crt ; make CLRSRC
	cd csu ; make CLRSRC
	cd sys ; make CLRSRC
	cd gen ; make CLRSRC
	cd stdio  ; make CLRSRC
	cd ldiv ; make CLRSRC
	cd nonfpldiv ; make CLRSRC
	rm -f t1 t ZAV ORDER *.a libcom
	rm -f G_*
	rm -f *.[ob]

instlib: $(LIB)
	@echo ">>>>> Install library" $(DESTDIR)/lib/$(LIB)
#        -mv $(DESTDIR)/lib/$(LIB) $(DESTDIR)/lib/$(LIB).old </dev/tty
	cp $(LIB) ${DESTDIR}/lib/$(LIB)
	chown bin.bin ${DESTDIR}/lib/$(LIB)
	chmod 644 ${DESTDIR}/lib/$(LIB)
	-ranlib ${DESTDIR}/lib/$(LIB)

instcrt:
	@echo ">>>>> Install C startup routines"
	cd csu; make install

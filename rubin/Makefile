#
# Этот Makefile предназначен для установки системы, считанной с
# ленты, и для записи на ленту. Makefile для трансляции системы
# находится в source/conf/Makefile
VERSION=2.15.3
TYPE=Demos2.2_fcntl
Tell_me:
	@echo Type /make {idemos|ipdp|imc68|ivax|ism4|ix386|ix286} /

instALL:
	cp lib/libq.a /usr/lib
	-ranlib /usr/lib/libq.a
	-mkdir data;mkdir data/base
	-echo 'Run usersetup if instALL RUBIN at first'
	-mkdir /usr/include/rubin
	cp source/h/equel.h source/h/datetime.h /usr/include/rubin

ivax:   instALL
	rm -f files/proctab7;cp files/proctab7V files/proctab7
	rm -f files/equel7;cp files/equel7V files/equel7
	rm -f files/proctab7-Q;ln files/proctab7 files/proctab7-Q
	rm -f files/equel7-Q;ln files/equel7 files/equel7-Q

idemos:   instALL
	rm -f files/proctab7;cp files/proctab7V files/proctab7
	rm -f files/equel7;cp files/equel7V files/equel7
	rm -f files/proctab7-Q;ln files/proctab7 files/proctab7-Q
	rm -f files/equel7-Q;ln files/equel7 files/equel7-Q
	-mv /etc/rubin_lock /etc/rubin_lock-;rm -f  /etc/rubin_lock-
	cp bin/rubin_lock /etc
	echo Attempt to insert call  of /etc/rubin_lock to /etc/rc
	sed '$$s/exit 0/# exit 0/' /etc/rc > /etc/rc.new
	-grep -s "^[^#]*rubin_lock" /etc/rc.new || echo "(/etc/rubin_lock ;echo Rubin_lock started )> /dev/concole"  >> /etc/rc.new
	echo "exit 0" >> /etc/rc.new  && mv /etc/rc.new  /etc/rc

imc68 : instALL
	rm -f files/proctab7;cp files/proctab7V files/proctab7
	rm -f files/equel7;cp files/equel7V files/equel7
	rm -f files/proctab7-Q;ln files/proctab7 files/proctab7-Q
	rm -f files/equel7-Q;ln files/equel7 files/equel7-Q

ix386:  instALL
	rm -f files/proctab7;cp files/proctab7V files/proctab7
	rm -f files/equel7;cp files/equel7V files/equel7
	rm -f files/proctab7-Q;ln files/proctab7 files/proctab7-Q
	rm -f files/equel7-Q;ln files/equel7 files/equel7-Q
	mv /usr/lib/libq.a /usr/lib/386/Slibq.a

ism4:   instALL
	rm -f files/proctab7;cp files/proctab7S files/proctab7
	rm -f files/equel7;cp files/equel7S files/equel7
	rm -f files/proctab7-Q;cp files/proctab7S-Q files/proctab7-Q
	rm -f files/equel7-Q;cp files/equel7S-Q files/equel7-Q

ipdp:  instALL
	rm -f files/proctab7;cp files/proctab7X files/proctab7
	rm -f files/equel7;cp files/equel7X files/equel7
	rm -f files/proctab7-Q;cp files/proctab7X-Q files/proctab7-Q
	rm -f files/equel7-Q;cp files/equel7X-Q files/equel7-Q

ix286:  instALL
	rm -f files/proctab7;cp files/proctab7X files/proctab7
	rm -f files/equel7;cp files/equel7X files/equel7
	rm -f files/proctab7-Q;cp files/proctab7X-Q files/proctab7-Q
	rm -f files/equel7-Q;cp files/equel7X-Q files/equel7-Q
	mv /usr/lib/libq.a /usr/lib/Slibq.a
	cp lib/Mlibq.a /usr/lib/Mlibq.a
	ranlib /usr/lib/Mlibq.a
	cp lib/Llibq.a /usr/lib/Llibq.a
	ranlib /usr/lib/Llibq.a

.listbin: Makefile doc/* .sed.list
	-mkdir RUBIN_$(VERSION)
	-echo > RUBIN_$(VERSION)/$(TYPE)
	find .??* doc demo files -type f -print > list.tmp
	echo  RUBIN_$(VERSION)/$(TYPE)> .listbin
	echo Read_me    >> .listbin
	echo Makefile   >  list1.tmp
	echo data/base/.KEEP >> list1.tmp
	-echo > data/base/.KEEP
	sed -f .sed.list list.tmp >> list1.tmp
	echo .seta      >> list1.tmp
	find bin -type f -print | sed '/^bin\/cc/d' >> list1.tmp
	echo lib/libq.a >> list1.tmp
	echo lib/Mlibq.a >> list1.tmp
	echo lib/Llibq.a >> list1.tmp
	echo source/h/equel.h  >> list1.tmp
	echo source/h/datetime.h >> list1.tmp
	rm list.tmp
	sort list1.tmp >> .listbin

.listdoc: Makefile doc/LP Read_me
	echo Read_me > .listdoc
	find doc/LP -print | sort >> .listdoc

.listsrc .listobj: source/*
	find source -type f -print > list.tmp
	echo .sed.list  > list1.tmp
	echo .cshrc     >> list1.tmp
	echo .login     >> list1.tmp
	echo Read_me    >> list1.tmp
	echo Makefile   >> list1.tmp
	sed -n '/\.o$$/p' list.tmp > .listobj
	echo lib >> .listobj
	-find source/*/*lib -type f -print >> .listobj
	sed -f .sed.list list.tmp >> list1.tmp
	echo .seta      >> list1.tmp
	rm list.tmp
	mv list1.tmp .listsrc

tape: tarLP tarbin
# tape: tarLP tarbin tarsrc tarobj

tarLP:
	@ echo "LP started; Enter disk and press Enter.";read ask
	tar cv0 doc/LP
	@echo LP Done ";Press Enter";read ask


tarbin:    .listbin
	@ echo "BIN started; Enter disk and press Enter.";read ask
	tar cv0F .listbin
	-rm -rf RUBIN_$(VERSION)
	@echo Done ";Press Enter ;read ask

tarnew:    .listsrc
	tar cv0F .listsrc files doc demo .list*

tarsrc:    .listsrc
	tar cv0F .listsrc .list*

tarobj:    .listobj
	tar cv0F .listobj

tarall:  .listall
	tar cv0F list1

.listall: .listbin .listsrc .listobj
	cat .listsrc .listbin .listobj> list1
	sort list1 > list2;mv list2 list1
	uniq list1 > .listall
	rm -f list1

t1425:  .listbin  .listdoc
	@ echo "doc started; Enter disk and press Enter.";read ask
	tar cvfbBnhs /dev/rfd2 20 400 .listdoc
	@echo LP Done ";Press Enter";read ask
	@ echo "Bin started; Enter disk and press Enter.";read ask
	tar cvfbBsh /dev/rfd2 20 400 .listbin
	@echo Bin Done ";Press Enter";read ask

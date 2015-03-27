#>##########################################################################<#
#>#                                                                        #<#
#>#                                      *         *                       #<#
#>#                     ****  ***  ** *     * **  ***                      #<#
#>#                    *     *   * * * * *  **  *  *                       #<#
#>#                    *     *   * * * * *  *   *  *                       #<#
#>#                    *     *   * * * * *  *   *  *                       #<#
#>#                     ****  ***  * * *  * *   *   *                      #<#
#>#                                                                        #<#
#>#                           ЭКРАННЫЙ "SHELL"                             #<#
#>#                                                                        #<#
#>#           Автор: Флеров М.Н., ИПК Минавтопрома, апрель 1987.           #<#
#>#                                                                        #<#
#>#                                                                        #<#
#>#                                                                        #<#
#>##########################################################################<#

##### для BSD 2.9 установить $(S), для МНОС'а - убрать $(S) #####
#
#
# $Log:	Makefile,v $
# Revision 2.4  89/12/18  11:39:39  rygoff
# Исправлена синтаксическая ошибка на тему install.
# Make install по-прежнему не работает.
# Работает только make comint
# 
# Revision 2.3  89/12/03  14:41:31  rygoff
# Начато испревление install.
# Но make install все равно пока не работает.
# 
# Revision 2.2  89/12/03  12:23:04  rygoff
# Если при компиляции есть синтаксические ошибки,
# object не создается, но зато не стирается по .PRECIOUS старый object.
# А при ошибках make не сваливается - запрещено.
# Поэтому при неудачной компиляции принудительно стираем старый object.
# 
# Revision 2.1  89/10/30  19:05:18  rygoff
# Заведена новая ветвь в связи с изменением структуры управления
# См. подробнее комментарии в main.c и exefound.c
# 
# 
# Revision 1.7  89/10/06  16:19:51  rygoff
# Добавил отладочный вариант $N_d (без ключа -s при сборке)
#
# Revision 1.6  89/08/26  14:26:56  rygoff
# Вернул работу с test'ом, ускорив насколько можно.
# вроде работает как надо
#
# Revision 1.5  89/08/25  13:15:25  rygoff
# Бросил возню с test'ами - очень медленно. Если не хватает object'a,
# можно вручную вытащить source, дальше все автоматически.
#
# Revision 1.4  89/08/22  00:17:46  rygoff
# Если в каталоге есть object, но нет source, ничего не делать
# К сожалению, приходится много чего проверять test'ом.
# Работает невыносимо медленно
#
# Revision 1.3  89/08/19  19:10:06  rygoff
# Вынес include-файлы в начало блока src.
# Переопределил сборку *.cm, (cc говорит "ошибка в mm")
#
# Revision 1.2  89/08/19  17:23:20  rygoff
# Добавлена работа с RCS
#
# Revision 1.1  89/08/19  16:24:12  rygoff
# Initial revision
#
#
###########################################################################

CC      = cc
# Never use rcc from SCO UNIX !
# It produces incorrect objects !

S       = -DUSG -DSYSV -DM_I386
P       = ..
#L      = -lx ../dpi/libdpi.a
#L      = -lx ../dpi/libdpi.a -ltermcap
L       = ../dpi/libdpi.a -ltermcap
CFLAGS  = $S -O -I. -I../dpi -I/usr/include -I/usr/include/sys
N       = comint
E       = $P/distribute/$N
R       = ./RCS

.SUFFIXES:      .cm .c .o .c,v .cm,v .h .h,v

###########################################################################

src   = comint.h   run.h                \
	main.c                exefound.c            change.c              \
		   ind_err.c  getcat.c   indcat.c   shbuf.c   ind_date.c  \
	getsuit.c  search.c              savmark.c  start.c   protname.c  \
	indrun.c   waitpid.c  tokill.c   inp_name.c shell.c   strt.c      \
	cat.c      ls.c       rm.c       rename.c   copy.c    execvp.c    \
	mkdir.c    file_cat.c conv.c     cm_line.c  gettune.c setenv.c    \
	dpj.o      fnct.c     vdpbeg.c   depend.cm

obj   = main.o                exefound.o            change.o              \
		   ind_err.o  getcat.o   indcat.o   shbuf.o   ind_date.o  \
	getsuit.o  search.o              savmark.o  start.o   protname.o  \
	indrun.o   waitpid.o  tokill.o   inp_name.o shell.o   strt.o      \
	cat.o      ls.o       rm.o       rename.o   copy.o    execvp.o    \
	mkdir.o    file_cat.o conv.o     cm_line.o  gettune.o setenv.o    \
	dpj.o      fnct.o     vdpbeg.o   depend.o

###########################################################################

all:    $N

$N:              $(obj) Makefile ../dpi/libdpi.a
	$(CC) -s $(CFLAGS) $(obj) -o $@ $L

$N_d:            $(obj) Makefile        # Для отладки не вытираем имена
	$(CC) $(obj) -o $@ $L $S

.c.o:
	$(CC) $(CFLAGS) -c $<

.cm.o:
	mm <$*.cm >$*.c  &&  $(CC) $(CFLAGS) -c $*.c  &&  rm $*.c


out:
	co -l -q ${src}


../dpi/libdpi.a:
	cd ../dpi && make

.DEFAULT:
	 co -q -l $@

###########################################################################

gettune.o      ind_err.o      cm_line.o      cat.o          indcat.o      \
shell.o        start.o        rename.o       mkdir.o        copy.o        \
rm.o           getcat.o       search.o       getsuit.o      depend.o      \
ls.o           strt.o         savmark.o      file_cat.o     shbuf.o       \
main.o         indrun.o       inp_name.o     change.o       exefound.o    \
			  : comint.h

main.o  exefound.o indrun.o tokill.o waitpid.o  start.o   strt.o  : run.h

clear:
	-rm -f *.[obO] $N



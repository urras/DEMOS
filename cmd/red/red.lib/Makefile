#
#
# Библиотека редактора red - справочные файлы и таблицы
# Всем этим компонентам место в /usr/lib/red, за исключением:
# re.std -> /usr/lib
# termcap -> /etc
#
# Здесь содержатся файлы трех типов
# 1. Таблицы кодировки (транслируются и запускаются).
# 2. Таблицы определения команд
# 3. Справочные файлы
# Кроме того, содержатся файл termcap и общий HELP re.std
#
ALL =$(CYRTABS) $(KEYTABS) $(HELP)
HELP=help.sm7209 help.wyse75 help.VT200 help.VT200C help.xenix
KEYTABS=common.key xenix.key
CYRTABS=Wyse75 VT200 PVT100

all:  $(ALL)


Wyse75:  Wyse75.c
	cc Wyse75.c
	a.out > Wyse75
	rm -f a.out

PVT100:PVT100.c
	cc PVT100.c
	a.out > PVT100
	rm -f a.out

VT200:  VT200.c
	cc VT200.c
	a.out > VT200
	rm -f a.out




install: all
	cp re.std /usr/lib
	-mkdir /usr/lib/red
	cp $(ALL) /usr/lib/red

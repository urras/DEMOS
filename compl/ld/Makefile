#  �������� ��������, ����������� ������ � overlay - �����������
#  � �� ����-��� � ����-���
# �������� ������� ��:
# �������� ������ ld, ����������� �� ��������� �� ����������� (-v <�������>)
# ���������� �������� libovl.a (�lds.o � Ostart.o)
# ��������� ��������� ������������� ogensw
# ������� ������ ���������� ldover
#
# ogensw � libovl.a ������������ � "/lib" ��� "/usr/lib"
DESTDIR=${HOME}/d22
#CFLAGS=-O
CFLAGS=-g
#LDFLAGS=-n
CC_CROSS=$(HOME)/d22/bin/cc
AR=$(HOME)/d22/bin/ar

all:        ld ogensw libovl.a  ldover.sh
ogensw:     ogensw.c
	cc ${CFLAGS} ${LDFLAGS} -o ogensw ogensw.c
ld:         ld.c
	cc ${CFLAGS} ${LDFLAGS} -o ld ld.c
libovl.a:   Oload.o Olds.o
	rm -f libovl.a;$(AR) r libovl.a Oload.o Olds.o
#Oload.o:    Oload.c Otabe.h Otabs.h
#	$(CC_CROSS) -c $ Oload.c
Olds.o:     Olds.s
	$(CC_CROSS) -c $*.s
install:    ld libovl.a ldover.sh ogensw
	@echo "� �� �����-������������?"
	cp ld ${DESTDIR}/bin/ld
	cp libovl.a ogensw ${DESTDIR}/lib
	cp ldover.sh ${DESTDIR}/bin/ldover
	chmod a-w ${DESTDIR}/bin/ldover
# �����:
testo:      test1.o test2.o test.o ldd libovl.a ldover.sh ogensw
	ln ldd ld
	ldover.sh -M . test.o -o testo { test1.o , test2.o }
	rm -f ld
	testo V.out.a ������
clear:
	rm -f *.o V.out.a testo *.ov

#
#
# ���������� ��������� red - ���������� ����� � �������
# ���� ���� ����������� ����� � /usr/lib/red, �� �����������:
# re.std -> /usr/lib
# termcap -> /etc
#
# ����� ���������� ����� ���� �����
# 1. ������� ��������� (������������� � �����������).
# 2. ������� ����������� ������
# 3. ���������� �����
# ����� ����, ���������� ���� termcap � ����� HELP re.std
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

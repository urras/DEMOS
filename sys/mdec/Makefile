#
# Makefile ��� ����������� � ��������� ������
#
# $Log:	Makefile,v $
# Revision 1.11  90/01/08  19:28:40  rygoff
# � ���������� ������� ��������� ����� ������� ����������.
# �������� �����.
# 
# Revision 1.10  90/01/08  19:23:19  rygoff
# ��������� ����������� -DNHEADS=8 
# ��� ���������� �����������-85 � ����� � ���������� ��������������� ���������
# (8 �������). ��� ����������� ��������� ���� ���������� � Makefile
# -DNHEADS=4
# 
# Revision 1.9  90/12/12  17:08:07  korotaev
# ������ ��� my � dw (���).
# 
# Revision 1.8  88/10/01  12:15:57  root
# ��� rluboot �������� NOTTY
# 
# Revision 1.7  88/01/21  17:36:37  root
# �������� xpuboot( link to hpuboot)
#
# Revision 1.6  88/01/15  14:06:17  korotaev
# ��������� ������� � ��������� rauboot, ������ ��������� ������ (!!!).
# ��� ������� � ����.
#
# Revision 1.5  88/01/13  18:31:27  korotaev
# ��������� ��������� �� �������� RA.
#
# Revision 1.4  88/01/05  16:26:00  korotaev
# ��������� ����� �������� ���� ��� � ����.
#
# Revision 1.3  86/07/13  14:09:12  avg
# ��������� ������ ��� DW:  RWF ��� ����� ������ RW.
#
# Revision 1.2  86/07/10  22:53:50  avg
# ��������� ������������� ������.
#
# Revision 1.1  86/07/10  22:46:05  avg
# Initial revision
#
#

# ��� ������ ���. ������� ��� RCS ���������� R=X
R=
RCS=RCS
COFLAGS=

DESTDIR=/mdec

CC = $(HOME)/d22/bin/cc
LD = $(HOME)/d22/bin/ld
AS = $(HOME)/d22/bin/as
STRIP = $(HOME)/d22/bin/strip

#
# ����� ��� �������� ���������� �����������
#

#--- �� 5408
HKF=RK06
# HKF=RK07

HPF=RP04
# HPF=RP05
# HPF=RP06
# HPF=DVHP

#--- �� 5400
RKF=RK05

RLF=RL01 -DNOTTY
# RLF=RL02

RMF=RM05
# RMF=RM02
# RMF=RM03

#--- �� 5061
RPF=RP03

#--- ��������� RD51 (�����������-85)
RWF=DW  -DNOTTY  -DEL85 -DNHEADS=8.   # ��� ������������ ����������
#                                     # ���� ���������� NHEADS=4

#--- ��������� ���-4
DWF=DW  -DNOP

#--- ������ ����������-85
FDF=FD  -DNOTTY  -DEL85

#--- ������ RX
RXF=RX

#--- ������ RX.02
RYF=RX02

#--- ������ ���-4 MY (�������������)
MYF=MY -DNOP
#--- ������ ���-4 MY (�������������)
MY2F=MY -DNOP -DDOUBLE

BOOTS=hkuboot hpuboot rkuboot rluboot rmuboot rpuboot rwuboot\
 fduboot rauboot rxuboot ryuboot myuboot myuboot.2 dwuboot
A=as -o $@.o $@.s;size $@.o;strip\
 $@.o;dd if=$@.o of=$@ bs=8w skip=1;rm -f $@.o

###########

all: $(BOOTS)

install: $(BOOTS)
	-cd $(DESTDIR); rm -f $(BOOTS)
	cp $(BOOTS) $(DESTDIR)
	cd $(DESTDIR); chmod 440 $(BOOTS); rm  -f xpuboot;ln hpuboot rpuboot

f-uboot:	f-uboot.s
	$(AS) -o f-uboot f-uboot.s
	$(STRIP) f-uboot

hkuboot:        hkuboot.s ; $(A)
hpuboot:        hpuboot.s ; $(A)
rkuboot:        rkuboot.s ; $(A)
rluboot:        rluboot.s ; $(A)
rmuboot:        rmuboot.s ; $(A)
rpuboot:        rpuboot.s ; $(A)
rwuboot:        rwuboot.s ; $(A)
fduboot:        fduboot.s ; $(A)
rauboot:        rauboot.s ; $(A)
rxuboot:        rxuboot.s ; $(A)
ryuboot:        ryuboot.s ; $(A)
myuboot:        myuboot.s ; $(A)
myuboot.2:      myuboot.2.s ; $(A)
dwuboot:        dwuboot.s ; $(A)

hkuboot.s hpuboot.s rkuboot.s rluboot.s rmuboot.s rpuboot.s dwuboot.s\
rwuboot.s fduboot.s rxuboot.s ryuboot.s myuboot.2.s myuboot.s: uboot.s Makefile

hkuboot.s:;     /lib/cpp -P -D$(HKF) uboot.s >hkuboot.s
hpuboot.s:;     /lib/cpp -P -D$(HPF) uboot.s >hpuboot.s
rkuboot.s:;     /lib/cpp -P -D$(RKF) uboot.s >rkuboot.s
rluboot.s:;     /lib/cpp -P -D$(RLF) uboot.s >rluboot.s
rmuboot.s:;     /lib/cpp -P -D$(RMF) uboot.s >rmuboot.s
rpuboot.s:;     /lib/cpp -P -D$(RPF) uboot.s >rpuboot.s
rwuboot.s:;     /lib/cpp -P -D$(RWF) uboot.s >rwuboot.s
fduboot.s:;     /lib/cpp -P -D$(FDF) uboot.s >fduboot.s
rxuboot.s:;     /lib/cpp -P -D$(RXF) uboot.s >rxuboot.s
ryuboot.s:;     /lib/cpp -P -D$(RYF) uboot.s >ryuboot.s
myuboot.s:;     /lib/cpp -P -D$(MYF) uboot.s >myuboot.s
myuboot.2.s:;   /lib/cpp -P -D$(MY2F) uboot.s >myuboot.2.s
dwuboot.s:;     /lib/cpp -P -D$(DWF) uboot.s >dwuboot.s

source: uboot.s Makefile

$(R)rauboot.s:  Makefile
	co $(COFLAGS) rauboot.s

$(R)uboot.s:
	co $(COFLAGS) uboot.s

$(R)Makefile:
	co $(COFLAGS) Makefile

CLRSRC: clear
	-rm -f uboot.s

clear clean:
	-rm -f ??uboot.s
	-rm -f *.[ob]
	-rm -f ??uboot

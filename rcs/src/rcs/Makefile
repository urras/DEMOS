# $Header: /usr/users/korotay/DEMOS_2.2/rcs/src/rcs/RCS/Makefile,v 1.6 1988/09/21 22:26:53 avg Exp $
# $Log: Makefile,v $
# Revision 1.6  1988/09/21  22:26:53  avg
# �������� ������������ �������� lib.
#
# Revision 1.5  86/02/24  17:52:39  avg
# �������� ���� CLRSRC ��� �������� �������� �������.
#
# Revision 1.4  86/02/24  16:40:34  avg
# ������� ������� ������ ������� ������ ��� co.
#
# Revision 1.3  86/02/23  21:47:13  avg
# ������� ��������� ��� ������ � ���� �� 1.2.
#
# Revision 1.2  86/01/02  15:41:27  fox
# ������� E_82 � ������� ����� ������ ��� �����
# ����������� � ���������� VAX.
#
# Revision 1.1  85/12/27  05:41:08  fox
# Initial revision
#

# P3
TOP       = ${HOME}/d22

#          � � � � � � � � �   � �  � � � � � � � � �
#          ==========================================
#
# 0.  ��������� ��������� � ����� rcsbase.h
#
# 1.  ������� ���������������� �������� ������ diff � diff3 �
#     ���������� ��. (������������� ����������� � ������������ �
#     ��������������� Make-������).
#     ������ ����� ���������� ������ ��������� ���������
#     ��������������� DIFF � DIFF3 (���� ��� ����������):

DIFF      = $(TOP)/lib/rcs/rdiff
DIFF3     = $(TOP)/lib/rcs/rdiff3

# 2.  �������������� ���������������� DIFF3 � ��������� ����� merge.

# 3.  ������� ���������� � ������� ����� ��������� ������� RCS
#     (����� DESTDIR) � ������ ��� ������� merge (����� MERGE):

MERGE     = $(TOP)/lib/rcs/merge
DESTDIR   = $(TOP)/bin

# 4.  ���������� ���� ���������� "-i" (���������� ������ ������ � ������),
#     ���� �� ���������. ������ ���� ��������� ��� �������� ���
#     ��� ����������� � �������� PDP11/44 - PDP11/70 (��������,
#     �����������-79). ��� ��� ���� ��-4, ��-1420, �.�.�., � �����
#     ��� ��-1700 � �����������-82 ���� "-i" �� �����������:

LDFLAGS   =
#LDFLAGS  = -i

# 5.  ������� RCS ����� ��������������� ��� ����� �������������
#     ������ co � ci. ���� ���� ����� ���������, ���������:
#     a) ������ ������ ��� ��������� snoop (� ����� SNOOP),
#        ������� ����� ������ � ���� ���������� � co � ci;
#     b) ������� ������ ��� ����� ��� ������ ��������� (���� log)
#        � ����� SNOOPFILE (���� log � ������� snoop ������ �����
#        ������ � ���� �� ���������).
#     c) �������� ����� ����� SNOOP � SNOOPFILE � ����� CFLAGS.
#
#     ���� ����� ���������������� �� ������������, ����� ������ ��������
#     SNOOP � SNOOPFILE �� ����� CFLAGS.
#     ���� ���������� ��� �������� ������� CFLAGS:

SNOOPFILE = /usr/adm/rcslog
SNOOP     = /usr/lib/rcs/snoop
#CFLAGS   = -O -DDIFF='"$(DIFF)"' -DMERGE='"$(MERGE)"' -DTARGETDIR='"$(DESTDIR)"' -DSNOOPFILE='"$(SNOOPFILE)"' -DSNOOP='"$(SNOOP)"'
#CFLAGS    = -O -DDIFF='"$(DIFF)"' -DMERGE='"$(MERGE)"' -DTARGETDIR='"$(DESTDIR)"'
CFLAGS    = -g -DDIFF='"$(DIFF)"' -DMERGE='"$(MERGE)"' -DTARGETDIR='"$(DESTDIR)"'

#   �������� ��� co. ( ������ - ��������� ������ )

COFLAGS=

# 6.  ��������� ��������, ����������� ��������� ������� "make all",
#     ���� ��� �������� ������ �������, ��������� �������
#     "make INSTALLNO" (��� "make INSTALLW" - ��� �������� �
#     ����������������� �������);

# 7,  ���� �������������� ������������ RCS ��� �������� �����
#     �������, ����������� ��������� �������� �������� ������
#     (������ ������, ���� � �.�.). ��� ���� ���� � �������
#     ������ � ����� ������������ ���� -k. ����� ������ �������
#     �������� ����������� � ����� � ����� ������� �����.

# 8.  ���� ��� ������������� � ��������� RCS �� �������� ������,
#     ��������� NOR = X
NOR=


OBJECTS = rcslex.o rcssyn.o rcsrev.o rcsgen.o rcssynTST.o rcsrevTST.o\
	rcsedit.o rcsutil.o rcsfnms.o co.o ci.o ident.o rlog.o rcs.o\
	rcsdiff.o rcsmerge.o rcskeep.o rcsfcmp.o snoop.o
RCSCOMMANDS = ci ident rcs rcsdiff rcsmerge rlog co

all:            merge $(RCSCOMMANDS)

INSTALLW:       INSTALLSNOOP INSTALLNO

install:        INSTALLNO

INSTALLNO:      merge $(RCSCOMMANDS)
# ����������...
#		-strip $(RCSCOMMANDS)
#       �������� ������������ �� ������ merge.
		grep -v '^#' merge > $(MERGE)
		chmod 755 $(MERGE) $(RCSCOMMANDS)

#       ���������� ������ ������ RCS (���� �������).
#               mv $(DESTDIR)/co $(OLDDIR)/co
#               mv $(DESTDIR)/ci $(OLDDIR)/ci
#               mv $(DESTDIR)/rlog $(OLDDIR)/rlog
#               mv $(DESTDIR)/rcs $(OLDDIR)/rcs
#               mv $(DESTDIR)/ident $(OLDDIR)/ident
		mv $(RCSCOMMANDS) $(DESTDIR)

INSTALLSNOOP:   snoop
		strip snoop
		mv snoop $(SNOOP)
		chmod u+s $(SNOOP)
		chmod 644 $(SNOOPFILE)
# ��������: ���������� ������� ��������� ��� SNOOP � SNOOPFILE.

clean clear:    ; rm -f *.o $(RCSCOMMANDS) snoop

depend:;

$(OBJECTS):     rcsbase.h

partime.o maketime.o co.o ci.o rlog.o rcsutil.o:        time.h

CI = ci.o rcslex.o rcssyn.o rcsgen.o rcsedit.o rcsrev.o rcsutil.o rcsfnms.o rcskeep.o rcsfcmp.o

ci:             $(CI); cc $(LDFLAGS) -o ci $(CI)

CO = co.o rcslex.o rcssyn.o rcsgen.o rcsedit.o rcsrev.o rcsutil.o rcsfnms.o partime.o maketime.o

co:             $(CO); cc $(LDFLAGS) -o co $(CO)

ident:          ident.o
		cc -o ident ident.o

RLOG = rlog.o rcslex.o rcssyn.o rcsrev.o rcsutil.o partime.o maketime.o rcsfnms.o

rlog:           $(RLOG); cc $(LDFLAGS) -o rlog $(RLOG)

RCS = rcs.o rcslex.o rcssyn.o rcsrev.o rcsutil.o rcsgen.o rcsedit.o rcsfnms.o

rcs:            $(RCS); cc $(LDFLAGS) -o rcs $(RCS)

RCSDIFF = rcsdiff.o rcsutil.o rcsfnms.o rcsrev.o rcssyn.o rcslex.o

rcsdiff:        $(RCSDIFF); cc -o rcsdiff $(RCSDIFF)

RCSMERGE = rcsmerge.o rcsutil.o rcsfnms.o rcsrev.o rcssyn.o rcslex.o

rcsmerge:       $(RCSMERGE); cc -o rcsmerge $(RCSMERGE)

snoop:          snoop.o
		cc -o snoop snoop.o

PAIRTEST = rcsfnmsTST.o rcssyn.o rcslex.o

pairtest:       $(PAIRTEST); cc -o pairtest $(PAIRTEST)

rcsfnmsTST.o:   rcsfnms.c
		cc -c -DPAIRTEST rcsfnms.c
		mv rcsfnms.o rcsfnmsTST.o

REVTEST = rcsrevTST.o rcssyn.o rcslex.o

revtest:        $(REVTEST); cc $(REVTEST)
		echo "a.out ../TEST/rev.test.v <../TEST/rev.test.in >&! tmp" |csh
		diff tmp ../TEST/rev.test.out
		rm tmp
		mv a.out revtest

rcsrevTST.o:    rcsrev.c
		cc -c -DREVTEST rcsrev.c
		mv rcsrev.o rcsrevTST.o

SYNTEST = rcslex.o rcssynTST.o rcsrev.o

syntest:        $(SYNTEST); cc $(SYNTEST)
		echo "a.out ../TEST/syn.test >&! tmp" | csh
		diff tmp ../TEST/syn.result
		rm tmp
		mv a.out syntest

rcssynTST.o:    rcssyn.c
		cc -c -DSYNDB -DCOMPAT2 -DSYNTEST rcssyn.c
		mv rcssyn.o rcssynTST.o

lextest:        rcslex.c
		cc -DLEXDB rcslex.c
		echo "a.out ../TEST/lex.test >&! tmp" | csh
		diff tmp ../TEST/lex.result
		rm tmp
		mv a.out lextest

#     ������ �������� ������� �� �������� ������ RCS.

$(NOR)rcsbase.h:     RCS/rcsbase.h,v;  co ${COFLAGS} rcsbase.h
$(NOR)rcsedit.c:     RCS/rcsedit.c,v;  co ${COFLAGS} rcsedit.c
$(NOR)rcsfcmp.c:     RCS/rcsfcmp.c,v;  co ${COFLAGS} rcsfcmp.c
$(NOR)rcsfnms.c:     RCS/rcsfnms.c,v;  co ${COFLAGS} rcsfnms.c
$(NOR)rcsgen.c:      RCS/rcsgen.c,v;   co ${COFLAGS} rcsgen.c
$(NOR)rcskeep.c:     RCS/rcskeep.c,v;  co ${COFLAGS} rcskeep.c
$(NOR)rcslex.c:      RCS/rcslex.c,v;   co ${COFLAGS} rcslex.c
$(NOR)rcsrev.c:      RCS/rcsrev.c,v;   co ${COFLAGS} rcsrev.c
$(NOR)rcssyn.c:      RCS/rcssyn.c,v;   co ${COFLAGS} rcssyn.c
$(NOR)rcsutil.c:     RCS/rcsutil.c,v;  co ${COFLAGS} rcsutil.c
$(NOR)ci.c:          RCS/ci.c,v;       co ${COFLAGS} ci.c
$(NOR)co.c:          RCS/co.c,v;       co ${COFLAGS} co.c
$(NOR)ident.c:       RCS/ident.c,v;    co ${COFLAGS} ident.c
$(NOR)merge:         RCS/merge,v;      co ${COFLAGS} merge
$(NOR)rcs.c:         RCS/rcs.c,v;      co ${COFLAGS} rcs.c
$(NOR)rcsdiff.c:     RCS/rcsdiff.c,v;  co ${COFLAGS} rcsdiff.c
$(NOR)rcsmerge.c:    RCS/rcsmerge.c,v; co ${COFLAGS} rcsmerge.c
$(NOR)rlog.c:        RCS/rlog.c,v;     co ${COFLAGS} rlog.c
$(NOR)snoop.c:       RCS/snoop.c,v;    co ${COFLAGS} snoop.c
$(NOR)time.h:        RCS/time.h,v;     co ${COFLAGS} time.h
$(NOR)partime.c:     RCS/partime.c,v;  co ${COFLAGS} partime.c
$(NOR)maketime.c:    RCS/maketime.c,v; co ${COFLAGS} maketime.c

CLRSRC:;        -rm *.[cobh] merge

#>##########################################################################<#
#>#                                                                        #<#
#>#                                      *         *                       #<#
#>#                     ****  ***  ** *     * **  ***                      #<#
#>#                    *     *   * * * * *  **  *  *                       #<#
#>#                    *     *   * * * * *  *   *  *                       #<#
#>#                    *     *   * * * * *  *   *  *                       #<#
#>#                     ****  ***  * * *  * *   *   *                      #<#
#>#                                                                        #<#
#>#                           �������� "SHELL"                             #<#
#>#                                                                        #<#
#>#           �����: ������ �.�., ��� ������������, ������ 1987.           #<#
#>#                                                                        #<#
#>#                                                                        #<#
#>#                                                                        #<#
#>##########################################################################<#

##### ��� BSD 2.9 ���������� $(S), ��� ����'� - ������ $(S) #####
#
#
# $Log:	Makefile,v $
# Revision 2.4  89/12/18  11:39:39  rygoff
# ���������� �������������� ������ �� ���� install.
# Make install ��-�������� �� ��������.
# �������� ������ make comint
# 
# Revision 2.3  89/12/03  14:41:31  rygoff
# ������ ����������� install.
# �� make install ��� ����� ���� �� ��������.
# 
# Revision 2.2  89/12/03  12:23:04  rygoff
# ���� ��� ���������� ���� �������������� ������,
# object �� ���������, �� ���� �� ��������� �� .PRECIOUS ������ object.
# � ��� ������� make �� ����������� - ���������.
# ������� ��� ��������� ���������� ������������� ������� ������ object.
# 
# Revision 2.1  89/10/30  19:05:18  rygoff
# �������� ����� ����� � ����� � ���������� ��������� ����������
# ��. ��������� ����������� � main.c � exefound.c
# 
# 
# Revision 1.7  89/10/06  16:19:51  rygoff
# ������� ���������� ������� $N_d (��� ����� -s ��� ������)
#
# Revision 1.6  89/08/26  14:26:56  rygoff
# ������ ������ � test'��, ������� ��������� �����.
# ����� �������� ��� ����
#
# Revision 1.5  89/08/25  13:15:25  rygoff
# ������ ����� � test'��� - ����� ��������. ���� �� ������� object'a,
# ����� ������� �������� source, ������ ��� �������������.
#
# Revision 1.4  89/08/22  00:17:46  rygoff
# ���� � �������� ���� object, �� ��� source, ������ �� ������
# � ���������, ���������� ����� ���� ��������� test'��.
# �������� ���������� ��������
#
# Revision 1.3  89/08/19  19:10:06  rygoff
# ����� include-����� � ������ ����� src.
# ������������� ������ *.cm, (cc ������� "������ � mm")
#
# Revision 1.2  89/08/19  17:23:20  rygoff
# ��������� ������ � RCS
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

$N_d:            $(obj) Makefile        # ��� ������� �� �������� �����
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



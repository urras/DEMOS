#
# Makefile для второй фазы загрузки системы
#
# $Log:	Makefile,v $
# Revision 1.4  88/11/02  22:15:58  dvolodin
# *** empty log message ***
# 
# Revision 1.3  87/09/29  16:03:48  avg
# Добавлена возможность указания своего устройства загрузки
# (в отличие от того, которое в sys/reboot.h).
# см. BOOTDEV в Makefile.
#
# Revision 1.2  87/07/14  13:50:45  dmitry
# У boot введена чистка таблицы имен.
#
# Revision 1.1  86/12/13  15:41:41  alex
# Initial revision
#
#

CC = $(HOME)/d22/bin/cc
AS = $(HOME)/d22/bin/as
LD = $(HOME)/d22/bin/ld
NM = $(HOME)/d22/bin/nm
STRIP = $(HOME)/d22/bin/strip

# Устройство загрузки (по умолчанию берется из sys/reboot.h).
# BOOTDEV= '-DMY_DEFNAME="rp(0,0)confa"'
# BOOTDEV= '-DMY_DEFNAME="tm(0,1)"'     # для дистрибутива ДЕМОС на СМ-4 etc
# BOOTDEV= '-DMY_DEF85="fd(0,0)confa"'  # для дистрибутива ДЕМОС на Э-85

INCLUDES=	-I..
CFLAGS =	${INCLUDES} -DDVK4

# Suppress "-target sun4" from default rule
.c.o:
	$(CC) $(CFLAGS) -c $<

#../boot:        boot.o bootpar.o M.o ubmapset.o bootdev.o ../libsa.a ../c.o
#	$(LD) -o ../boot M.o ubmapset.o boot.o bootpar.o bootdev.o\
# ../c.o ../libsa.a -lc
../boot:        boot.o bootpar.o M.o ubmapset.o bootdev.o ../libsa.a ../c.o
	$(LD) -o ../boot M.o ubmapset.o boot.o bootpar.o\
 bootdev.o ../c.o ../dummyfont.o ../libsa.a l3.o strcmp.o strcpy.o\
 csv.o atol.o nonfops.o
	$(NM) -g ../boot > ../boot.nm
	$(STRIP) ../boot

../c.o : ../c.c
	$(CC) $(CFLAGS) -c ../c.c
	mv c.o ../c.o

boot.o: boot.c
	$(CC) $(CFLAGS) -c $(BOOTDEV) boot.c

M.o:	M.s
	$(AS) -u -o $*.o $*.s

bootpar.o:	bootpar.c
ubmapset.o:	ubmapset.c
bootdev.o:	bootdev.c

source: Makefile boot.c M.s ubmapset.c

clean clear:
	-rm -f *.[ob]

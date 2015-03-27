#
#  Makefile for RUBIN EQUEL runtime library
#
#	@(#)Makefile	7.1	2/5/81
#

RUBIN=	../..
H=	../h
LIB=	$(RUBIN)/lib

OBJS=\
IIw_left.o\
IIw_right.o\
IIcvar.o\
IIgettup.o\
IIn_get.o\
IIn_ret.o\
IIretrieve.o\
IIsetup.o\
IIflushtup.o\
IIsync.o\
IIwrite.o\
IIexit.o\
IIconvert.o\
IIreadinp.o\
IIpb_get.o\
IIpb_put.o\
IIpb_flush.o\
IIrubin.o\
IIresync.o\
IIpb_write.o\
IIpb_wphys.o\
IIpb_prime.o\
IIpb_read.o\
IIpb_rphys.o\
IIbmove.o\
IIsequal.o\
IIsyserr.o\
IIgetc.o\
IIitos.o\
IIerror.o\
IIp_err.o\
IIconcatv.o\
IIlength.o\
IIatoi.o\
form_in.o\
form_out.o\
form_date.o\
form_compl.o\
form_conv.o\
IIsmove.o\
sysdati.o

SRCS=	IIw_left.c IIw_right.c IIcvar.c IIgettup.c IIn_get.c IIn_ret.c \
	IIrubin.c IIretrieve.c IIsetup.c IIflushtup.c IIsync.c \
	IIwrite.c IIexit.c IIsyserr.c IIconvert.c IIerror.c IIp_err.c\
	IIgetc.c IIresync.c IIreadinp.c IIpb_get.c IIpb_put.c IIpb_read.c \
	IIpb_rphys.c IIpb_flush.c IIpb_write.c IIpb_wphys.c IIpb_prime.c \
	IIitos.c  IIconcatv.c IIbmove.c IIlength.c IIatoi.c IIsequal.c \
	IIglobals.h \
	IIsmove.c\
	../iutil/form_date.c\
	../iutil/form_conv.c\
	../iutil/form_in.c\
	../iutil/form_out.c\
	../iutil/form_compl.c\
	../iutil/sysdati.c


SYSLIB=	/usr/local/lib

CFLAGS= -DEQUEL -I$H -I../ctlmod -O
AR=	-ar
ARFLAGS=rvu
RANLIB= ls -l
CP=     ln
CHMOD=	chmod
CHOWN=	-chown
LIBMODE=664
GET=
REL=
PR=	print
VGRIND=	csh /usr/ucb/vgrind
HEADER=	'Equel Runtime Library'

libq.a: $(OBJS)
	$(AR) $(ARFLAGS) $@  $?
	$(CHMOD) $(LIBMODE) libq.a
	$(RANLIB) libq.a

install: $(LIB)/libq.a

$(LIB)/libq.a: libq.a
	rm -f $(LIB)/libq.a
	$(CP) libq.a $(LIB)/libq.a
	$(RANLIB) $(LIB)/libq.a

sysinstall: $(SYSLIB)/libq.a

$(SYSLIB)/libq.a: libq.a
	$(CP) libq.a $(SYSLIB)/libq.a
	$(CHOWN) rubin $(SYSLIB)/libq.a
	$(RANLIB) $(SYSLIB)/libq.a

sources: $(SRCS)

form_date.o: ../iutil/form_date.c
	$(CC) -c $(CFLAGS) ../iutil/form_date.c
form_conv.o: ../iutil/form_conv.c
	$(CC) -c $(CFLAGS) ../iutil/form_conv.c
form_in.o:   ../iutil/form_in.c
	$(CC) -c $(CFLAGS) ../iutil/form_in.c
form_out.o:  ../iutil/form_out.c
	$(CC) -c $(CFLAGS) ../iutil/form_out.c
form_compl.o:../iutil/form_compl.c
	$(CC) -c $(CFLAGS) ../iutil/form_compl.c
sysdati.o:   ../iutil/sysdati.c
	$(CC) -c $(CFLAGS) ../iutil/sysdati.c

new:
	rm -f libq.a

clean:
	rm -f *.o
	rm -f a.out core libq.a

print: sources
	$(PR) Makefile TraceFlags *.[hm] *.[csy]

vgrind: sources
	cp /dev/null index
	$(VGRIND) -h "$(HEADER) (Release 7)" -n Makefile TraceFlags
	$(VGRIND) -h "$(HEADER) (Release 7)" *.[hm] *.[csy]
	$(VGRIND) -h "$(HEADER) (Release 7)" -x index

depend:
	grep '^#[ 	]*include' $(SRCS) | sed -n -f ../h/depend.sed > makedep
	echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile Makefile.bak
	ed - Makefile < eddep
	rm eddep makedep
	echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	echo '# see make depend above' >> Makefile

# DO NOT DELETE THIS LINE -- make depend uses it
# DEPENDENCIES MUST END AT END OF FILE
IIw_left.o:$H/defines.h $H/local.h
IIw_left.o:../equel/constants.h
IIw_left.o:IIglobals.h
IIw_right.o:$H/defines.h $H/local.h
IIw_right.o:../equel/constants.h
IIw_right.o:IIglobals.h
IIcvar.o:$H/defines.h $H/local.h
IIcvar.o:$H/symbol.h
IIcvar.o:IIglobals.h
IIgettup.o:$H/defines.h $H/local.h
IIgettup.o:$H/symbol.h
IIgettup.o:IIglobals.h
IIn_get.o:$H/defines.h $H/local.h
IIn_get.o:$H/symbol.h
IIn_get.o:IIglobals.h
IIn_ret.o:$H/defines.h $H/local.h
IIn_ret.o:$H/symbol.h
IIn_ret.o:IIglobals.h
IIrubin.o:$H/defines.h $H/local.h
IIrubin.o:$H/symbol.h
IIrubin.o:$H/opsys.h
IIrubin.o:$H/aux.h
IIrubin.o:IIglobals.h
IIretrieve.o:$H/defines.h $H/local.h
IIretrieve.o:$H/symbol.h
IIretrieve.o:IIglobals.h
IIsetup.o:$H/defines.h $H/local.h
IIsetup.o:$H/symbol.h
IIsetup.o:IIglobals.h
IIflushtup.o:$H/defines.h $H/local.h
IIflushtup.o:$H/aux.h
IIflushtup.o:$H/symbol.h
IIflushtup.o:IIglobals.h
IIsync.o:$H/defines.h $H/local.h
IIsync.o:$H/symbol.h
IIsync.o:IIglobals.h
IIwrite.o:$H/defines.h $H/local.h
IIwrite.o:$H/symbol.h
IIwrite.o:IIglobals.h
IIexit.o:$H/defines.h $H/local.h
IIexit.o:$H/symbol.h
IIexit.o:IIglobals.h
IIconvert.o:$H/defines.h $H/local.h
IIconvert.o:$H/symbol.h
IIerror.o:$H/defines.h $H/local.h
IIerror.o:$H/symbol.h
IIerror.o:IIglobals.h
IIp_err.o:$H/defines.h $H/local.h
IIp_err.o:$H/aux.h
IIp_err.o:$H/version.h
IIp_err.o:IIglobals.h
IIgetc.o:$H/defines.h $H/local.h
IIgetc.o:IIglobals.h
IIresync.o:$H/defines.h $H/local.h
IIresync.o:IIglobals.h
IIreadinp.o:$H/useful.h
IIreadinp.o:$H/defines.h $H/local.h
IIreadinp.o:$H/aux.h
IIreadinp.o:IIglobals.h
IIpb_get.o:$H/useful.h
IIpb_put.o:$H/useful.h
IIpb_read.o:$H/defines.h $H/local.h
IIpb_read.o:IIglobals.h
IIpb_write.o:$H/defines.h $H/local.h
IIpb_write.o:IIglobals.h
IIpb_prime.o:$H/symbol.h
form_date.o:$H/defines.h $H/local.h
form_date.o:$H/datetime.h
form_date.o:$H/aux.h
form_date.o:../iutil/form_defs.h
form_conv.o:$H/defines.h $H/local.h
form_conv.o:$H/datetime.h
form_conv.o:../iutil/form_defs.h
form_in.o:$H/defines.h $H/local.h
form_in.o:$H/datetime.h
form_in.o:../iutil/form_defs.h
form_out.o:$H/defines.h $H/local.h
form_out.o:$H/datetime.h
form_out.o:../iutil/form_defs.h
form_compl.o:$H/defines.h $H/local.h
form_compl.o:$H/datetime.h
form_compl.o:../iutil/form_defs.h
sysdati.o:$H/defines.h $H/local.h
sysdati.o:../iutil/form_defs.h
sysdati.o:$H/datetime.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above

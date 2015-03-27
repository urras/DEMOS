#
#  Makefile for RUBIN iutil library
#	(includes old access library)
#
#	@(#)Makefile	7.3	9/26/83
#

RUBIN=	../..
H=	../h
LIB=	$(RUBIN)/lib

OBJS=	acc_addbuf.o add_prim.o batch.o batchxtra.o cleanrel.o \
	closecat.o closer.o clr_tuple.o delete.o do_u_flag.o \
	errfilen.o formatpg.o getnuser.o gettree.o getuser.o \
	initucode.o dblocks.o inpcloser.o insert.o markopen.o \
	noclose.o null_fn.o opencat.o cat_desc.o openr.o \
	get_reltup.o getequal.o get.o printdesc.o \
	replace.o del_tuple.o findbest.o add_ovflo.o cslocks.o \
	find.o last_page.o ndxsearch.o compare.o newlino.o \
	paramd.o printup.o out_arg.o put_tuple.o rhash.o \
	rllocks.o rub.o scan_dups.o get_tuple.o setkey.o \
	trim_relna.o tup_len.o writebatch.o accbuf.o page.o \
	pglocks.o globals.o readadmin.o checkadmin.o mkzpage.o\
	rubinname.o utility.o acc_init.o lock.o getservent.o\
	sysdati.o serial.o\
	form_conv.o form_compl.o form_in.o form_out.o form_date.o
# setjmp.o

# SRCS are split because make only allows 40 objects on a lhs.
SRCS1=	acc_addbuf.c add_prim.c batch.c batchxtra.c cleanrel.c \
	closecat.c closer.c clr_tuple.c delete.c do_u_flag.c \
	errfilen.c formatpg.c getnuser.c gettree.c getuser.c \
	initucode.c dblocks.c inpcloser.c insert.c markopen.c \
	noclose.c null_fn.c opencat.c cat_desc.c openr.c \
	acc_init.c get_reltup.c getequal.c get.c printdesc.c serial.c
SRCS2=	replace.c del_tuple.c findbest.c add_ovflo.c cslocks.c \
	find.c last_page.c ndxsearch.c compare.c newlino.c \
	paramd.c printup.c out_arg.c put_tuple.c rhash.c \
	rllocks.c rub.c scan_dups.c get_tuple.c setkey.c \
	trim_relna.c tup_len.c writebatch.c accbuf.c page.c \
	pglocks.c globals.c readadmin.c checkadmin.c mkzpage.c\
	rubinname.c utility.c lock.c getservent.c \
	sysdati.c\
	form_conv.c form_compl.c form_in.c form_out.c form_date.c
# setjmp.s

SRCS=	$(SRCS1) $(SRCS2)

# UTEC определен в defines.h
CFLAGS= -I$H -O
AR=	-ar
ARFLAGS=urv
RANLIB= ranlib
GET=	sccs get
REL=
PR=	print
VGRIND=	csh /usr/ucb/vgrind
HEADER=	Iutil Library
CP=     cp
CHMOD=	chmod
LIBMODE=664

# Из-за отсутствия RANLIB сунули здесь accbuf явно
#iutil: $(OBJS)
#        $(AR) $(ARFLAGS) iutil accbuf.o `lorder $?|tsort`
#        $(CHMOD) $(LIBMODE) iutil
#        ls -l iutil

iutil: $(OBJS)
	$(AR) $(ARFLAGS) iutil `lorder $?|tsort`
	$(CHMOD) $(LIBMODE) iutil
	$(RANLIB) iutil

install: $(LIB)/iutil

$(LIB)/iutil: iutil
	rm -f $(LIB)/iutil
	$(CP) iutil $(LIB)/iutil
	$(RANLIB) $(LIB)/iutil

new:
	rm -f iutil

clean:
	rm -f *.o
	rm -f iutil a.out core

print: sources
	$(PR) Makefile TraceFlags *.[hm] *.[csy]

vgrind: sources
	cp /dev/null index
	$(VGRIND) -h "$(HEADER) (Release 7)" -n Makefile TraceFlags
	$(VGRIND) -h "$(HEADER) (Release 7)" *.[csy]
	$(VGRIND) -h "$(HEADER) (Release 7)" -x index

sources: $(SRCS)

$(SRCS1):
	$(GET) $(REL) SCCS/s.$@
$(SRCS2):
	$(GET) $(REL) SCCS/s.$@

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

acc_addbuf.o:$H/defines.h $H/local.h
acc_addbuf.o:$H/access.h
add_prim.o:$H/defines.h $H/local.h
add_prim.o:$H/access.h
batch.o:$H/defines.h $H/local.h
batch.o:$H/aux.h
batch.o:$H/catalog.h
batch.o:$H/symbol.h
batch.o:$H/access.h
batchxtra.o:$H/defines.h $H/local.h
batchxtra.o:$H/aux.h
batchxtra.o:$H/symbol.h
batchxtra.o:$H/access.h
cleanrel.o:$H/defines.h $H/local.h
cleanrel.o:$H/aux.h
cleanrel.o:$H/access.h
closecat.o:$H/defines.h $H/local.h
closecat.o:$H/aux.h
closecat.o:$H/access.h
closer.o:$H/defines.h $H/local.h
closer.o:$H/access.h
clr_tuple.o:$H/defines.h $H/local.h
clr_tuple.o:$H/symbol.h
clr_tuple.o:$H/access.h
delete.o:$H/defines.h $H/local.h
delete.o:$H/access.h
do_u_flag.o:$H/defines.h $H/local.h
do_u_flag.o:$H/aux.h
errfilen.o:$H/defines.h $H/local.h
errfilen.o:$H/aux.h
errfilen.o:$H/version.h
formatpg.o:$H/defines.h $H/local.h
formatpg.o:$H/access.h
formatpg.o:$H/aux.h
getnuser.o:$H/defines.h $H/local.h
getnuser.o:$H/aux.h
gettree.o:$H/defines.h $H/local.h
gettree.o:$H/catalog.h
gettree.o:$H/tree.h
gettree.o:$H/symbol.h
getuser.o:$H/defines.h $H/local.h
getuser.o:$H/aux.h
initucode.o:$H/defines.h $H/local.h
initucode.o:$H/aux.h
initucode.o:$H/version.h
initucode.o:$H/opsys.h
initucode.o:$H/access.h
initucode.o:$H/lock.h
dblocks.o:$H/defines.h $H/local.h
dblocks.o:$H/lock.h
inpcloser.o:$H/defines.h $H/local.h
inpcloser.o:$H/access.h
insert.o:$H/defines.h $H/local.h
insert.o:$H/access.h
markopen.o:$H/useful.h
markopen.o:$H/opsys.h
noclose.o:$H/defines.h $H/local.h
noclose.o:$H/aux.h
noclose.o:$H/access.h
opencat.o:$H/defines.h $H/local.h
opencat.o:$H/aux.h
opencat.o:$H/opsys.h
opencat.o:$H/access.h
cat_desc.o:$H/defines.h $H/local.h
cat_desc.o:$H/aux.h
cat_desc.o:$H/access.h
openr.o:$H/defines.h $H/local.h
openr.o:$H/aux.h
openr.o:$H/access.h
openr.o:$H/symbol.h
openr.o:$H/code.h
get_reltup.o:$H/defines.h $H/local.h
get_reltup.o:$H/access.h
get_reltup.o:$H/aux.h
getequal.o:$H/defines.h $H/local.h
getequal.o:$H/access.h
get.o:$H/defines.h $H/local.h
get.o:$H/aux.h
get.o:$H/access.h
printdesc.o:$H/defines.h $H/local.h
serial.o:$H/defines.h $H/local.h
serial.o:$H/aux.h
serial.o:$H/access.h
serial.o:$H/code.h
replace.o:$H/defines.h $H/local.h
replace.o:$H/access.h
del_tuple.o:$H/defines.h $H/local.h
del_tuple.o:$H/access.h
findbest.o:$H/defines.h $H/local.h
findbest.o:$H/aux.h
findbest.o:$H/access.h
add_ovflo.o:$H/defines.h $H/local.h
add_ovflo.o:$H/access.h
add_ovflo.o:$H/aux.h
add_ovflo.o:$H/lock.h
cslocks.o:$H/defines.h $H/local.h
cslocks.o:$H/access.h
cslocks.o:$H/lock.h
find.o:$H/defines.h $H/local.h
find.o:$H/aux.h
find.o:$H/symbol.h
find.o:$H/access.h
find.o:$H/lock.h
last_page.o:$H/defines.h $H/local.h
last_page.o:$H/access.h
last_page.o:$H/opsys.h
ndxsearch.o:$H/defines.h $H/local.h
ndxsearch.o:$H/aux.h
ndxsearch.o:$H/symbol.h
ndxsearch.o:$H/access.h
ndxsearch.o:$H/lock.h
compare.o:$H/defines.h $H/local.h
compare.o:$H/access.h
compare.o:$H/symbol.h
compare.o:$H/aux.h
newlino.o:$H/defines.h $H/local.h
newlino.o:$H/access.h
paramd.o:$H/defines.h $H/local.h
paramd.o:$H/aux.h
paramd.o:$H/catalog.h
paramd.o:$H/access.h
printup.o:$H/defines.h $H/local.h
printup.o:$H/aux.h
printup.o:$H/access.h
printup.o:$H/symbol.h
printup.o:$H/datetime.h
out_arg.o:$H/defines.h $H/local.h
out_arg.o:$H/aux.h
put_tuple.o:$H/defines.h $H/local.h
put_tuple.o:$H/symbol.h
put_tuple.o:$H/access.h
rhash.o:$H/defines.h $H/local.h
rhash.o:$H/aux.h
rhash.o:$H/symbol.h
rhash.o:$H/access.h
rhash.o:$H/lock.h
rllocks.o:$H/defines.h $H/local.h
rllocks.o:$H/lock.h
rub.o:$H/useful.h
scan_dups.o:$H/defines.h $H/local.h
scan_dups.o:$H/symbol.h
scan_dups.o:$H/access.h
scan_dups.o:$H/lock.h
get_tuple.o:$H/defines.h $H/local.h
get_tuple.o:$H/access.h
get_tuple.o:$H/symbol.h
setkey.o:$H/defines.h $H/local.h
setkey.o:$H/symbol.h
trim_relna.o:$H/defines.h $H/local.h
tup_len.o:$H/defines.h $H/local.h
tup_len.o:$H/access.h
writebatch.o:$H/defines.h $H/local.h
writebatch.o:$H/aux.h
writebatch.o:$H/symbol.h
writebatch.o:$H/access.h
accbuf.o:$H/defines.h $H/local.h
accbuf.o:$H/access.h
accbuf.o:$H/aux.h
accbuf.o:$H/lock.h
accbuf.o:$H/opsys.h
page.o:$H/defines.h $H/local.h
page.o:$H/access.h
page.o:$H/aux.h
page.o:$H/lock.h
pglocks.o:$H/defines.h $H/local.h
pglocks.o:$H/access.h
pglocks.o:$H/lock.h
pglocks.o:$H/opsys.h
readadmin.o:$H/defines.h $H/local.h
readadmin.o:$H/access.h
readadmin.o:$H/aux.h
readadmin.o:$H/lock.h
checkadmin.o:$H/defines.h $H/local.h
checkadmin.o:$H/version.h
checkadmin.o:$H/access.h
mkzpage.o:$H/defines.h $H/local.h
mkzpage.o:$H/access.h
rubinname.o:$H/defines.h $H/local.h
rubinname.o:$H/access.h
rubinname.o:$H/code.h
utility.o:$H/defines.h $H/local.h
utility.o:$H/access.h
lock.o:$H/lock.h
sysdati.o:$H/defines.h $H/local.h
sysdati.o:form_defs.h
form_conv.o:$H/defines.h $H/local.h
form_conv.o:$H/datetime.h
form_conv.o:form_defs.h
form_compl.o:$H/defines.h $H/local.h
form_compl.o:$H/datetime.h
form_compl.o:form_defs.h
form_in.o:$H/defines.h $H/local.h
form_in.o:$H/datetime.h
form_in.o:form_defs.h
form_out.o:$H/defines.h $H/local.h
form_out.o:$H/datetime.h
form_out.o:form_defs.h
form_date.o:$H/defines.h $H/local.h
form_date.o:$H/datetime.h
form_date.o:$H/aux.h
form_date.o:form_defs.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above

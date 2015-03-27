# BIG
# DEMOS
# MNOS                  macros
# PROPORTIONAL
# RUSMAC
# SOAN

SHELL   =       /bin/sh
CFLAGS  =       -O -DSOAN -DINCH=240 -DASA
OBJS    =       n1.o n2.o n3.o n4.o n5.o n6.o n7.o n8.o n9.o n10.o \
		ni.o nii.o ntab.o hytab.o suftab.o

SRCS    =       n1.c n2.c n3.c n4.c n5.c n6.c n7.c n8.c n9.c n10.c \
		ni.c nii.c ntab.c hytab.c suftab.c

HDRS    =       tdef.h d.h v.h tw.h s.h

nroff:          $(OBJS)
		cc -o $@ $(CFLAGS) $(OBJS)

ovnroff:        $(OBJS)
		cc -o $@ $(CFLAGS) -Q $(OBJS)

$(OBJS):        $(HDRS)

hytab.o:        hytab.c
		cc -S hytab.c
		ed hytab.s <textscript
		as -o hytab.o hytab.s
		rm hytab.s

suftab.o:       suftab.c
		cc -S suftab.c
		ed suftab.s <textscript
		as -o suftab.o suftab.s
		rm suftab.s

print:;         lpr -Pfl $(HDRS) $(SRCS)

clean:;         rm -f $(OBJS)

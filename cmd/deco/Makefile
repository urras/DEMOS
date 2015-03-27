#
#       Demos commander, Copyright (C) 1989, 1990 Demos/*, Moscow.
#

#       -DTERMIO        use termio.h instead sgtty.h
#       -DNOKEYPAD      don't use keypad
#       -DINITFILE      send 'if' file to initialize terminal
#       -DGETWD         use standard getwd () (BSD)
#       -DGETCWD        use standard getcwd () (System V)
#       -DDIRENT        use file system independent directory format
#       -DSTDDIR        use standard readdir library
#       -DSTRINIT       send is, fs when VRestore, VReopen
#       -DHOSTNAME      use gethostname
#       -DHARDKEYS      cannot differ key escape codes by two last chars
#       -DBCOPY         use bcopy, bcmp instead of memcpy, memcmp
#       -DLONGPTR       sizeof (char *) == sizeof (long)

DEST          = /usr/local/bin
LINKER        = $(CC)
MAKEFILE      = Makefile
PRINT	      = pr
PROGRAM       = deco

# For Labtam NS 32332
# CFLAGS        = -O -DGETCWD -DTERMIO -DSTRINIT
# LDFLAGS       = -i

# For DEMOS 2.2
CFLAGS        = -O -DGETWD
LDFLAGS       = -i -Q
CC            = cc

# For Xenix-286
# CFLAGS        = -O -K -M2l -DTERMIO -DLONGPTR
# LDFLAGS       = -i -M2l

# For Xenix-386
# CFLAGS        = -O -K -DTERMIO
# LDFLAGS       = -i

# For Xenix-386 with gnu compiler
# CFLAGS        = -O -g -DTERMIO
# LDFLAGS       = -g
# CC            = gcc
# LINKER        = gcc

# For IX-386 with standard compiler
# CFLAGS        = -O -DTERMIO -DDIRENT
# LDFLAGS       =

# For IX-386 with gnu compiler
# CFLAGS        = -O -g -DTERMIO -DDIRENT
# LDFLAGS       = -g
# CC            = gcc
# LINKER        = gcc

# For Bsd43 and Ultrix
# CFLAGS        = -O -DGETWD -DSTDDIR -DHOSTNAME -DBCOPY -DVFORK
# LDFLAGS       =

# For Bsd43 and Ultrix with gnu compiler
# CFLAGS        = -O -g -DGETWD -DSTDDIR -DHOSTNAME -DBCOPY -DVFORK
# LDFLAGS       = -g
# CC            = gcc
# LINKER        = gcc

HDRS          = cap.h\
		deco.h\
		dir.h\
		env.h\
		key.h\
		mem.h\
		menu.h\
		rec.h\
		scr.h\
		tm.h

OBJSOV        = cap.o\
		choice.o\
		com.o\
		dir.o\
		edit.o\
		env.o\
		ex.o\
		getwd.o\
		help.o\
		hexview.o\
		key.o\
		mem.o\
		pw.o\
		rec.o\
		run.o\
		time.o\
		tm.o\
		tty.o\
		ut.o\
		view.o

OBJSBAS =       mh.o cmd.o scr.o draw.o main.o match.o menu.o mem.o

SRCS          = cap.c\
		choice.c\
		cmd.c\
		com.c\
		dir.c\
		draw.c\
		edit.c\
		env.c\
		ex.c\
		getwd.c\
		help.c\
		hexview.c\
		key.c\
		main.c\
		match.c\
		mem.c\
		menu.c\
		mh.c\
		pw.c\
		rec.c\
		run.c\
		scr.c\
		time.c\
		tm.c\
		tty.c\
		ut.c\
		view.c

all:		$(PROGRAM)

$(PROGRAM):     $(OBJSOV) $(OBJSBAS) $(LIBS)
		$(LINKER) $(LDFLAGS) $(OBJSOV) -L $(OBJSBAS) $(LIBS) -o $(PROGRAM)

clean:;         rm -f *.o *.b z* core a.out deco

depend:;        mkmf -f $(MAKEFILE) PROGRAM=$(PROGRAM) DEST=$(DEST)

index:;		@ctags -wx $(HDRS) $(SRCS)

install:	$(PROGRAM)
		cp $(PROGRAM) $(DEST)
		strip $(DEST)/$(PROGRAM)
		chmod 711 $(DEST)/$(PROGRAM)
		chgrp bin $(DEST)/$(PROGRAM)
		chown bin $(DEST)/$(PROGRAM)

print:;		@$(PRINT) $(HDRS) $(SRCS)

program:        $(PROGRAM)

tags:           $(HDRS) $(SRCS); @ctags $(HDRS) $(SRCS)

lint:           $(HDRS) $(SRCS); @lint $(SRCS) >lint

update:		$(DEST)/$(PROGRAM)

$(DEST)/$(PROGRAM): $(SRCS) $(LIBS) $(HDRS) $(EXTHDRS)
		make -f $(MAKEFILE) DEST=$(DEST) install
###
cap.o: cap.h mem.h env.h
choice.o: scr.h dir.h deco.h
cmd.o: dir.h deco.h scr.h mem.h env.h
com.o: scr.h dir.h deco.h mem.h env.h
dir.o: dir.h deco.h mem.h
draw.o: dir.h deco.h scr.h
edit.o: scr.h rec.h mem.h
env.o: mem.h
ex.o: dir.h deco.h scr.h mem.h
help.o: scr.h env.h
hexview.o: scr.h rec.h mem.h
key.o: key.h cap.h
main.o: key.h scr.h dir.h deco.h mem.h env.h
mem.o: mem.h
menu.o: scr.h menu.h
mh.o: scr.h dir.h deco.h menu.h
pw.o: mem.h
rec.o: rec.h mem.h
run.o: mem.h env.h
scr.o: scr.h key.h mem.h env.h cap.h
time.o: tm.h
tm.o: tm.h env.h
view.o: scr.h rec.h

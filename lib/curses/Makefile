DEST	      = /usr/lib

CFLAGS        = -O

EXTHDRS	      = /usr/include/ctype.h \
		/usr/include/sgtty.h \
		/usr/include/signal.h \
		/usr/include/stdio.h \
		/usr/include/sys/code.h \
		/usr/include/sys/ioctl.h \
		/usr/include/sys/localopts.h

HDRS	      = curses.h \
		unctrl.h

LIBRARY	      = libcurses.a

MAKEFILE      = Makefile

OBJS	      = addch.o \
		addstr.o \
		box.o \
		clear.o \
		clrtobot.o \
		clrtoeol.o \
		cr_put.o \
		cr_tty.o \
		curses.o \
		delch.o \
		deleteln.o \
		delwin.o \
		endwin.o \
		erase.o \
		getch.o \
		getstr.o \
		initscr.o \
		insch.o \
		insertln.o \
		longname.o \
		move.o \
		mvprintw.o \
		mvscanw.o \
		mvwin.o \
		newwin.o \
		overlay.o \
		overwrite.o \
		printw.o \
		refresh.o \
		scanw.o \
		scroll.o \
		standout.o \
		touchwin.o \
		tstp.o \
		unctrl.o

PRINT         = print

SRCS	      = addch.c \
		addstr.c \
		box.c \
		clear.c \
		clrtobot.c \
		clrtoeol.c \
		cr_put.c \
		cr_tty.c \
		curses.c \
		delch.c \
		deleteln.c \
		delwin.c \
		endwin.c \
		erase.c \
		getch.c \
		getstr.c \
		initscr.c \
		insch.c \
		insertln.c \
		longname.c \
		move.c \
		mvprintw.c \
		mvscanw.c \
		mvwin.c \
		newwin.c \
		overlay.c \
		overwrite.c \
		printw.c \
		refresh.c \
		scanw.c \
		scroll.c \
		standout.c \
		touchwin.c \
		tstp.c \
		unctrl.c

all:		$(LIBRARY)

$(LIBRARY):	$(OBJS)
		@echo -n "Loading $(LIBRARY) ... "
		@ar cru $(LIBRARY) $(OBJS)
		@ranlib $(LIBRARY)
		@echo "done"

clean:;         @rm -f $(OBJS) lint.out a.out

depend:;	@mkmf -f $(MAKEFILE) LIBRARY=$(LIBRARY) DEST=$(DEST)

extract:;	@ar xo $(DEST)/$(LIBRARY)
		@rm -f __.SYMDEF

lint:;          @lint -hxb ${SRCS} llib-lcurses > lint.out

index:;		@ctags -wx $(HDRS) $(SRCS)

install:	$(LIBRARY)
		@echo Installing $(LIBRARY) in $(DEST)
		@install $(LIBRARY) $(DEST)
		@ranlib $(DEST)/$(LIBRARY)

library:        $(LIBRARY)

print:;		@$(PRINT) $(HDRS) $(SRCS)

update:         $(DEST)/$(LIBRARY)

$(DEST)/$(LIBRARY): $(SRCS) $(HDRS) $(EXTHDRS)
		@-ar xo $(DEST)/$(LIBRARY)
		@make -f $(MAKEFILE) DEST=$(DEST) install clean
###
addch.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
addstr.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
box.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
clear.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
clrtobot.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
clrtoeol.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
cr_put.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
cr_tty.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
curses.o: curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h
delch.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
deleteln.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
delwin.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
endwin.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
erase.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
getch.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
getstr.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
initscr.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h /usr/include/signal.h
insch.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
insertln.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
move.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
mvprintw.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
mvscanw.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
mvwin.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
newwin.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
overlay.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h /usr/include/ctype.h
overwrite.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
printw.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
refresh.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h /usr/include/ctype.h
scanw.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
scroll.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
standout.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
test.o: curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h
touchwin.o: curses.ext curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h \
	/usr/include/sys/localopts.h unctrl.h
tstp.o: /usr/include/signal.h /usr/include/sys/localopts.h curses.ext \
	curses.h /usr/include/stdio.h /usr/include/sgtty.h \
	/usr/include/sys/code.h /usr/include/sys/ioctl.h unctrl.h

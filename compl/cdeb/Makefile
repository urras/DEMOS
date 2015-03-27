################## ðïûáçï÷ùê ïôìáäþéë äìñ C-ðòïçòáíí ######################
#                                                                         #
#                      ###    ###     #####   ####                        #
#                     #   #   #  #    #       #   #                       #
#                     #       #   #   ###     ####                        #
#                     #       #   #   #       #   #                       #
#                     #   #   #  #    #       #   #                       #
#                      ###    ###     #####   ####                        #
#                                                                         #
###########################################################################
# 19.02.86, òÕÄÎÅ× á.ð. -
# æÌÁÇÉ ÄÌÑ ü-79
CFLAGS  = -O -DSEPID
LDFLAGS = -n
#
#CFLAGS  = -O
#LDFLAGS = -n -f
NAME    = cdeb

OBJS=   cdeb.o      line.o      indsignal.o dpcdpr.o    cdump.o     \
        getword.o   getcmnd.o   conv.o      symbols.o   indchar.o   \
        convi10.o   indline.o   indvalue.o  indstack.o  linenumb.o

#####
$(NAME): $(OBJS) Makefile
        $(CC) $(LDFLAGS) $(OBJS) -o $(NAME)

cdeb.o getword.o symbols.o indvalue.o linenumb.o : cdeb.h
symbols.o indvalue.o                             : f77types.h


########
install: /bin/$(NAME)           /lib/libc.a(cdebug.o) \
         /usr/minihelp/$(NAME)

/usr/bin/$(NAME): $(NAME)
        strip $(NAME)
        cp    $(NAME) /usr/bin/$(NAME)
        chmod 755     /usr/bin/$(NAME)

/usr/lib/libc.a(cdebug.o): cdebug.s
        as             cdebug.s  -o cdebug.o
        ld -r -x                    cdebug.o
        mv a.out                    cdebug.o
        ar r           /usr/lib/libc.a  cdebug.o
        ranlib         /usr/lib/libc.a

/usr/help/$(NAME).hlp: help
        -rm -f              /usr/help/$(NAME).hlp
        cp             help /usr/help/$(NAME).hlp
        addhelp

/usr/lib/minihelp/$(NAME): minihelp
        -mkdir                  /usr/lib/minihelp
        cp             minihelp /usr/lib/minihelp/$(NAME)
        chmod 644               /usr/lib/minihelp/$(NAME)

######
clear:
        -rm -f *.[ob]
        -rm -f $(NAME)

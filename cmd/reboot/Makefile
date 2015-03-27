###########################################################################
#                                                                         #
#                  ****                                                   *
#                 *    * *             *      *                           *
#                 *      *     *   *  ***     *  ***  *   * * **          *
#                  ****  ****  *   *   *   **** *   * *   * **  *         *
#                      * *   * *   *   *  *   * *   * * * * *   *         *
#                 *    * *   * *   *   *  *   * *   * * * * *   *         *
#                  ****  *   *  *** *   *  ****  ***   * *  *   *         *
#                                                                         #
###########################################################################

LDFLAGS       = -n
CFLAGS        = -O
OBJS          = reboot.o \
                shutdown.o
SRCS          = reboot.c \
                shutdown.c
all:            reboot shutdown
reboot: reboot.o sysrboot.o
        cc -o reboot $(LDFLAGS) reboot.o sysrboot.o
shutdown: shutdown.o
        cc -o shutdown $(LDFLAGS) shutdown.o
###
reboot.o: /usr/include/stdio.h /usr/include/sys/reboot.h \
        /usr/include/errno.h /usr/include/signal.h /usr/include/sys/types.h \
        /usr/include/utmp.h /usr/include/ediag.h
shutdown.o: /usr/include/stdio.h /usr/include/ctype.h /usr/include/signal.h \
        /usr/include/utmp.h /usr/include/sys/types.h

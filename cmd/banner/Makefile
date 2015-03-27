######################################################################
#                                                                    #
#              *                                                     #
#              *      ****  * **  * **   ***  * **                   #
#              ****  *   *  **  * **  * *   * **  *                  #
#              *   * *   *  *   * *   * ****  *                      #
#              *   * *   *  *   * *   * *     *                      #
#              ****   *** * *   * *   *  ***  *                      #
#                                                                    #
#                            νξοσ ςμ 1.2                             #
#                                                                    #
#             ζοςνιςοχατεμψ ϊαηομοχλοχ βομψϋινι βυλχανι              #
#                                                                    #
######################################################################

MM=../mm/mm
FONT=font
LDFLAGS=-s -n
CFLAGS= -O
DESTDIR=/bin

####
all:    banner

banner: banner.o $(FONT).o
        $(CC) $(LDFLAGS) -o banner banner.o $(FONT).o

banner.o: banner.c

$(FONT).c: $(FONT).m
        $(MM) - < $(FONT).m >$(FONT).c
$(FONT).o: $(FONT).c
        $(CC) -a -c $(FONT).c

########
install: banner
        -rm -f $(DESTDIR)/banner
        cp banner $(DESTDIR)/banner

######
clear:
        -rm -f *.[ob]
        -rm -f banner

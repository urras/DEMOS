#
# Utilities for object code management. zaitcev
#

#CC         = $(HOME)/d22/bin/cc
CC         = cc
CFLAGS     = -g

TARGETS    = c7t10 nm size strip ar
BINDIR     = $(HOME)/d22/bin

all:    $(TARGETS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c

c7t10:    c7t10.c
nm:       nm.c
size:     size.c
strip:    strip.c
ar:       ar.c

$(TARGETS):
	$(CC) -o $@ $(CFLAGS) $@.c

clear:
	rm -f $(TARGETS)

install:   $(TARGETS)
	cp $(TARGETS) $(BINDIR)

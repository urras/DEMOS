CC=cc286 -M2l -g -F 2000
CFLAGS=-O
LIBQ=../../lib/Llibq.a
EQUEL=equel
EFLAGS=

TESTS=demo tutorial test2_15 testdate equelmem
CFILES=equeldemo.c equeltut.c equel2_15.c testdate.c

all:    $(TESTS)

createb:
	-demodb demo
	sysmod demo
	-purge -f demo

demo:   equeldemo.o
	$(CC) -o demo equeldemo.o $(LIBQ)


tutorial:   equeltut.o
	$(CC) -o tutorial equeltut.o $(LIBQ)

testdate:   equeldate.o
	$(CC) -o testdate equeldate.o $(LIBQ)

test2_15:   equel2_15.o
	$(CC) -o test2_15 equel2_15.o $(LIBQ)

equelmem:   equelmem.o
	$(CC) -o equelmem equelmem.o $(LIBQ)

equeldemo.o: equeldemo.c
equeltut.o: equeltut.c
equeldate.o: equeldate.c
equel2_15.o: equel2_15.c
equelmem.o: equelmem.c

equeldemo.c: equeldemo.q
	$(EQUEL) $(EFLAGS) $?

equeltut.c: equeltut.q
	$(EQUEL) $(EFLAGS) $?

equeldate.c: equeldate.q
	$(EQUEL) $(EFLAGS) $?

equel2_15.c: equel2_15.q
	$(EQUEL) $(EFLAGS) $?

equelmem.c: equelmem.q
	$(EQUEL) $(EFLAGS) $?

clean:
	rm -f *.b *.o $(CFILES) $(TESTS)

DESTDIR =
CFLAGS  =       -O
SHELL   =       /bin/sh

OBJ     =       t0.o t1.o t2.o t3.o t4.o t5.o t6.o t7.o t8.o \
		t9.o tb.o tc.o te.o tf.o tg.o ti.o tm.o ts.o \
		tt.o tu.o tv.o

tbl:            $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

install:        tbl
	install -s tbl $(DESTDIR)/usr/bin/tbl

$(OBJ):         t..c

clean:
	rm -f *.o tbl

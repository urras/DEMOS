CFLAGS = -O
SEPID = -n
FP = -f

all:    adb

cmp:    adb
        cmp adb /bin/adb
        rm adb *.o

cp:     adb
        cp adb /bin/adb
        rm adb *.o

adb:    access.o command.o expr.o findfn.o
adb:    format.o input.o opset.o main.o
adb:    message.o output.o pcs.o print.o
adb:    runpcs.o setup.o sym.o
adb:;   cc -o adb -s $(SEPID) $(FP) *.o -lfpsim


access.o:       defs.h
command.o:      defs.h
expr.o:         defs.h
        cc -c $(CFLAGS) $(FP) expr.c
findrtn.o:      defs.h
format.o:       defs.h
        cc -c $(CFLAGS) $(FP) format.c
input.o:        defs.h
main.o:         defs.h
message.o:      mac.h mode.h
output.o:       defs.h
        cc -c $(CFLAGS) $(FP) output.c
pcs.o:          defs.h
print.o:        defs.h
        cc -c $(CFLAGS) $(FP) print.c
runpcs.o:       defs.h
setup.o:        defs.h
sym.o:          defs.h

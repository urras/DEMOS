
DWST    = 20
DW$CSR  = 174000+DWST
DW$VEC  = 300
DW$SEC  = 16.
DW$SRF  = 6.
PRESEL  = 260.
DWID    = 0-DWST
DWERR   = 4-DWST
DWPRE   = 4-DWST
DWSEC   = 6-DWST
DWBUF   = 10-DWST
DWCYL   = 12-DWST
DWHEAD  = 14-DWST
DWST2   = 16-DWST
DWCMD   = 16-DWST
DWSTAT  = 20-DWST
DWINIT  = 20-DWST
EDWM    = 0400
ERTR0   = 01000
ERABO   = 02000
ERIDNF  = 10000
ERICRC  = 20000
EDWCRC  = 40000
S2ERR   = 400
S2DRQ   = 4000
S2SEK   = 10000
S2WRF   = 20000
S2DWY   = 40000
CMREST  = 20
CMREAD  = 40
CMWRIT  = 60
CMFORM  = 120
STOPND  = 1
STDRQ   = 200
STDCAP  = 400
STBUSY  = 100000
STINIT  = 10
STINT   = 100

WREQ    = 100000
RDAWRT  =      1

esc = 33
cr  = 15
lf  = 12

.Macro Pushr
.irp reg,<r0,r1,r2,r3,r4,r5>
mov reg,-(6)
.endm ;irp
.Endm Pushr
.Macro Popr
.irp reg,<r5,r4,r3,r2,r1,r0>
mov (6)+,reg
.endm ;irp
.Endm Popr

.Macro waitit count,?l1,?l2
mov r0,-(6)
mov r1,-(6)
mov #4000.,r1
l1:
tst @r4
bpl l2
.iif nb count inc count
sob r0,l1
sob r1,l1
;.mtps #0
.print #cbsy
jmp exit
l2:
mov (6)+,r1
mov (6)+,r0
.endm waitit


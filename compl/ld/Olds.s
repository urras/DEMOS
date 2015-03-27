.globl _O_ldstart
_O_ldstart:
jsr pc,_O_load
tst (sp)+
jmp (r0)
.globl _O_retn
_O_retn:
inc _O_pretn
inc _O_pretn
mov *_O_pretn,-(sp)
jmp *(sp)+
.data
.globl _O_pretn
_O_pretn:
.=.+2

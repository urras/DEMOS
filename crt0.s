/       $Header: crt0.s,V 1.3 86/07/22 19:16:10 avg Exp $
/
/       Стартовый модуль Си.
/
/       $Log:	crt0.s,V $
/     Revision 1.3  86/07/22  19:16:10  avg
/     Выброшена возможность исп. в MSG русских букв, т.к.
/     это зависит от кодировки.
/     Кроме того, буква в MSG преобразуется в маленькую -
/     for the sake of compatibility.
/     
/     Revision 1.2  86/05/17  09:42:08  dmitry
/     Русский режим устанавливается по любому слову,
/     начинающемуся с любой из 4 букв:
/       r       R       р       Р
/                       (русские)
/
/     Revision 1.1  86/04/20  19:05:13  dmitry
/     Вставлена обработка MSG.
/

.globl  _exit
.globl  _main
.globl  _environ
.globl  __ediag

start:
    setd
    mov     2(sp),r0
    clr     -2(r0)
    mov     sp,r0
    sub     $4,sp
    mov     4(sp),(sp)
    tst     (r0)+
    mov     r0,2(sp)
1:
    tst     (r0)+
    bne     1b
    cmp     r0,*2(sp)
    blo     1f
    tst     -(r0)
1:
    mov     r0,4(sp)
    mov     r0,_environ
2:
    mov     (r0)+,r2
    beq     2f
    mov     $msg,r3
    mov     $4,r4
1:
    cmpb    (r2)+,(r3)+
    bne     2b
    sob     r4,1b

    bisb    $40,(r2)
    cmpb    (r2),$'r
    bne     2f
    clr     __ediag
2:
    jsr     pc,_main
    cmp     (sp)+,(sp)+
    mov     r0,(sp)
    jsr     pc,*$_exit
    sys     exit
.bss
_environ:
    .=.+2
.data
    .=.+2
__ediag:        1
msg:            <MSG=>

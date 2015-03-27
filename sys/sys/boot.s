#include "h/whoami.h"
#include "../include/iopage.m"

/  The boot options and device are placed in the last SZFLAGS bytes
/  at the end of core for the bootstrap.
ENDCORE=        160000          / end of core, mem. management off
SZFLAGS=        6               / size of boot flags
BOOTOPTS=       2               / location of options, bytes below ENDCORE
BOOTDEV=        4
CHECKWORD=      6

reset= 5

.globl  _doboot
.text
_doboot:
	mov     6(sp),r1        / boot block math. address
	mov     4(sp),r4        / boot options
	mov     2(sp),r3        / boot device

/ Установить лист KDSA0 на адрес 0, разрешить R/W
	clr     KDSA0
	mov     $[177\<8]|RW,KDSD0

/ Скопировать 256 слов из KDSA5 в физ. адрес 0
	mov     $256.,r0
	clr     r5
1:      mov     (r1)+,(r5)+
	sob     r0,1b

#ifdef  KERN_NONSEP
	clr     r1

#ifndef DVK4
	tst     8(sp)           / if true, it is Dec Pro bootstrap...
	beq     2f

/ Для Э-85: выключить ДП, выключить все прерывания
	mov     $340, *$177776  / God Save It
	clr     *$177572
	mov     $60, r0         / Установить все биты РЗП
	mov     $100, r1        / Очистка всех разрядов РЗО
	mov     r0, *$173202
	mov     r1, *$173202
	mov     r0, *$173206
	mov     r1, *$173206
	mov     r0, *$173212
	mov     r1, *$173212
	mov     $20, r1         / стартовая точка - 020
	br      3f
#endif DVK4

2:
	reset                   / Turn off mem mgt & ub map

/  On power fail, hardboot is the entry point (map is already off)
/  and the args are in r4, r3.

3:
	.globl  hardboot
hardboot:

	mov     r4, ENDCORE-BOOTOPTS
	mov     r3, ENDCORE-BOOTDEV
	com     r4              / if CHECKWORD == ~bootopts, flags are believed
	mov     r4, ENDCORE-CHECKWORD

	jmp     (r1)            / переход на начало

#else   KERN_NONSEP

	.globl  hardboot
hardboot:
	mov     $rst+2,r0
	mov     $010437,(r0)+           / mov r4,$*
	mov     $ENDCORE-BOOTOPTS,(r0)+ /          ENDCORE-BOOTOPTS
	mov     $010337,(r0)+           / mov r3,$*
	mov     $ENDCORE-BOOTDEV,(r0)+  /          ENDCORE-BOOTDEV
	mov     $005104,(r0)+           / com r4
	mov     $010437,(r0)+           / mov r4,$*
	mov     $ENDCORE-CHECKWORD,(r0)+ /         ENDCORE-CHECKWORD
	mov     $000112,(r0)+           / jmp (r2)
	mov     r3, r0
	ash     $-3,r0
	bic     $!07,r0
	mov     $0172150,r1
	clr     r2
rst:
	reset
#endif  KERN_NONSEP

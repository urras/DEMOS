/*
 *	Low Core
 *
 * $Log:	l.s,v $
 * Revision 1.9  90/12/12  17:05:32  korotaev
 * Правки под ДВК.
 * 
 * Revision 1.8  90/11/12  18:52:28  root
 * Новые вещи для СМ 1425 и перемещение некоторых include'ов.
 * 
 * Revision 1.7  88/08/30  13:42:19  korotaev
 * Введена обработка конфликтных векторов. 
 * Вектора всех загрузочных дисков инициируются по мажору
 * полученному из boot'а.
 * 
 * Revision 1.6  88/06/23  21:23:47  ache
 * + 31 оверлей в ядре
 * 
 * Revision 1.5  88/04/21  21:44:20  nms
 * Освобожден ложно занятый вектор 120
 * 
 * Revision 1.4  88/03/23  14:13:12  korotaev
 * Состояние после слияния с АЗЛК, Э-85 и Бурковским планировщиком
 * 
 * Revision 1.3  88/01/04  20:39:24  korotaev
 * Слияние версий с АЗЛК.
 * 
 * Revision 1.2  86/11/23  22:48:25  alex
 * Добавлен dj
 * 
 * Revision 1.1  86/04/19  15:02:25  avg
 * Initial revision
 * 
 */

#define		LOCORE
#include        "h/whoami.h"              /* for localopts */
#include	"../include/trap.h"
#include	"../include/iopage.m"
#include	"../include/koverlay.h"	/* for definition of OVLY_TABLE_BASE */

#include "h/dh.h"
#include "h/dhu.h"
#include "h/dj.h"
#include "h/dn.h"
#include "h/dw.h"
#include "h/dz.h"
#include "h/fd.h"
#include "h/hk.h"
#include "h/hp.h"
#include "h/hs.h"
#include "h/ht.h"
#include "h/kl.h"
#include "h/ks.h"
#include "h/kx.h"
#include "h/ky.h"
#include "h/lp.h"
#include "h/mk.h"
#include "h/my.h"
#include "h/pc.h"
#include "h/ra.h"
#include "h/rk.h"
#include "h/rl.h"
#include "h/rm.h"
#include "h/rp.h"
#include "h/rw.h"
#include "h/rx.h"
#include "h/tm.h"
#include "h/ts.h"
#include "h/video.h"
#include "h/wl.h"
#include "h/xp.h"

	.globl	call, trap, buserr, start, _panic
#ifdef	NONFP
	.globl	instrap
#else
#	define	instrap		trap
#endif
	.globl	emt
	.globl	powrdown

#ifndef	KERN_NONSEP
	.data
#endif

ZERO:

br4 = 200
br5 = 240
br6 = 300
br7 = 340

. = ZERO+0
#ifdef	KERN_NONSEP
	/  If vectors at 110 and 444 are unused,
	/  autoconfig will set these to something more reasonable.
	/  On jump, this will branch to 112, which branches to 50.
	/  On trap, will vector to 444, where a ZEROTRAP will be simulated.
	42				/ illegal instruction if jump
	777				/ trace trap at high priority if trap
#else
	trap; br7+ZEROTRAP.		/ trap-to-zero trap
#endif

/ trap vectors
. = ZERO+4
	buserr; br7+BUSFLT.		/ bus error
	instrap; br7+INSTRAP.		/ illegal instruction
	trap; br7+BPTTRAP.		/ bpt-trace trap
	trap; br7+IOTTRAP.		/ iot trap
	powrdown; br7+POWRFAIL.		/ power fail
	emt; br7+EMTTRAP.		/ emulator trap
	start;br7+SYSCALL.		/ system  (overlaid by 'syscall')

. = ZERO+40
.globl	do_panic
	jmp	do_panic

.globl	 dump
. = ZERO+44
	jmp	dump
#ifdef	KERN_NONSEP
	/  Handler for jump-to-zero panic.
. = ZERO+50
	mov	$zjmp, -(sp)
	jsr	pc, _panic
#endif

#if     NKL > 0
. = ZERO+60
	klin; br5
	klou; br4
#endif

. = ZERO+100
#if PDP11 == 23
	kwlp23; br7                     / Тест на включение часов ДВК-4
#else
	kwlp; br6
#endif
	kwlp; br6

. = ZERO+114
	trap; br7+PARITYFLT.            / 11/70 parity (and other CPUs also)

#if     NMY > 0
. = ZERO+170
	myio; br5
#endif

#if     NVI > 0
. = ZERO+200
	viin; br4
	viou; br4
#endif

#if     NKY > 0
. = ZERO+210
	kyio; br4
#endif

#if     NKX > 0
. = ZERO+220
	kxin; br4
	kxou; br4
#endif

#if     NVI > 0
. = ZERO+230
	kwlp85; br6
#endif

. = ZERO+240
	trap; br7+PIRQ.			/ programmed interrupt
	trap; br7+ARITHTRAP.		/ floating point
	trap; br7+SEGFLT.		/ segmentation violation

#if NRW > 0
. = ZERO+300
	rwio; br5+0.
	rwpd; br5+0.
#else
#if     NDW > 0
. = ZERO+300
	dwio; br7
#endif

#endif

#if NFD > 0
. = ZERO+310
	fdio; br5
#endif

#if NVI > 0
. = ZERO+320
	vifr; br5
#endif

#if NRW > 1
. = ZERO+330
	rwio; br5+1.
	rwpd; br5+1.
#endif

#if     NWL > 0
. = ZERO+770
	wlin; br5
	wlou; br5
#endif

/ floating vectors
. = ZERO + 1000
endvec = .				/ endvec should be past vector space
					/ (if NONSEP, should be at least 450)

/ overlay descriptor tables
. = ZERO+OVLY_TABLE_BASE
.globl	ova, ovd, ovend
#ifdef  IPK_XKOV
ova:    .=.+100                         / overlay addresses
ovd:    .=.+100                         / overlay sizes
#else
ova:	.=.+20				/ overlay addresses
ovd:	.=.+20				/ overlay sizes
#endif
ovend:	.=.+2				/ end of overlays

//////////////////////////////////////////////////////
/		interface code to C
//////////////////////////////////////////////////////

/
/ Ниже описана структура для C ( см. sys/main.c (vect_conflict()) )
/
/       struct vecconfl {
/               int vc_name;
/               int *vc_vect;
/               int vc_major;
/       };
/
.globl _vecconfl
_vecconfl:
#if     NRA > 0
	raio; 154; 14.
#endif
#if	NRL > 0
	rlio; 160; 8.
#endif
#if	NHK > 0
	hkio; 210; 4.
#endif
#if	NRK > 0
	rkio; 220; 0.
#endif
#if	NXP > 0
	xpio; 254; 6.
#endif
#if	NHP > 0
	hpio; 254; 6.
#endif
#if	NRM > 0
	rmio; 254; 6.
#endif
#if	NRP > 0
	rpio; 254; 1.
#endif
#if     NRX > 0
	rxio; 264; 11.
#endif
	0;    0;   0

#ifndef	KERN_NONSEP
.text
	/  This is text location 0 for separate I/D kernels.
	mov     $zjmp, -(sp)
	jsr	pc, _panic
	/*NOTREACHED*/

	/  Unmap is called from _doboot to turn off memory management.
	/  The "return" is arranged by putting a jmp at unmap+2 (data space).

	reset=	5
	.globl unmap
unmap:
	reset
	/  The next instruction executed is from unmap+2 in physical memory,
	/  which is unmap+2 in data space.

#endif	KERN_NONSEP

	.data
zjmp:	<jump to 0\0>
	.text

/  CGOOD and CBAD are used by autoconfig.
/  All unused vectors are set to CBAD
/  before probing the devices.

.globl	CGOOD, CBAD, _conf_int
	rtt = 6
CGOOD:	mov	$1, _conf_int ; rtt
CBAD:	mov	$-1,_conf_int ; rtt

#if NKL > 0
.globl	_klrint
klin:	jsr	r0,call; jmp _klrint
.globl	_klxint
klou:	jsr	r0,call; jmp _klxint
#endif

#if NWL > 0
.globl  _wlrint
wlin:   jsr     r0,call; jmp _wlrint
.globl  _wlxint
wlou:   jsr     r0,call; jmp _wlxint
#endif

#if NKX > 0
.globl  _kxrint
kxin:   jsr     r0,call; jmp _kxrint
.globl  _kxxint
kxou:   jsr     r0,call; jmp _kxxint
#endif

#if NKY > 0
.globl _kyintr
kyio:   jsr     r0,call; jmp _kyintr
#endif

.globl	_clock
#if NVI > 0     /* этой проверкой мы устанавливаем Эл-ку-85 */
kwlp85: tst     *$173030  / DEC Professional timer register C
#else
#ifdef DVK4
/ Тест для включения clock-ов. Ячейка isclock проверяется clkstart
/ на предмет определения "включен ли таймер". Чечик И.Е., ЛГУ Физ.фак.
.globl  _isclock
kwlp23: bis     $br7, 2(sp)     / Восстановить 7 приоритет "main"
	mov     KISD0, -(sp)    / Спрятать состояние страницы
	movb    $RW, KISD0      / Разрешить запись в младший лист
	mov     $kwlp, *$100    / Записать адрес и приоритет...
	mov     $br6, *$102     / ...собственно п/пр-мы обработки тиков
	mov     (sp)+, KISD0    / Вытащить нормальное состояние страницы
	clr     _isclock        / Зачистить флаг запроса
	rtt
#endif
#endif
kwlp:	jsr	r0,call; jmp _clock

#if NVI > 0
.globl  _virint
viin:   jsr     r0,call; jmp _virint
.globl  _vixint
viou:   jsr     r0,call; jmp _vixint
.globl  _softroll
vifr:   jsr     r0,call; jmp _softroll
#endif

#if	NDH > 0
.globl	_dhrint
dhin:	jsr	r0,call; jmp _dhrint
.globl	_dhxint
dhou:	jsr	r0,call; jmp _dhxint
#endif

#if     NDHU > 0
.globl  _dhurint
uhin:   jsr     r0,call; jmp _dhurint
.globl  _dhuxint
uhou:   jsr     r0,call; jmp _dhuxint
#endif

#if     NPC > 0
.globl  _pcrint
pcin:   jsr     r0,call; jmp _pcrint
.globl  _pcxint
pcou:   jsr     r0,call; jmp _pcxint
#endif

#if	NDJ > 0
.globl	_djrint
djin:	jsr	r0,call; jmp _djrint
.globl	_djxint
djou:	jsr	r0,call; jmp _djxint
#endif

#if	NDM > 0
.globl	_dmintr
dmin:	jsr	r0,call; jmp _dmintr
#endif

#if	NDZ > 0
.globl	_dzrint
dzin:	jsr	r0,call; jmp _dzrint
#ifndef	DZ_PDMA
.globl	_dzxint
dzou:	jsr	r0,call; jmp _dzxint
#endif	DZ_PDMA
#endif	NDZ

#if	NHK > 0
.globl	_hkintr
hkio:	jsr	r0,call; jmp _hkintr
#endif

#if	NHP > 0
.globl	_hpintr
hpio:	jsr	r0,call; jmp _hpintr
#endif

#if	NHS > 0
.globl	_hsintr
hsio:	jsr	r0,call; jmp _hsintr
#endif

#if	NHT > 0
.globl	_htintr
htio:	jsr	r0,call; jmp _htintr
#endif

#if	NLP > 0
.globl	_lpintr
lpio:	jsr	r0,call; jmp _lpintr
#endif

#if	NRK > 0
.globl	_rkintr
rkio:	jsr	r0,call; jmp _rkintr
#endif

#if     NRX > 0
.globl  _rxintr
rxio:   jsr     r0,call; jmp _rxintr
#endif

#if     NRW > 0
.globl  _rwintr, _rwpdma
rwio:   jsr     r0,call; jmp _rwintr
rwpd:   jsr     r0,call; jmp _rwpdma
#endif

#if     NFD > 0
.globl _fdintr
fdio:   jsr     r0,call; jmp _fdintr
#endif

#if	NRL > 0
.globl	_rlintr
rlio:	jsr	r0,call; jmp _rlintr
#endif

#if     NMY > 0
.globl  _myintr
myio:   jsr     r0,call; jmp _myintr
#endif

#if	NRM > 0
.globl	_rmintr
rmio:	jsr	r0,call; jmp _rmintr
#endif

#if	NRP > 0
.globl	_rpintr
rpio:	jsr	r0,call; jmp _rpintr
#endif

#if	NTM > 0
.globl	_tmintr
tmio:	jsr	r0,call; jmp _tmintr
#endif

#if	NTS > 0
.globl	_tsintr
tsio:	jsr	r0,call; jmp _tsintr
#endif

#if	NXP > 0
.globl	_xpintr
xpio:	jsr	r0,call; jmp _xpintr
#endif

#if     NDW > 0
.globl	_dwintr
dwio:   jsr     r0,call; jmp _dwintr
#endif

#if     NKS > 0
.globl  _ksrint
ksin:   jsr     r0,call; jmp _ksrint
.globl  _ksxint
ksou:   jsr     r0,call; jmp _ksxint
#endif  NKS

#if     NMK > 0
.globl	_mkrint
mkin:	jsr	r0,call; jmp _mkrint
.globl	_mkxint
mkou:	jsr	r0,call; jmp _mkxint
#endif  NMK

#if     NRA > 0
.globl  _udintr
raio:   jsr     r0,call; jmp _udintr
#endif

#if	NVP > 0
.globl  _vpintr
vpio:	jsr	r0,call; jmp _vpintr
#endif

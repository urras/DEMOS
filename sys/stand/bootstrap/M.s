/  Стартовый код для второй фазы загрузчика
/ с возможностью реализации UCB_AUTOBOOT
/
/ 94/04/27
/ Удалены все jbr-like команды ввиду ошибки в антоновском ассемблере -
/ jbr несовместима с локальными метками. zaitcev
/
/ 94/05/13
/ Вставлены заглушки для дебильных процессоров ВМ3, в которых mtpi
/ не работает из-под пользовательского режима (mtpi, mfpi, clrseg).
/
/ 94/05/15
/ Закомментирован _reloc(), который не используется в текущем boot.
/
/ $Log:	M.s,v $
/ Revision 1.5  88/11/02  22:15:31  dvolodin
/ *** empty log message ***
/ 
/ Revision 1.4  88/03/28  11:09:29  korotaev
/ После слияния с AZLK.
/
/ Revision 1.3  87/09/29  15:35:17  avg
/ Заменена проверка слова в идентификационном числе видеоконтроллера
/ на проверку байта.
/ (KLUGE! По хорошему надо сканировать занятые позиции и искать
/ расположение видеоконтроллера, флоппи и винчестера).
/
/ Revision 1.2  86/12/13  15:39:43  alex
/ Заменили clr на mov для улучшения росписи ОЗУ-П.
/
/ Revision 1.1  86/07/14  20:14:05  avg
/ Initial revision
/
/
/ Поддерживает 11/40, 11/45, 11/70, 11/23, 11/23+карта в/в (11/24), 11/73
/ и другие похожие машины

mfpi    = 6500^tst
stst    = 170300^tst
mtpi    = 6600^tst
mfpd    = 106500^tst
mtpd    = 106600^tst
spl     = 230
ldfps   = 170100^tst
stfps   = 170200^tst
wait    = 1
rtt     = 6
halt    = 0
reset   = 5
systrap = 104400

/  Режимы загрузки и устройство помещаются в последних SZFLAG байтах
/  ОЗУ ядром системы при перезагрузке.
/  Первая фаза загрузчика оставляет зто в регистрах;
/  эта фаза трет эти флаги в памяти так, чтобы они не могли быть
/  использованы еще раз.
ENDCORE=        160000          / конец памяти при выключенном ДП
SZFLAGS=        8.              / размер флагов загрузки
BOOTOPTS=       2               / размещение ключей, байтов перед ENDCORE
BOOTDEV=        4
CHECKWORD=      6
RBOOTDEV=       8.

.globl  _end
.globl  _main,_ubmapset
	jmp     start

/
/ Векторы прерываний
/
	trap;340        / ошибка шины: облом!
	trap;341        / плохая команда
	trap;342        / BPT
	trap;343        / IOT
	trap;344        / отказ питания
	trap;345        / EMT
tvec:
	start;346       / TRAP
.=400^.


start:
	reset
	mov     $340,PS
	mov     $140100,sp

/ сохранить флаги загрузки (если они есть)
	mov     r4,_bootopts
	mov     r3,_bootdev
	mov     r2,_checkword
	mov     r1,_rbootdev
/ потереть флаги в памяти
	clr     ENDCORE-BOOTOPTS
	clr     ENDCORE-BOOTDEV
	clr     ENDCORE-CHECKWORD
/
/ Определить тип процессора
/ первым делом проверяются ключи. Если они == 40, 24, 23, 45 или 70
/ установить соответствующий тип процессора.
/
	clrb    _sep_id
	clrb    _ubmap
	clrb    _haveCSW
	mov     $2f,nofault     / проверить наличие ключей!
	tst     *$SWREG
	clr     nofault         / ключи есть
	incb    _haveCSW
	mov     $40.,r0
	cmp     *$SWREG,$40
	jmp     gotcha
	cmp     *$SWREG,$23
	bne     1f
	mov     $23.,r0
	jmp     gotcha
1:
	cmp     *$SWREG,$24
	bne     1f
	mov     $24.,r0
	incb    _ubmap
	jmp     gotcha
1:
	cmp     *$SWREG,$45
	bne     1f
	mov     $45.,r0
	incb    _sep_id
	jmp     gotcha
1:
	cmp     *$SWREG,$70
	bne     2f
	mov     $70.,r0
	incb    _sep_id
	incb    _ubmap
	jmp     gotcha
/
/ Если на ключах ничего хорошего нет,
/ попытаемся определить тип процессора
/
2:
	mov     $40.,r0         / по умолчанию - 11/40
	mov     $2f,nofault
	mov     *$KDSA6,r1      / Есть ли раздельные поля i/d
	incb    _sep_id
	mov     $45.,r0
	mov     $8f,nofault
	mov     *$UBMAP,r1      / Есть ли карта ОШ
	incb    _ubmap
	mov     $70.,r0
	br      1f
2:
	mov     $9f,nofault
	mov     *$UBMAP,r1      / Есть ли карта ОШ
	incb    _ubmap
	mov     $24.,r0         / карта ОШ, нет разд. I/D = 11/24
	br      1f
9:      clr     nofault
	mov     $1f,nofault
	mov     *$SSR3, r1      / 22х-битовая адресация ?
	mov     $23., r0        / -- нет карты ОШ и разд. I/D = 11/23
	br      1f
8:      clr     nofault
	mov     $1f,nofault
	mov     *$SSR3, r1      / 22х-битовая адресация ?
	mov     $73., r0        / -- нет карты ОШ, есть разд. I/D = 11/73
1:      clr     nofault

gotcha:
	mov     r0,_cputype
/
/ Определить наличие видеомонитора DEC Professional
/
	mov     $0f,nofault
	mov     VI_RID, r0
	cmpb    r0, $02         / Проверить идентификационное
				/ число видеоконтроллера (3-я позиция)
	bne     0f
	inc     _video
0:
	clr nofault

/
/       Установить регистры поля I системного режима
/       в физ. адрес 0 и на страницу В/В
/
	clr     r1
	mov     $77406, r2
	mov     $KISA0, r3
	mov     $KISD0, r4
	jsr     pc, setseg
	mov     $IO, -(r3)

/
/       Установить регистры поля I режима пользователя
/       в физ. адрес 128Кб и на страницу В/В
/
	mov     $6000, r1       / 06000 = 192*1024/64
	mov     $UISA0, r3
	mov     $UISD0, r4
	jsr     pc, setseg
	mov     $IO, -(r3)

/
/       Если процессор типа 11/40, нужны только регистры пространства I
/
	tstb    _sep_id
	beq     1f

/
/       Установить регистры поля D системного режима
/       в физ. адрес 0 и на страницу В/В
/
	clr     r1
	mov     $KDSA0, r3
	mov     $KDSD0, r4
	jsr     pc, setseg
	mov     $IO, -(r3)

/
/       Установить регистры поля D режима пользователя
/       в физ. адрес 128Кб и на страницу В/В
/
	mov     $6000, r1       / 06000 = 192*1024/64
	mov     $UDSA0, r3
	mov     $UDSD0, r4
	jsr     pc, setseg
	mov     $IO, -(r3)

1:
/ включить ДП
	clrb    _ksep
	tstb    _ubmap
	beq     2f
	jsr     pc,_ubmapset
	tstb    _sep_id
	bne     3f
	mov     $60,SSR3        / 22-битовая адресация, нет разделения I/D
	br      1f
3:
	mov     $65,SSR3        / 22-битовая адресация, разделенные поля I/D
	movb    $1,_ksep
	cmp     _cputype,$70.
	bne     1f
	mov     $3,MSCR
	br      1f
2:
	tstb    _sep_id         / нет карты ОШ; sep_id?
	beq     1f
	mov     $5,SSR3
	movb    $1,_ksep
1:
	cmp     _cputype,$23.
	bne     9f
	mov     $60, SSR3       / Включить 22xбитовую адресацию
9:
	cmp     _cputype,$73.
	bne     1f
	bis     $20,SSR3        / Включить 22-х битовую адресацию
	mov     $1,MSCR         / Disable cache parity interrupt
1:

	mov     $30340,PS
	inc     SSR0


/ копировать программу в польз. поле I
	mov     $_end,r0
	clc
	ror     r0
	clr     r1
1:
	mov     (r1),-(sp)
	mtpi    (r1)+
	sob     r0,1b


/ продолжить работу в пользовательском пр-ве
	mov     $140004,sp
	tstb    _sep_id
	bne     1f
	clr     *$KISA6
	br      2f
1:
	clr     *$KDSA6
2:      mov     $140340,-(sp)
	mov     $user,-(sp)
	rtt
user:
/ очистить bss
	mov     $_edata,r0
	mov     $_end,r1
	sub     r0,r1
	inc     r1
	clc
	ror     r1
1:
	mov     $0,(r0)+  / clr не везде проходит
	sob     r1,1b
	mov     $_end+512.,sp
	mov     sp,r5

	jsr     pc,_main
	mov     _cputype,r0
	mov     _bootopts,r4
	mov     r4,r2
	com     r2              / контрольное слово
	systrap

	br      user

setseg:
	mov     $8.,r0
1:
	mov     r1,(r3)+
	add     $200,r1
	mov     r2,(r4)+
	sob     r0,1b
	rts     pc

.globl  _setseg
_setseg:
	mov     2(sp),r1
	mov     r2,-(sp)
	mov     r3,-(sp)
	mov     r4,-(sp)
	mov     $77406,r2
	mov     $KISA0,r3
	mov     $KISD0,r4
	jsr     pc,setseg
	tstb    _ksep
	bne     1f
	mov     $IO,-(r3)
1:
	mov     (sp)+,r4
	mov     (sp)+,r3
	mov     (sp)+,r2
	rts     pc

.globl  _setnosep
_setnosep:
	bic     $4,SSR3 / выключить разделение полей i/d ядра
	clrb    _ksep
	rts pc

.globl  _setsep
_setsep:
	bis     $4,SSR3 / включить разделение полей i/d ядра (если выключено)
	movb    $1,_ksep
	rts pc

/P3: Очень странно, но reloc() не использован в загрузчике.
/Выбрасываем его от греха подальше.
/
//  Переместиться в  более старшие адреся ОЗУ -- если ядро большое.
/.globl  _reloc, _segflag
/_reloc:
/	jsr     r5, csv
/	mov     $2f, nofault
/	/  Установить регистры поля I ядра на новую область
/	mov     $6000, r1                       / 192 Кб
/	mov     $77406,r2
/	mov     $KISA0, r3
/	mov     $KISD0, r4
/	jsr     pc, setseg
/	/  Копироваться в новую область
/	clr     r0
/	mov     $28.*1024., r1                  / 28К слов
/1:
/	mov     (r0), -(sp)
/	mtpi    (r0)+
/	sob     r1, 1b
/	clr     nofault
/
/	/  Переустановить регистры поля I ядра в 0.
/	clr     -(sp)
/	jsr     pc, _setseg
/	tst     (sp)+
/
/	/  Установить пользовательские регистры поля D на новую область
/	tstb    _sep_id
/	beq     1f
/	mov     $6000, r1                       / 192 Кб
/	mov     $UDSA0, r3
/	mov     $UDSD0, r4
/	jsr     pc, setseg
/	mov     $IO, -(r3)
/
/	/  Пользовательские регистры поля I --
/	/   выполнение продолжается уже на новой копии.
/1:
/	mov     $6000, r1                       / 192 Кб
/	mov     $UISA0, r3
/	mov     $UISD0, r4
/	mov     $7,r0
/1:
/	mov     r1,(r3)+
/	add     $200,r1
/	mov     r2,(r4)+
/	sob     r0,1b
/
/	mov     $3, _segflag                    / новые биты расширеной памяти
/	clr     r0                              / ... успех
/	jmp     cret
/
/	/  Прерывание по ошибке шины (недостаточно памяти)
/2:
/	mov     $-1, r0
/	jmp     cret

/P3: Заменена на медленный аналог на C, работающий через mtpi().
/Это проще, чем переписывать, и не настолько уж медленно.
// clrseg(addr,count)
/.globl  _clrseg
/_clrseg:
/	mov     4(sp),r0
/	beq     2f
/	asr     r0
/	bic     $!77777,r0
/	mov     2(sp),r1
/1:
/	clr     -(sp)
/	mtpi    (r1)+
/	sob     r0,1b
/2:
/	rts     pc


/ mtpd(word,addr)
.globl  _mtpd
_mtpd:
	cmp     _cputype,$23.		/ ДВК, однако
	beq	mtpi_23
	mov     4(sp),r0
	mov     2(sp),-(sp)
	mtpd    (r0)+
	rts     pc

/ mtpi(word,addr)
.globl  _mtpi
_mtpi:
	cmp     _cputype,$23.
	beq	mtpi_23
	mov     4(sp),r0
	mov     2(sp),-(sp)
	mtpi    (r0)+
	rts     pc

/P3: Вот mtpi(), которое не содержит mtpi.
/В оригинале есть побочный эффект: возвращается адрес+2. Не знаю, нужно
/ли это кому-нибудь. На всякий случай поддержано. Предполагается, что
/мы находимся где-то внизу адресного пространства программы и можем
/двигать сегмент #6.
mtpi_23:
	mov	4(sp),r0	/ addr
	mov	2(sp),r1	/ value
	mov	r2,-(sp)
	mov	r3,-(sp)

	mov	r0,r2		/ Вычисляем индекс сегмента назначения
	ash     $-12.,r2
	bic     $177761,r2

	mov	*$UISA6,r3	/ Спасаем и устанавливаем сегмент #6
	mov	KISA0(r2),*$UISA6

	mov	r0,r2		/ Относим смещение к базе #6
	bic     $160000,r2
	bis	$140000,r2

	mov	r1,*r2

	mov	r3,*$UISA6	/ Восстанавливаем базу #6

	mov	(sp)+,r3
	mov	(sp)+,r2
	add	$2,r0
	rts	pc

/ unsigned mfpi(addr)
.globl  _mfpi
_mfpi:
	cmp	_cputype,$23.
	beq	mfpi_23
	mov     2(sp),r0
	mfpi    (r0)
	mov (sp)+,r0
	rts     pc

mfpi_23:
	mov	2(sp),r0
	mov	r2,-(sp)
	mov	r0,r2
	ash     $-12.,r2
	bic     $177761,r2
	mov	*$UISA6,r1
	mov	KISA0(r2),*$UISA6
	bic     $160000,r0
	bis	$140000,r0
	mov	*r0,r0
	mov	r1,*$UISA6
	mov	(sp)+,r2
	rts	pc

.globl  __rtt
__rtt:
	halt

.globl  _trap

trap:
	mov     *$PS,-(sp)
	mov     r0,-(sp)
	mov     r1,-(sp)
	tst     nofault
	bne     3f
	jsr     pc,_trap
	mov     (sp)+,r1
	mov     (sp)+,r0
	tst     (sp)+
	rtt
3:      mov     (sp)+,r1
	mov     (sp)+,r0
	tst     (sp)+
	mov     nofault,(sp)
	rtt

.globl  _autoboot
_autoboot:
	mov     r1,-(sp)
	clr     r0
	mov     $60.,r1
1:
	sob     r0,1b
	sob     r1,1b
	cmpb    $3,*$177562
	beq     1f
	inc     r0
1:
	mov     (sp)+,r1
	rts     pc

PS      = 177776
SSR0    = 177572
SSR1    = 177574
SSR2    = 177576
SSR3    = 172516
KISA0   = 172340
KISA1   = 172342
KISA6   = 172354
KISA7   = 172356
KISD0   = 172300
KISD7   = 172316
KDSA0   = 172360
KDSA6   = 172374
KDSA7   = 172376
KDSD0   = 172320
KDSD5   = 172332
SISA0   = 172240
SISA1   = 172242
SISD0   = 172200
SISD1   = 172202
UISA0   = 177640
UISA6	= 177654
UISD0   = 177600
UDSA0   = 177660
UDSD0   = 177620
MSCR    = 017777746     / регистр управления памятью 11/70
IO      = 177600
SWREG   = 177570
UBMAP   = 170200
VI_RID  = 174400        / Регистр идентификации видеоконтроллера
			/  --- 3 позиция сист. магистрали ---

.data
.globl  _cputype
.globl  _ksep, _sep_id, _ubmap, _haveCSW, _video
.globl  _bootopts, _bootdev, _checkword, _rbootdev

nofault:        .=.+2   / установлена ловушка прерываний
_cputype:       .=.+2   / тип процессора (40, 45, 23, 24 или 70)
_sep_id:        .=.+1   / 1 если есть раздельные поля I и D
_ubmap:         .=.+1   / 1 если есть карта ОШ
_haveCSW:       .=.+1   / 1 если есть регистр консольных переключателей
_ksep:          .=.+1   / 1 если у ядра включено разделение полей I/D
_bootopts:      .=.+2   / флаги для автозагрузки
_bootdev:       .=.+2   / у-во для загрузки системы, если нет RB_ASKNAME
_checkword:     .=.+2   / сохр. r2, дополнение bootopts для автозагрузки
_video:         .=.+2   / единица, если вместо консольного терминала
			/ используется видеомонитор DEC Professional
_rbootdev:      .=.+2   / Real boot device

/
/ Стартовый код для автономных утилит
/
/ $Log:	srt0.s,v $
/ Revision 1.2  88/11/02  21:42:31  dvolodin
/ *** empty log message ***
/ 
/ Revision 1.1  86/07/14  19:11:35  avg
/ Initial revision
/
/
/ Начальный загрузчик передает тип CPU в r0
/

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
reset   = 5
/ trap  = 104400

PS      = 177776
VI_RID  = 174400        / Регистр идентификации видеоконтроллера
			/  --- 3 позиция сист. магистрали ---

.globl  _end
.globl  _main, __rtt
.globl  _edata
	jmp     start

/
/ Вектора прерываний
/
	trap;340        / ош. шины
	trap;341        / несущ. команда
	trap;342        / BPT
	trap;343        / IOT
	trap;344        / отказ питания
	trap;345        / EMT
tvec:
	start;346       / TRAP
.=400^.
.text


start:
	mov     $340,*$PS
	mov     $trap,tvec
/
/ определить тип процессора
	mov     r0,*$_cputype   / загрузчик оставляет его в r0
/ есть ли консольный регистр ?
	mov     $1f,nofault
	tst     CSW
	inc     _haveCSW
1:
	clr     nofault
/
/ Определить наличие видеомонитора DEC Professional
/
	mov     $0f,nofault
	mov     VI_RID, r0
	cmp     r0, $174402     / Проверить идентификационное
				/ число видеоконтроллера
	bne     0f
	inc     _video
0:
	clr nofault
/
	mov     $157776,sp
	mov     $_edata,r0
	mov     $_end,r1
	sub     r0,r1
	inc     r1
	clc
	ror     r1
1:
	clr     (r0)+
	sob     r1,1b
	jsr     pc,_main

/ Установить стек на адрес, где записана пара ps-pc,
/ (верхние адреса памяти).
/ так что возможен возврат в загрузчик

__rtt:
	mov     $140000,sp
	rtt                             / с богом!
	br      .


.globl  _trap
trap:
	mov     *$PS,-(sp)
	mov     r0,-(sp)
	mov     r1,-(sp)
	tst     nofault
	bne     3f
2:      jsr     pc,_trap
	mov     (sp)+,r1
	mov     (sp)+,r0
	tst     (sp)+
	rtt
3:      mov     (sp)+,r1
	mov     (sp)+,r0
	tst     (sp)+
	mov     nofault,(sp)
	rtt

.data
.globl  _cputype, _haveCSW, _video

nofault:        .=.+2   / установлена ловушка для прерывания
_cputype:       .=.+2   / тип процессора (может быть 23,24,40,45 или 70)
_haveCSW:       .=.+2   / единица, если есть консольный регистр
_video:         .=.+2   / единица, если вместо консольного терминала
			/ используется видеомонитор DEC Professional
KISA6 = 172354
KISA7 = 172356
KDSA6 = 172374
UBMAP = 170200
CSW   = 177570

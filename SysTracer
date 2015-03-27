  jmp 4f
     .=.+40
  4:
  mov pc,sp
  mov $Tracer,*$14 ; mov $340,*$16
  bpt

  mov $10.,r1 ; clr r0
  7:
    asl r0;  add r1,r0
  br 7b

// - - - - - - - - - - -   23.01.90  10:13'  VERSION 45
//                                                   //
//   MULTIPURPOUSE INTERACTIVE USER-FRIENDLY SYSTEM  //
//                                                   //
//                   System Tracer                   //
// - - - - - - - - - - - - - - - - - - - - - - - - - //
// Заметим, что:
//   Все данные хранятся в сегменте текста

/USE PLM:PACKAG
ET=03
CL=10
RN=12
LF=15

coost=177564;  coist=177560
CHAVEC = 04;  RESVEC = 10;  TVEC = 14
Tbit = 20

 .even
TRCFLG: .byte 0               /Флаг "Идет пошаговая отладка"
STPFLG: .byte 0               /savcom содержит слово
SAVCOM:  0                    /Слово, вытесненное стопором
POINT:   0                    /"Точка зрения" отладчика (и стопор тоже)

RTICOP = 000002;  BPTCOP = 000003   /Коды передвигаемых операций
NEXTPC:  0

 .even
R0PROG:  0; 0; 0; 0; 0; 0
SPPROG:  0
PCPROG:  0
PSPROG:  0

ISTR:   <R71234 >;  ILEN=.-ISTR;  .byte 0   /Буфер ввода строки

RNLF:   .byte  RN,LF,0
STPTXT: .byte RN,LF; <=STOPOR=>; .byte 0
SYMTAB: <0123456789ABCDEF>
HXIMAG: <TTTT\0>
ULUL:   <__\0>
EMESS:  .byte RN,LF; <??\0>

 .even
 .=.+[50*2]
OWNSTK:

INST:
  MNEMO=.-INST; <MOVB >
  OPER =.-INST; <*0000(R0),@0000(R0) >
  INSTLN=.-INST
  <\0>

HEXLN = 4


  .globl Tracer
Tracer:
//
//  Переходим в контекст трассировщика
//
 mov r0,R0PROG
 mov $[R0PROG+2],r0
 mov r1,(r0)+
 mov r2,(r0)+
 mov r3,(r0)+
 mov r4,(r0)+
 mov r5,(r0)+
 tst (r0)+
 mov (r6)+,(r0)+
 mov (r6)+,(r0)+
 mov r6,SPPROG
 mov $OWNSTK,r6
//
//  Определяем, что произошло и, если надо,
//  снимаем стопор и диагностируем.
//
 tstb TRCFLG;   bne I_1            /Не было пошаговой отладки ?
   mov $STPTXT,r0;   jsr r7,DPC    /Значит, выскочили на стопор
 I_1:
 tstb STPFLG;   beq I_2            /Был установлен сторор ->
   mov POINT,r0;  mov SAVCOM,*r0   /Снимаем его
   add $2,r0;  cmp r0,PCPROG       /Выскочили на него же ?
   bne I_3
     sub $2,PCPROG                 /Поправляемся на слово с BPT
   I_3:
 I_2:
 clrb STPFLG;  clrb TRCFLG
//
//  Выдаем строчку с командой и регистрами
//
INSOUT:
 mov PCPROG,POINT
 mov $RNLF,r0
 jsr r7,DPC
 cmp NEXTPC,PCPROG
 beq I_4
   mov $'*,r0
 br I_5
 I_4:
   mov $'\s,r0
 I_5:
 jsr r7,DPO
 mov $INST,r0
 mov POINT,r1
 jsr r7,GETCOM
 mov r0,NEXTPC
 mov $INST,r0
 jsr r7,DPC
 mov $R0PROG,r2

 mov $10,r1
 br X_7
 X_8:
   mov (r2)+,r0
   jsr r7,BNHX4
   jsr r7,DPC
   mov $'\s,r0
   jsr r7,DPO
 X_7:
 dec r1;  bpl X_8
 mov $'\s,r0
 jsr r7,DPO
 mov PSPROG,r0
 jsr r7,BNHX4
 jsr r7,DPC

INPUT:
 mov $':,r0
 jsr r7,DPO

 mov $ISTR,r1
 mov $ILEN,r2
//----------------
//  Ввод строки  (inline)
//----------------
 mov r1,r4
 clr r3                       /Актуальная длина строки
 W_9:
   jsr r7,DPI
   cmp  r0,$ET;  beq W_10
   cmpb r0,$RN;  beq W_10
   cmpb r0,$LF;  beq W_10

    // Преобразуем буквы в большие
    //  if( (c & 0x60) == 0x60 ) c -= 0x20;
   mov r0,-(sp)
   bic $177630,*sp
   cmp $000140,(sp)+
   bne 1f
     sub $040,r0
   1:

   cmpb r0,$'\s;  blt I_11;  cmpb r0,$177;  bge I_11
     movb r0,(r1)+
     jsr r7,DPO
     inc r3
     cmp r3,r2;  blt I_12
       dec r1;   dec r3
       mov $CL,r0
       jsr r7,DPO
     I_12:
   br I_13
   I_11:
   cmpb r0,$CL;  beq V_15;  cmpb r0,$177;  bne I_14;  V_15:
     tst r3;  ble I_16
       dec r3;   dec r1
       jsr r7,DPO
     I_16:
   I_14:
   I_13:
 br W_9
 W_10:
 mov r4,r1

//-----------------
//  Интерпретация введенной команды
//-----------------
 tst r3;  bne I_17            /Ничего не ввели - пошаговое исполнение
   bis $Tbit,PSPROG
   incb TRCFLG
   br GO
 I_17:
 cmpb *r1,$'J;  bne I_18      /Останов на следующей инструкции
   incb TRCFLG
   mov NEXTPC,r4
   br STOPR
 I_18:
 cmpb *r1,$'S;  bne I_19      /Стопор в точке зрения
   mov POINT,r4
STOPR:
   mov *r4,SAVCOM
   mov $BPTCOP,*r4
   incb STPFLG
   mov r4,POINT               /Спасаем POINT для входных проверок
   bic $Tbit,PSPROG
   br GO
 I_19:
 cmpb *r1,$'N;  bne I_20      /Запуск без контроля отладчика
   bic $Tbit,PSPROG
GO:
   mov $PSPROG+2,r0
   mov SPPROG,r6
   mov -(r0),-(r6);   mov -(r0),-(r6)     /PS & PC для выполнения rtt
   tst -(r0)
   mov -(r0),r5;  mov -(r0),r4;  mov -(r0),r3
   mov -(r0),r2;  mov -(r0),r1;  mov R0PROG,r0
   rtt
 I_20:
DONE1:
 cmpb *r1,$'R;  bne I_21      /Модификация РОН
   inc r1;   dec r3
    /Грузим в r2 адрес ячейки с регистром
   movb *r1,r2;  sub $'0,r2;  bic $177770,r2;  asl r2;  add $R0PROG,r2
   jsr r7,CONVI;   mov r0,*r2
   jmp INSOUT
 I_21:
 cmpb *r1,$'^;  bne I_22      /Перевыдача строки с инструкцией
   jmp INSOUT
 I_22:                        /Задание адреса для просмотра памяти
 cmpb *r1,$'\s; beq V_24; cmpb *r1,$'G; bne I_23; V_24:
   jsr r7,CONVI;   mov r0,POINT
   br WRDOUT
 I_23:
 cmpb *r1,$'@;  bne I_25      /Косвенное задание адреса
   mov POINT,r3
   jsr r7,TSTIT
   tst r5;  bne ERR
   mov *POINT,POINT
   br WRDOUT
 I_25:
 cmpb *r1,$'Y;  bne I_26      /Дамп байт
   jsr r7,CONVI; tst r0
   bne I_27
     mov $1,r0
   I_27:
   mov r0,-(r6)
   br X_28
   X_29:
     jsr r7,YDUMP
     add $020,POINT
   X_28:
   dec *r6;  bpl X_29
   mov (r6)+,r0
   jmp INPUT
 I_26:
 cmpb *r1,$'=;  bne I_30      /Модификация слова в точке зрения
   mov r3,-(r6) ; mov r1,r4   /Спасаем регистры
   mov POINT,r3
   jsr r7,TSTIT
   mov (r6)+,r3
   cmp r3,$1;  beq ERR;  tst r5;  bne ERR   /Нечего или некуда писать
   mov r4,r1
   jsr r7,CONVI
   mov r0,*POINT
   br WRDOUT
 I_30:
 cmpb *r1,$'+;  beq V_32;  cmpb *r1,$'D;  bne I_31;  V_32:
   add $2,POINT
   br WRDOUT
 I_31:
 cmpb *r1,$'-;  beq V_34;  cmpb *r1,$'U;  bne I_33;  V_34:
   sub $2,POINT
   br WRDOUT
 I_33:
ERR:
 mov $EMESS,r0
 jsr r7,DPC
 jmp INPUT
//-----------------
//  Показ памяти
//-----------------
/ Вызов:  POINT - адрес нужного слова
WRDOUT:
 mov POINT,r3
 jsr r7,ADOUT
 inc r3;   jsr r7,BYTOUT      /Старший байт
 dec r3;   jsr r7,BYTOUT      /Младший байт
 mov $'\s,r0;   jsr r7,DPO
 tst r5
 bne I_35
   mov $INST,r0
   mov r3,r1
   jsr r7,GETCOM              /Четность проверяется в GETCOM
 br I_36
 I_35:
   mov $INST,r0
   mov $INSTLN,r1
   mov $'\s,r2
   jsr r7,CLEAR
 I_36:
 mov $INST,r0
 jsr r7,DPC
 jmp INPUT

YDUMP:
 mov POINT,r3
 jsr r7,ADOUT
 mov $010,r4
 br X_37
 X_38:
   inc r3; jsr r7,BYTOUT; dec r3; jsr r7,BYTOUT; add $2,r3
   mov $'\s,r0;  jsr r7,DPO
 X_37:
 dec r4; bpl X_38
 sub $020,r3
                    /Подменяем вектор ошибки
 mov $CHAVEC,r1; mov *r1,r2; mov $ITBLCK,*r1

 mov $020,r4
 br X_39
 X_40:
   clr r5
   tstb *r3         /Здесь возможно прерывание
   tst r5
   beq I_41
     mov $'\s,r0;  jsr r7,DPO;   inc r3;
   br I_42
   I_41:
     movb (r3)+,r0
     cmp r0,$'\s; blt V_44; cmpb r0,$177; blt I_43; V_44:
       mov $'.,r0
     I_43:
     jsr r7,DPO
   I_42:
 X_39:
 dec r4;  bpl X_40
 mov r2,*r1         /Восстанавливаем вектор
 rts r7

ADOUT:
 mov $RNLF,r0
 jsr r7,DPC
 bit $1,r3; beq I_45
   mov $'<,r0
 br I_46
 I_45:
   mov $'#,r0
 I_46:
 jsr r7,DPO
 mov r3,r0
 jsr r7,BNHX4
 jsr r7,DPC
 mov $'=,r0
 jsr r7,DPO
 rts r7

BYTOUT:
 mov $CHAVEC,r1;  mov *r1,r2;  mov $ITBLCK,*r1  /Подменяем вектор
 clr r5
 tstb *r3                                       /Возможно прерывание
 mov r2,*r1                                     /Вектор - на место
 tst r5; beq I_48
   mov $ULUL,r0
 br I_49
 I_48:
   movb *r3,r0;  mov $2,r1;  jsr r7,BNHX
 I_49:
 jsr r7,DPC
 rts r7

TSTIT:
 mov $CHAVEC,r1;  mov *r1,r2;  mov $ITBLCK,*r1  /Подменяем вектор
 clr r5
 tstb *r3    /Возможно прерывание (кроме канального еще нечетный адрес)
 mov r2,*r1  /Вектор - на место
 rts r7

ITBLCK:
 inc r5
 rti

//---------------
//  Преобразование числа в текст
//---------------
BNHX4:
 mov r1,-(r6)
 mov $4,r1
 jsr r7,BNHX
 mov (sp)+,r1
 rts r7
BNHX:               /Возвращает указатель на статический буфер
 mov r2,-(r6);  mov r1,r2
 mov $HXIMAG+4,r1;  sub r2,r1
 jsr r7,BINHEX
 mov (r6)+,r2
 mov r1,r0
 rts r7

 /Библиотечный преобразователь - работает в указанном буфере
BINHEX:
 mov r0,-(r6); mov r3,-(r6); mov r4,-(r6)
 add r2,r1
 mov r2,r3
 br X_51
 X_52:
   mov r0,r4;   bic $177760,r4;   movb SYMTAB(r4),-(r1)
   asr r0; asr r0; asr r0; asr r0
 X_51:
 dec r3; bpl X_52
 mov (r6)+,r4; mov (r6)+,r3; mov (r6)+,r0
 rts r7

/ Внутренний MVS: r0-куда, r1-откуда, r2-сколько
MVS:
 br X_53
 X_54:
   movb (r1)+,(r0)+
 X_53:
 dec r2; bpl X_54
 rts r7

//-----------------
//  Входное преобразование текста в число
//-----------------
CONVI:
 inc r1;   dec r3             /Вынесено из разбора для сокращения кодов
 mov r2,-(r6)
 clr r0
 br X_55
 X_56:
   cmpb *r1,$'0;  blt I_57;  cmpb *r1,$'9;  bgt I_57
     movb (r1)+,r2;  sub $'0,r2
   br I_58
   I_57:
   cmpb *r1,$'A;  blt I_59;  cmpb *r1,$'F;  bgt I_59
     movb (r1)+,r2;  sub $['A-10.],r2
   br I_58
   I_59:
     br cnvret
   I_58:
   asl r0; asl r0; asl r0; asl r0
   bis r2,r0
 X_55:
 dec r3; bpl X_56
cnvret:
 mov (r6)+,r2
 rts r7

//---------------
//  Обслуживание дисплея
//---------------
DPC:
 mov r1,-(r6)
 mov r0,r1
 W_60:
 movb (r1)+,r0; beq W_61
   jsr r7,DPO
 br W_60
 W_61:
 mov (r6)+,r1
 rts r7

DPO:
 W_62:
   tstb *$coost
 bpl W_62
 mov r0,*$[coost+2]
 rts r7

DPI:
 W_63:
   tstb *$coist
 bpl W_63
 mov *$[coist+2],r0
 bic $177200,r0
 rts r7

//-------------------
//  Сборка мнемоники
//-------------------
TEXT  = 0;  GROUP = 4      /Стуктура статьи команды

RTS_  = 0;  SOB_  = 1;  MARK_ = 2;  ONE_  = 3    /Группы по аргументам
TWO_  = 4;  BRU_  = 5;  RSS_  = 6;  NO_   = 7

MEMADR: 0
TYPCOM: 0

/ Данные ARGUM
TYP:    0
REG:    0
  ABS = 10; REL = 12
TYPADR: .byte  0,0,ABS,[ABS+1],0,0,REL,[REL+1]

/ Вызов: r0-Адрес строки
/        r1-PC программы
/ Выход: r0-Адрес конца инструкции
GETCOM:
 mov r1,-(r6)
 mov r2,-(r6)
 mov r3,-(r6)
 mov r4,-(r6)
 mov r5,-(r6)
 mov r0,r4

 mov r1,MEMADR
 mov $INSTLN,r1
 mov $'\s,r2
 jsr r7,CLEAR
 mov MEMADR,r1

 bit $1,r1;  beq I_64         /PC нечетный ?
   mov r4,r0
   mov $5,r1
   mov $'*,r2
   jsr r7,CLEAR
   jmp RET
 I_64:

 add $2,MEMADR
 mov *r1,r3
 mov r3,r0
 jsr r7,SEARCH
 tst r0;  bne I_65            /Команда не найдена ?
   mov r4,r0
   mov $5,r1
   mov $'?,r2
   jsr r7,CLEAR
   jmp RET
 I_65:
 mov r1,TYPCOM
 mov r0,r1
 mov r4,r0
 mov $4,r2
 jsr r7,MVS
 mov r0,r4
 tst TYPCOM;  beq I_66        /Байтовая команда
   movb $'B,-1(r4)
 I_66:
 movb $'\s,(r4)+
 mov *r1,r0
 asl r0
 add r0,r7
 br _RTS; br _SOB; br _MARK; br _ONE; br _TWO;
 br _BRU; br _RSS; br _NOAD

_RTS:
 mov r3,r0
 jsr r7,RDUM
 br RET

_SOB:
 mov r3,r0
 mov $6,r2; br X_67
 X_68:
   asr r0
 X_67:
 dec r2; bpl X_68
 jsr r7,RDUM
 movb $',,(r4)+
_MARK:
 mov r3,r0; bic $177700,r0
 mov r4,r1
 mov $2,r2
 jsr r7,BINHEX
 br RET

_ONE:
 mov r3,r0
 jsr r7,ARGUM
 br RET

_TWO:
 mov r3,r0
 mov $6,r2; br X_69
 X_70:
   asr r0
 X_69:
 dec r2; bpl X_70
 jsr r7,ARGUM
 movb $',,(r4)+
 mov r3,r0
 jsr r7,ARGUM
 br RET

_BRU:
 movb $'$,(r4)+
 movb r3,r0
 tst r0;  bge I_71
   movb $'-,(r4)+
   neg r0
 br I_72
 I_71:
   movb $'+,(r4)+
 I_72:
 mov r4,r1
 mov $2,r2
 jsr r7,BINHEX
 br RET

_RSS:
 mov r3,r0
 mov $6,r2; br X_73
 X_74:
   asr r0
 X_73:
 dec r2; bpl X_74
 jsr r7,RDUM
 movb $',,(r4)+
 mov r3,r0
 jsr r7,ARGUM
 br RET

_NOAD:
 mov r3,r0
 mov r3,r1; bic $377,r1

  /EMT или TRAP
 cmp r1,$104000; beq V_76; cmp r0,$104400; bne I_75; V_76:

   mov r4,r1
   mov $2,r2
   jsr r7,BINHEX
 br I_77
 I_75:
 mov r3,r1
 bic $000017,r1
 cmp r1,$240; beq V_79; cmp r1,$260; bne I_78; V_79:  /Oперации с NZVC
   mov r4,r1
   mov $1,r2
   jsr r7,BINHEX
 I_78:
 I_77:
 br RET

RET:
 mov (r6)+,r5
 mov (r6)+,r4
 mov (r6)+,r3
 mov (r6)+,r2
 mov (r6)+,r1
 mov MEMADR,r0
 rts r7

RDUM:
 movb $'R,(r4)+
 bic $177770,r0;  add $'0,r0;  movb r0,(r4)+
 rts r7

CLEAR:
 br X_80
 X_81:
   movb r2,(r0)+
 X_80:
 dec r1; bpl X_81
 rts r7

// Получение операнда по регистру и адресации
/ Вызов: r0-операнд (кусок команды)
/        r1-Адрес индексного слова (т.е. сразу за COP'ом)
/        r4-Адрес буфера
/ Выход: в буфере - раскрутка аргумента
ARGUM:
 mov r2,-(r6)
 mov r3,-(r6)
 mov r5,-(r6)
 bic $177700,r0       / r0-Аргумент команды
 mov MEMADR,r1        / r1-Адрес индексного слова

 mov r5,-(r6)
   mov r0,r2;  asr r2; asr r2; asr r2;  bic $177770,r2   /Тип адресации
   mov r0,r3;  bic $177770,r3                            /Регистр
   clr r0
   cmp r3,$7;  beq I_94                 /Регистр - НЕ PC
     cmp r2,$6;  blt I_95               /Индексная адресация
       mov (r1)+,r0
     I_95:
   br I_96
   I_94:
     mov $TYPADR,r5
     add r2,r5
     tstb *r5;  beq I_97
       movb *r5,r2
       mov (r1)+,r0
     I_97:
   I_96:
 mov (r6)+,r5
                                  / r0-индексное слово
 mov r1,MEMADR                    / r1-новый адрес
 mov r2,TYP
 mov r3,REG

 bit $1,r2;  beq I_82
   movb $'@,(r4)+
 I_82:
 cmp r2,$ABS; blt I_83        /Адресация через PC
   cmp r2,$REL; bge I_84
     movb $'#,(r4)+
   br I_85
   I_84:
     add MEMADR,r0
   I_86:
   I_85:
   mov r4,r1;  mov $4,r2;  jsr r7,BINHEX;  add $4,r4
 br I_87
 I_83:
   bic $01,r2;  bne I_88      /Прямая адресация
     movb $'R,(r4)+
     add $'0,r3
     movb r3,(r4)+
   br I_89
   I_88:
     cmp r2,$6; bne I_90      /Индексная адресация
       mov r4,r1;  mov $4,r2;  jsr r7,BINHEX;  add $4,r4
     br I_91
     I_90:
     cmp r2,$04;  bne I_92    /Автодекремнтная
       movb $'-,(r4)+
     I_92:
     I_91:
     movb $'(,(r4)+
     movb $'R,(r4)+
     mov REG,r3;  add $'0,r3;  movb r3,(r4)+
     movb $'),(r4)+
     cmp r2,$02;  bne I_93    /Автоинкрементная
       movb $'+,(r4)+
     I_93:
   I_89:
 I_87:
 mov (r6)+,r5
 mov (r6)+,r3
 mov (r6)+,r2
 rts r7

//----------------
// Поиск мнемоники по КОП'у
//----------------
TWOTAB: 0;0;0;IMOV;0;ICMP;0;IBIT;0;IBIC;0;IBIS;0;IADD;0;0
        0;0;1;IMOV;1;ICMP;1;IBIT;1;IBIC;1;IBIS;0;ISUB;0;0

BRUTAB:   0;   IBPL;IBR ;IBMI;IBNE;IBHI;IBEQ;IBLOS
          IBGE;IBVC;IBLT;IBVS;IBGT;IBCC;IBLE;IBCS

ONETAB:   0;IROR;1;IROR ;0;IROL;1;IROL
          0;IASR;1;IASR ;0;IASL;1;IASL
          0;IMRK;0;IWPS ;0;0   ;0;0
          0;0;   0;0    ;0;ISXT;0;IRPS
          0;ICLR;1;ICLR ;0;ICOM;1;ICOM
          0;IINC;1;IINC ;0;IDEC;1;IDEC
          0;INEG;1;INEG ;0;IADC;1;IADC
          0;ISBC;1;ISBC ;0;ITST;1;ITST

RSSTAB:   IMUL;IDIV;IASH;IASC;IXOR;0   ;0   ;ISOB

SPCTAB:   0;0;    0; 60;  0; 60;  0; 60              /Нет 1, 2, 3
          IJMP;0; IJMP;0; IJMP;0; IJMP;0
          IRTS;10;0;20;   ICLE;0; ISET;0
          ISWB;0; ISWB;0; ISWB;0; ISWB;0

S0_TAB:   IHLT;IWAI;IRTI;IBPT;IIOT;IRST;IRTT;0

IMOV: <MOV >; .byte TWO_,0
ICMP: <CMP >; .byte TWO_,0
IBIT: <BIT >; .byte TWO_,0
IBIC: <BIC >; .byte TWO_,0
IBIS: <BIS >; .byte TWO_,0
IADD: <ADD >; .byte TWO_,0
ISUB: <SUB >; .byte TWO_,0
IBR: <BR  >; .byte BRU_,0
IBNE: <BNE >; .byte BRU_,0
IBEQ: <BEQ >; .byte BRU_,0
IBGE: <BGE >; .byte BRU_,0
IBLT: <BLT >; .byte BRU_,0
IBGT: <BGT >; .byte BRU_,0
IBLE: <BLE >; .byte BRU_,0
IBPL: <BPL >; .byte BRU_,0
IBMI: <BMI >; .byte BRU_,0
IBHI: <BHI >; .byte BRU_,0
IBLOS: <BLOS>; .byte BRU_,0
IBVC: <BVC >; .byte BRU_,0
IBVS: <BVS >; .byte BRU_,0
IBCC: <BCC >; .byte BRU_,0
IBCS: <BCS >; .byte BRU_,0
IEMT: <EMT >; .byte NO_,0
ITRAP: <TRAP>; .byte NO_,0
IJSR: <JSR >; .byte RSS_,0
ICLR: <CLR >; .byte ONE_,0
INEG: <NEG >; .byte ONE_,0
IDEC: <DEC >; .byte ONE_,0
IINC: <INC >; .byte ONE_,0
ICOM: <COM >; .byte ONE_,0
IADC: <ADC >; .byte ONE_,0
ISBC: <SBC >; .byte ONE_,0
ITST: <TST >; .byte ONE_,0
IROR: <ROR >; .byte ONE_,0
IROL: <ROL >; .byte ONE_,0
IASR: <ASR >; .byte ONE_,0
IASL: <ASL >; .byte ONE_,0
IMRK: <MARK>; .byte MARK_,0
ISXT: <SXT >; .byte ONE_,0
IRPS: <RPSW>; .byte ONE_,0
IWPS: <WPSW>; .byte ONE_,0
ISOB: <SOB >; .byte SOB_,0
IXOR: <XOR >; .byte RSS_,0
IMUL: <MUL >; .byte RSS_,0
IDIV: <DIV >; .byte RSS_,0
IASH: <ASH >; .byte RSS_,0
IASC: <ASHC>; .byte RSS_,0
IHLT: <HALT>; .byte NO_,0
IWAI: <WAIT>; .byte NO_,0
IRTI: <RTI >; .byte NO_,0
IBPT: <BPT >; .byte NO_,0
IIOT: <IOT >; .byte NO_,0
IRST: <RST >; .byte NO_,0
IRTT: <RTT >; .byte NO_,0
IRTS: <rts >; .byte RTS_,0
IJMP: <jmp >; .byte ONE_,0
ISWB: <SWAB>; .byte ONE_,0
ICLE: <CCLR>; .byte NO_,0
ISET: <CSET>; .byte NO_,0

  .even
WRD: .byte 0
BYT: .byte 0

SEARCH:
 mov r2,-(r6)
 mov r4,-(r6)
 mov r0,WRD
 movb BYT,r2

 mov r2,r1
 bic $177617,r1
 cmp r1,$160
 bne I_98
 tstb r2
 blt EFIND
 mov r2,r1
 bic $177761,r1
 cmp r1,$12
 beq EFIND
 cmp r1,$14
 beq EFIND

 mov RSSTAB(r1),r0
 clr r4

 br I_99
 I_98:
 tst r1
 bne I_100

 bit $010,r2
 beq I_101

 mov r2,r1
 bic $177771,r1
 cmp r1,$006
 beq EFIND

 tst r1
 bne I_102

 tstb r2
 blt I_103

 mov $IJSR,r0

 br I_104
 I_103:
 bit $001,r2
 bne I_105

 mov $IEMT,r0

 br I_104
 I_105:

 mov $ITRAP,r0

 I_104:

 clr r4

 br I_106
 I_102:

 mov r0,r1
 bic $176077,r1

 cmp r1,$500
 beq EFIND
 cmp r1,$600
 beq EFIND

 asr r1
 asr r1
 asr r1

 tst r0
 bge I_107
 bis $004,r1
 I_107:

 add $ONETAB,r1

 mov (r1)+,r4

 mov *r1,r0

 I_106:

 br I_108
 I_101:
 mov r2,r1
 bic $177560,r1
 beq I_109

 mov r0,WRD
 mov r0,r1
 bic $000377,r1
 bisb BYT,r1

 mov $6,r2
 br X_110
 X_111:

 asr r1

 X_110:
 dec r2
 bpl X_111

 bic $177741,r1
 mov BRUTAB(r1),r0
 clr r4

 br I_108
 I_109:

 mov r0,r2
 asr r2
 asr r2
 bic $177703,r2

 beq I_112

 add $[SPCTAB+2],r2
 mov *r2,r1

 bit r1,r0
 bne EFIND

 mov -(r2),r0

 br I_113
 I_112:

 cmp r0,$007
 bge EFIND

 mov r0,r1
 asl r1
 mov S0_TAB(r1),r0

 I_113:

 clr r4

 I_108:

 br I_99
 I_100:

 mov r2,r1
 bic $177417,r1
 asr r1
 asr r1
 add $TWOTAB,r1

 mov (r1)+,r4
 mov *r1,r0

 I_99:
 mov r4,r1
 mov (r6)+,r4
 mov (r6)+,r2
 rts r7

EFIND:
 clr r0
 mov (r6)+,r4
 mov (r6)+,r2
 rts r7

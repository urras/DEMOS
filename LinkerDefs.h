/*
 * LSI-11 линкер
 * Глобальные об'явления типов и константы
 */

#define loop for(;;)
#ifndef NULL
#  define NULL 0
#endif

 /* Системные определения */
#ifndef LFNAME
#  define LFNAME 10
#endif

#ifndef TRUE
#  define TRUE  1
#endif
#ifndef FALSE
#  define FALSE 0
#endif

 /* Описатель открытой библиотеки */
#define MAXLIB  16   /* Заметим, что библиотеки открыты одновременно */
typedef struct {
    char Lparent;             /* Метка об'емлющего каталога */
    char Llabel;              /* Метка файла библиотеки */
    char Lname[ LFNAME ];     /* Имя, чтобы не вызывать FLADDR */
} Library;

 /* Элемент таблицы символов линкера */
#define MAXSYMS  1000
#define SNLENG 8
typedef struct {
    char Sname[ SNLENG ];     /* Заметим, что имя дополнено НУЛЬКАМИ */
    int Stype;                /* Набор битовых полей для Ritchie */
    unsigned short Sval;
} Symbol;
 /* Маски поля Stype, аналогично записи в файле */
#define EXTERN  040
#define UNDEF    00
#define ABS      01
#define TEXT     02
#define DATA     03
#define BSS      04
% #define COMM     05  /* internal use only */
 /* Функция выделения поля сегмента, в котором определен символ */
#define LOCATION(type)  ((type)&07)
 /* Структура записи о символе в об'ектном модуле */
#define SYMNAME   0
#define SYMTYPE   SNLENG
#define SYMVAL    SNLENG+2
#define SYMSIZEOF SNLENG+4

 /* Элемент списка внешних ссылок модуля */
#define EXTPOOLSIZE (MAXMOD * 10)
typedef struct ExternQuele {
    struct ExternQuele *Enext;  /* Сцепка очереди */
    Symbol *Eptr;               /* Указатель в таблицу символов */
    short Elocnum;              /* Индекс в локальной таблице */
} ExtQue;

 /* Описатель использованного модуля */
#define MAXMOD  20   /* Размер вектора для обработанных модулей */
typedef enum { Mobjfile, Mlibrary }  Mtag;
typedef struct {
    char Mfilnam[ LFNAME ];     /* Имя файла или члена библиотеки */
    unsigned int Mseek;         /* Смещение тела модуля в файле */
    unsigned int Mldbase;       /* Смещение тела модуля в памяти */
    unsigned int Mbssoff;       /* Смещение сегмента bss в памяти */
    unsigned int Mtextsize, Mdatasize, Mbsssize, Msymsize; /*Размеры */
    ExtQue *Mbegsym;            /* Указатель на очередь глобальных */

    Mtag Mlibflag;              /* Данный модуль читаем из библиотеки */
    union {                 /* Вариантная запись под тегом Mlibflag */
        struct {
            Library *Mlibptr;
        } Mlib;
        struct {
            int Mdummy;         /* Пустых структур, увы, не бывает */
        } Mobj;
    } Mvar;
} Module;

 /* Маски информации о перемещении */
#define RABS     00
#define RTEXT    02
#define RDATA    04
#define RBSS     06
#define REXT    010
#define REF_TO  016             /* Маска сегмента размещения ссылки */
#define RPCBIT   01             /* Бит относительной адресации */
#define SYMNUM(r) (((r)>>4)&07777)  /* Номер символа в лок. таблице */

 /* Описатель выходного модуля */
typedef struct {
    unsigned int Sladdr;               /* Адрес загрузки */
    unsigned int Stlen, Sdlen, Sblen;  /* Длины сегментов */
} Statistics;

/*
 * Прототипы функций
 */

void Pass1(void);   /* Копирование модулей и чтение таблиц символов */
void Pass2(void);   /* Настройка по данным о перемещении */

void Reloc(Module*);  /* Настройка тела модуля по таблице перемещений */

void SymGet(Module*);         /* Считывание таблицы символов */

char *CFopen(void);           /* Запрос упр. файла */
char *CFget(void);            /* Чтение очередного имени */

void Tinit(void);             /* Установка счетчиков на нуль */
void Tcopy(Module*);          /* Сливание об'ектных модулей в загр. */
void Tget(Statistics*);       /* Прочесть суммарные размеры программы */
void Tseta(unsigned);         /* Задать адрес загрузки */
unsigned Tabss(void);         /* Прочесть адрес bss */

void Next0(unsigned,unsigned);/* Инициатор читалки таблиц */
Symbol *NextSymbol(void);     /* Чтение таблицы символов */

void ModIni(void);            /* Подготовка поиска */
Module *ModOpen(char*);       /* Открыть следующий модуль */
void ModSeek(unsigned);       /* Изменение указателя чтения */
void ModRead(char**,int*);    /* Заполнение буфера */
void ModClose(void);          /* Освобождение метки */
Module *ModNext(void);        /* Чтение таблицы модулей */

void ClrSym(void);
Symbol *NewSym(Symbol*);      /* Обновление таблиц символов */

void OutIni(char*);           /* Открыть выходной файл */
void OutWrite(char*,int);     /* Буферизованная запись */
unsigned OutWGet(unsigned);   /* Чтение 2-байтового слова */
void OutClose(void);          /* Переименование и освобождение метки */

void ClrES(void);
 /* Сохранение положения символа в локальной табл.*/
void AddES(Symbol*,Module*,int);
 /* Поиск локального символа по номеру */
Symbol *LookES(unsigned,Module*);

void InitDiag(void);          /* dptype */
void SetDiag(char*);          /* Задание имени файла */
void Error(int);              /* Выход с сообщением об ошибке */
void Diagnose(unsigned,char*);/* Сообщение об ошибке и возврат */
void ErrTerm(void);           /* Сообщение об общем числе ошибок */

void Pict(char*);

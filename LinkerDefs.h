/*
 * LSI-11 ������
 * ���������� ��'������� ����� � ���������
 */

#define loop for(;;)
#ifndef NULL
#  define NULL 0
#endif

 /* ��������� ����������� */
#ifndef LFNAME
#  define LFNAME 10
#endif

#ifndef TRUE
#  define TRUE  1
#endif
#ifndef FALSE
#  define FALSE 0
#endif

 /* ��������� �������� ���������� */
#define MAXLIB  16   /* �������, ��� ���������� ������� ������������ */
typedef struct {
    char Lparent;             /* ����� ��'�������� �������� */
    char Llabel;              /* ����� ����� ���������� */
    char Lname[ LFNAME ];     /* ���, ����� �� �������� FLADDR */
} Library;

 /* ������� ������� �������� ������� */
#define MAXSYMS  1000
#define SNLENG 8
typedef struct {
    char Sname[ SNLENG ];     /* �������, ��� ��� ��������� �������� */
    int Stype;                /* ����� ������� ����� ��� Ritchie */
    unsigned short Sval;
} Symbol;
 /* ����� ���� Stype, ���������� ������ � ����� */
#define EXTERN  040
#define UNDEF    00
#define ABS      01
#define TEXT     02
#define DATA     03
#define BSS      04
% #define COMM     05  /* internal use only */
 /* ������� ��������� ���� ��������, � ������� ��������� ������ */
#define LOCATION(type)  ((type)&07)
 /* ��������� ������ � ������� � ��'������ ������ */
#define SYMNAME   0
#define SYMTYPE   SNLENG
#define SYMVAL    SNLENG+2
#define SYMSIZEOF SNLENG+4

 /* ������� ������ ������� ������ ������ */
#define EXTPOOLSIZE (MAXMOD * 10)
typedef struct ExternQuele {
    struct ExternQuele *Enext;  /* ������ ������� */
    Symbol *Eptr;               /* ��������� � ������� �������� */
    short Elocnum;              /* ������ � ��������� ������� */
} ExtQue;

 /* ��������� ��������������� ������ */
#define MAXMOD  20   /* ������ ������� ��� ������������ ������� */
typedef enum { Mobjfile, Mlibrary }  Mtag;
typedef struct {
    char Mfilnam[ LFNAME ];     /* ��� ����� ��� ����� ���������� */
    unsigned int Mseek;         /* �������� ���� ������ � ����� */
    unsigned int Mldbase;       /* �������� ���� ������ � ������ */
    unsigned int Mbssoff;       /* �������� �������� bss � ������ */
    unsigned int Mtextsize, Mdatasize, Mbsssize, Msymsize; /*������� */
    ExtQue *Mbegsym;            /* ��������� �� ������� ���������� */

    Mtag Mlibflag;              /* ������ ������ ������ �� ���������� */
    union {                 /* ���������� ������ ��� ����� Mlibflag */
        struct {
            Library *Mlibptr;
        } Mlib;
        struct {
            int Mdummy;         /* ������ ��������, ���, �� ������ */
        } Mobj;
    } Mvar;
} Module;

 /* ����� ���������� � ����������� */
#define RABS     00
#define RTEXT    02
#define RDATA    04
#define RBSS     06
#define REXT    010
#define REF_TO  016             /* ����� �������� ���������� ������ */
#define RPCBIT   01             /* ��� ������������� ��������� */
#define SYMNUM(r) (((r)>>4)&07777)  /* ����� ������� � ���. ������� */

 /* ��������� ��������� ������ */
typedef struct {
    unsigned int Sladdr;               /* ����� �������� */
    unsigned int Stlen, Sdlen, Sblen;  /* ����� ��������� */
} Statistics;

/*
 * ��������� �������
 */

void Pass1(void);   /* ����������� ������� � ������ ������ �������� */
void Pass2(void);   /* ��������� �� ������ � ����������� */

void Reloc(Module*);  /* ��������� ���� ������ �� ������� ����������� */

void SymGet(Module*);         /* ���������� ������� �������� */

char *CFopen(void);           /* ������ ���. ����� */
char *CFget(void);            /* ������ ���������� ����� */

void Tinit(void);             /* ��������� ��������� �� ���� */
void Tcopy(Module*);          /* �������� ��'������ ������� � ����. */
void Tget(Statistics*);       /* �������� ��������� ������� ��������� */
void Tseta(unsigned);         /* ������ ����� �������� */
unsigned Tabss(void);         /* �������� ����� bss */

void Next0(unsigned,unsigned);/* ��������� ������� ������ */
Symbol *NextSymbol(void);     /* ������ ������� �������� */

void ModIni(void);            /* ���������� ������ */
Module *ModOpen(char*);       /* ������� ��������� ������ */
void ModSeek(unsigned);       /* ��������� ��������� ������ */
void ModRead(char**,int*);    /* ���������� ������ */
void ModClose(void);          /* ������������ ����� */
Module *ModNext(void);        /* ������ ������� ������� */

void ClrSym(void);
Symbol *NewSym(Symbol*);      /* ���������� ������ �������� */

void OutIni(char*);           /* ������� �������� ���� */
void OutWrite(char*,int);     /* �������������� ������ */
unsigned OutWGet(unsigned);   /* ������ 2-��������� ����� */
void OutClose(void);          /* �������������� � ������������ ����� */

void ClrES(void);
 /* ���������� ��������� ������� � ��������� ����.*/
void AddES(Symbol*,Module*,int);
 /* ����� ���������� ������� �� ������ */
Symbol *LookES(unsigned,Module*);

void InitDiag(void);          /* dptype */
void SetDiag(char*);          /* ������� ����� ����� */
void Error(int);              /* ����� � ���������� �� ������ */
void Diagnose(unsigned,char*);/* ��������� �� ������ � ������� */
void ErrTerm(void);           /* ��������� �� ����� ����� ������ */

void Pict(char*);

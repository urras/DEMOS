/*
 * Ассемблер  вер. 1.0
 *    ЭВМ СМ-4   МНОС РЛ 1.2
 *    Автор: Антонов В.Г. ( ВМК МГУ, ИПК Минавтопрома )
 *    Последнее изменение: 09.12.1984
 *
 *    Sun-4/65   SunOS 4.1.2
 *    zaitcev
 *    07/21/93
 */
#ifndef R_11
#define  recursive
#endif

/*
 * Параметры ассемблера
 */
#define MAXTABSYM  1300
#define MAXSTRING  512
#define DLABS      1000
#define MAXUBR     1000
#define UBROPT     0

#ifndef EOF
#define EOF (-1)
#endif

/*
 * Таблица символов
 */
struct STAB {
    char    name[8];
    int     type;
    unsigned value;
};

/*
 * Классы лексем
 */
#define LEX_ERR       (-1)
#define LEX_STR         0
#define LEX_NUM         1
#define LEX_LID         2
#define LEX_LDIG        3
#define LEX_EOL         4
#define LEX_EOF         5
#define LEX_TEXT        6
#define LEX_BSS         7
#define LEX_DATA        8
#define LEX_EVEN        9
#define LEX_BYTE       10
#define LEX_COMM       11
#define LEX_DOT        12
#define LEX_DOT2       13
#define LEX_GLOBL      14
#define LEX_NAME       15
#define LEX_COP        16
#define LEX_IF         17
#define LEX_EIF        18
#define LEX_UDL        19
#define LS_STAR        20
#define LS_PLUS        21
#define LS_MINUS       22
#define LS_EQ          23
#define LS_OPAR        24
#define LS_CPAR        25
#define LS_OBRA        26
#define LS_CBRA        27
#define LS_NOT         28
#define LS_AND         29
#define LS_COMMA       30
#define LS_DOLLAR      31
#define LS_STYPE       32
#define LS_RSHIFT      33
#define LS_LSHIFT      34
#define LS_DIV         35
#define LS_OR          36
#define LS_MOD         37
#define LR_SKIP        38
#define LR_COM         39
#define LR_BYTE        40
#define LEX_LDT        41
#define LEX_LDD        42
#define LEX_LDB        43

/*
 * Типы символов
 */
#define Not_found       (-1)

#define EXTERN   040
#define UNDEF    0
#define ABS      1
#define TEXT     2
#define DATA     3
#define BSS      4
#define INVERTED 010

/*
 * Биты перемещаемости
 */
#define REL_PC  01
#define RABS    0
#define RTEXT   02
#define RDATA   04
#define RBSS    06
#define REXTRN  010
#define RNSYM( k )  ((k)<<4)

/*
 * Типы команд
 */
#define ISX     07600
#define XABS        0
#define XFRD     0200
#define XBR      0400
#define XJSR     0600
#define XSR     01000
#define XSYS    01200
#define XMVF    01400
#define XDOP    01600
#define XFSR    02000
#define XSOP    02200
#define XREG    02400
#define XEIS    02600
#define XSOB    03000
#define XJBR    03200
#define XJBC    03400
#define XNOP    03600
#define XPC     04000
#define XDDD    04200
#define XXOR    04400
#define XSOS    04600

#define JMPCODE 0000100
#define NOBR    0400
#define STFCODE 0174000
#define LDFCODE 0172400

/*
 * Возврат лексемы на шаг
 */
extern  blflg;
#define backlex blflg++

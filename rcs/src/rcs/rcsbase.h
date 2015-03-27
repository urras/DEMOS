
/*
 *             ����� ����������� � ��������� ������ RCS
 */
#define RCSBASE "$Header: /usr/users/korotay/DEMOS_2.2/rcs/src/rcs/RCS/rcsbase.h,v 1.4 1988/09/21 20:37:38 avg Exp $"
/****************************************************************************
 * ����������:
 * ===========
 * ��� ��� ���� �����������-82, ������� #define �_82. � ���� ������
 * ����� ������������ ������ ������� getwd() � rename() ��
 * ����������� �� ���������� C. ����� ����, ����� E_82 ���������
 * �������� ������������� ����� ������ � ������� � ������ ������ ������
 * (����� NCPFN � NCPPN).
 * ����� STRICT_LOCKING ������ ����� ��������� ���������� �����
 * �� ��������� (��. STRICT_LOCKING).
 * ��� ������������� ����� �������� ������ ����� (����� BYTESIZ).
 ****************************************************************************
 */


/* $Log: rcsbase.h,v $
 * Revision 1.4  1988/09/21  20:37:38  avg
 * �������� STRICT_LOCKING. �������!
 *
 * Revision 1.3  86/02/23  22:23:22  avg
 * ������� ��������� ��� ���� � ��� ������ � U-�����.
 *
 * Revision 1.2  86/01/02  15:50:52  fox
 * ������� E_82 � �������� ����� ����� ��� �����
 * ����������� � ���������� VAX.
 *
 * Revision 1.1  85/12/26  22:02:36  fox
 * ������� �������� ������ ������
 *
 */

#include <stdio.h>
#undef putc         /* ����� ���������������� */

/* P3 */
#ifdef sparc
#define E82
#endif

/* ��������� ����� �� ��������� ����� DATEFORM ��������
 * ���������� ����� � ����������� ������ (00, 01 ...).
 * ���� ��� ����� ������� �� �����, ����� ���������� ��������
 * ��� ������ � ������.
 */
#ifdef USG
#       define rindex    strrchr
#       define DATEFORM  "%.2d.%.2d.%.2d.%.2d.%.2d.%.2d"
#else
#       define DATEFORM  "%02d.%02d.%02d.%02d.%02d.%02d"
#endif

/* ������ � ������ file ���� � ������� � ������� RCS                        */
#define PRINTDATE(file,date) fprintf(file,"%.2s/%.2s/%.2s",date,date+3,date+6)
#define PRINTTIME(file,date) fprintf(file,"%.2s:%.2s:%.2s",date+9,date+12,date+15)

/*
 * ���������
 */
#define BYTESIZ             8 /* ����� ����� � �����                        */

#define STRICT_LOCKING      0 /* 0 ��������� �������� ���������� ��         */
			      /* ���������, ���� ����� ������������         */
			      /* � ���������� �������.                      */
			      /* 1 ����� ��������� ���������� ��            */
			      /* ���������.                                 */
#define hshsize           119 /* ������ hash-�������; hshsize = 4*k -1      */
			      /* ��������: 547 ��� 719                      */
#define strtsize (hshsize * 30) /* ������ ������� �����                     */
#define logsize           512 /* ���. ������ �����������                    */
#define revlength          30 /* ���. ����� ������ ������                   */
#define datelength         20 /* ����� ���� � ������� RCS                   */
#define joinlength         20 /* ����� ��������� ������ ����� -j            */
#define RCSDIR         "RCS/" /* ��� ��������������  ��� ������ RCS         */
#define RCSSUF            'v' /* ������� �������� ������ RCS                */
#define RCSSEP            ',' /* ����������� ��� RCSSUF                     */
#define KDELIM            '$' /* ������ ��� �������� ����������             */
#define VDELIM            ':' /* ����������� ��������� ����� � ��������     */
#define DEFAULTSTATE    "Exp" /* ������ ������ �� ���������                 */
#ifdef E_82
#  define NCPFN           256 /* ����. ����� ������ � ����� �����           */
#  define NCPPN          1024 /* ����. ����� ������ � ������ ����� �����    */
#else
#  define NCPFN            14 /* ����. ����� ������ � ����� �����           */
#  define NCPPN       6*NCPFN /* ����. ����� ������ � ������ ����� �����    */
#endif
#define keylength          20 /* ������ ������ ��� ���������� ��. ����������*/
#define keyvallength NCPPN+revlength+datelength+60
			      /* ������ ������ ��� �������� ��. ���������   */



#define true     1
#define false    0
#define nil      0
#define elsif    else if
#define elif     else if


/* ����� ��������� ������ */

#define NEWRCSFILE  ",RCSnewXXXXXX"
#define DIFFILE     ",RCSciXXXXXX"
#define TMPFILE1    ",RCSt1XXXXXX"
#define TMPFILE2    ",RCSt2XXXXXX"
#define TMPFILE3    ",RCSt3XXXXXX"
#define JOINFIL2    ",RCSj2XXXXXX"
#define JOINFIL3    ",RCSj3XXXXXX"


/* ������ putc. �������� ������, �� �������� ��� ������� ������             */
int     _cc_;
#define putc(x,p) (--(p)->_cnt>=0?((*(p)->_ptr++=(_cc_=(x))?_cc_:0)):fflsbuf((x),p))

/* GETC ��������� ������ del (��� 0177) � ����� �����                       */
#define GETC(in,out,echo) (echo?putc(getc(in),out):getc(in))

/* ������� ����� ������� �������� ����� �� RCSmod, ��� �������������
��������� ���������� ������ (� ����������� �� lockflag � StrictLocks)       */
#define WORKMODE(RCSmode) (RCSmode&~0222)|((lockflag||!StrictLocks)?0600:0000)


/* ������ �������� � ���� ������ */
enum tokens {
/* ������ ��������*/ DIGIT, IDCHAR, NEWLN, LETTER, PERIOD, SBEGIN, SPACE, UNKN,
/* ������� */       COLON, DATE, EOFILE, ID, KEYW, NUM, SEMI, STRING,
};

/* ����� SBEGIN (������ ������) ����������� ����������� ������������        */
#define AT      SBEGIN

/* ������ ����������� ��� ��������� ����� */
#define SDELIM  '@'

/* ����. ������� ����� ����������, ��������:
 * #define DQUOTE       SBEGIN
 * #define SDELIM       '"'
 * #define AT           IDCHAR
 * ��� �� ������ ������������ � SDELIM, KDELIM � VDELIM
 */

/* ������ ������� */

#define ACCENT   IDCHAR
#define AMPER    IDCHAR
#define BACKSL   IDCHAR
#define BAR      IDCHAR
#define COMMA    UNKN
#define DIVIDE   IDCHAR
#define DOLLAR   IDCHAR
#define DQUOTE   IDCHAR
#define EQUAL    IDCHAR
#define EXCLA    IDCHAR
#define GREAT    IDCHAR
#define HASH     IDCHAR
#define INSERT   UNKN
#define LBRACE   IDCHAR
#define LBRACK   IDCHAR
#define LESS     IDCHAR
#define LPARN    IDCHAR
#define MINUS    IDCHAR
#define PERCNT   IDCHAR
#define PLUS     IDCHAR
#define QUEST    IDCHAR
#define RBRACE   IDCHAR
#define RBRACK   IDCHAR
#define RPARN    IDCHAR
#define SQUOTE   IDCHAR
#define TILDE    IDCHAR
#define TIMES    IDCHAR
#define UNDER    IDCHAR
#define UPARR    IDCHAR




/****************************************
 * ��������� ������ ��� ������� ��������
 ****************************************/


/* ��������� hash ������� */
struct hshentry {
	char              * num;      /* ��������� �� ����� ������ (� ���-8)*/
	char              * date;     /* ��������� �� ���� ������           */
	char              * author;   /* id ����������� ������������        */
	char              * lockedby; /* ��� �������������� ������          */
	char              * log;      /* �����������, ���������� ��� ������ */
	char              * state;    /* ������ ������ (�� ��������� Exp)   */
	struct branchhead * branches; /* ������ ������ ������ ������        */
	struct hshentry   * next;     /* ��������� ������ ���� �����        */
	int                 insertlns;/* ����� ����������� ����� (���. rlog)*/
	int                 deletelns;/* ����� ����������� (��������� rlog) */
	char                selector; /* ����� ��� selection/deletion       */
};

/* ������ ��������� ��� ������ ������ */
struct branchhead {
        struct hshentry   * hsh;
        struct branchhead * nextbranch;
};

/* ������� ������ ������� */
struct access {
        char              * login;
        struct access     * nextaccess;
};

/* ������� ������ ��� ���������� */
struct lock {
        char              * login;
        struct hshentry   * delta;
        struct lock       * nextlock;
};

/* ������� ������ ���������� ���� */
struct assoc {
        char              * symbol;
        struct hshentry   * delta;
        struct assoc      * nextassoc;
};


/* ����� ���������� (getadmin � getdelta())*/
extern char            * Comment;
extern struct access   * AccessList;
extern struct assoc    * Symbols;
extern struct lock     * Locks;
extern struct hshentry * Head;
extern int               StrictLocks;
extern int               TotalDeltas;

/* ����� ���������� (����������� ����������)*/
extern enum tokens map[];
#define ctab (&map[1])
extern struct hshentry   hshtab[];
extern struct hshentry * nexthsh;
extern enum tokens       nexttok;
extern int               hshenter;
extern char            * NextString;
extern char            * cmdid;

/* ����� ������� */
extern int serror();
extern int faterror();
extern int fatserror();

/*
 * �������� ��������� (������������ � co � ident)
 */
#define AUTHOR          "Author"
#define DATE            "Date"
#define HEADER          "Header"
#define LOCKER          "Locker"
#define LOG             "Log"
#define REVISION        "Revision"
#define SOURCE          "Source"
#define STATE           "State"

enum markers { Nomatch, Author, Date, Header,
               Locker, Log, Revision, Source, State };

/* ������������ � putdtext � scanlogtext */
#define DELNUMFORM      "\n\n%s\n%s\n"

/* ��������������� rcs -o, ������������ puttree() � rcssyn */
#define DELETE          'D'


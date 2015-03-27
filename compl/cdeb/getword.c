/* === CDEBUGGER === ��������� ��������� � ������������ ���������� */

/* ��������� ����������� � ������ ������.

   ����� �����:
        get_word(�����)         - ������ ����� �� ��������� ������������
                                  ������������ ������
        get_iword(�����)        - ������ ����� �� ��������� ������������
                                  ������������ ������, ������� ������
                                  (������ � ������ "SEPID")
        set_word(�����,��������)- ������ ����� � �������� ������������
                                  ������������ ������
        init_task()     - ��������� ��������� ��������� ����� ������
                          ������� ������������ ������
        run_task()      - ������ ������ � ��������� ���������� ��������� �
                          ������� �����. ������� � ����� stp_adr,
                          �� ��������� ������� ��� �� �������, � �����. ��
                          stp_flag. �� �������� ��������� ���������
                          ������ �� ������ �������� � ��������� �������. */

/* #define DEB /* */

#include "cdeb.h"
#include <signal.h>

int  sregval[9];
int  pid;                               /* ������������� �������. ������ */


/* ������� � ���������� ������ PTRACE */
#define SETTRC  0                       /* ��'�����, ��� ���� ���������� */
#define RUSER   2                       /* ������ �� "data" ������ */
#define RIUSER  1                       /* ������ �� "text" ������ */
#define WUSER   5                       /* ������ �  "data" ������ */
#define RUREGS  3                       /* ������ �� ����. ���. ������   */
#define WUREGS  6                       /* ������ �  ����. ���. ������   */
#define CONTIN  7                       /* ������ ������                 */
#define EXIT    8                       /* ���������� ������������ ������*/


/* ������������ ��������� � ��������� ������� ������ */
#define PS      -1
#define PC      -2
#define SP      -6
#define R5      -9
#define R4      -10
#define R3      -11
#define R2      -12
#define R1      -5
#define R0      -3
#define a(rr)   2*(512+rr)
int regadr[] = {a(R0),a(R1),a(R2),a(R3),a(R4),a(R5),a(SP),a(PC),a(PS)};

int _exectrap;


/* ������ � ������������� ������������ ������ */

init_task(argc, argv)
    char **argv;  int  argc;
{
#   define T_bit   020
    int onintr();
    int stat, w;
    int Ruid, Rgid;                     /* ��� ��������� ����������      */

    if( argc < 2 ) fatal_error( 5 );    /* �������� ��������� DEBUG      */
    signal(SIGQUIT,SIG_IGN);            /* ���������  ctrl-\ ���������   */

#ifdef ����
    Ruid = getuid()&0377;
    Rgid = getgid()&0377;
#else
    Ruid = getuid();
    Rgid = getgid();
#endif

    pid = fork();                       /* ��������� �����. ������       */
    if( !pid ) {                        /* �������-��� -> ��� ���������� */
        setuid(Ruid); setgid(Rgid);
        signal(SIGQUIT,SIG_DFL);        /* ��������� ctrl-\              */
        _exectrap = 1;
        ptrace(SETTRC, 0, 0, 0);
        execv( argv[1], &argv[1] );     /* ��������� �����. ������       */
        fatal_error( 6 );
    }

    signal(SIGINT, onintr);             /* ���������  ctrl-C �           */
    signal(SIGQUIT,onintr);             /*            ctrl-\ ���������   */

    nice(-5);                           /* ��������� ���������� ���������*/
    setuid(Ruid); setgid(Rgid);

    while( (w = wait(&stat)) != pid  &&  w != -1 );  /* ���� ��������    */
    if( w == -1 || ((stat>>8)&0177) != SIGTRAP )     /* ��������� ������ */
        fatal_error( 2 );
    regval[8] = (~T_bit) & ptrace( RUREGS, pid, regadr[8], 0);
    if( errno ) fatal_error( 7 );
    set_word( CDEBFLAG, 1 );            /* ������ ���� ������� � 'cdebug'*/

    stp_adr = 0;  run_task();           /* ������ �� 1-�� ������. ������ */
}


get_reg_value()                         /* ��������� �������� ���������  */
{                                       /* � ����� �������� (���., ���.) */
    register int i;
    errno = 0;
    for( i=0; i<8; ++i)
        sregval[i] = regval[i] = get_word( CDEBFLAG - 16 + 2*i );
    regval[6] += 2;
    sregval[7] = 0;
    act_adr    = regval[7];
    act_line   = get_iword( act_adr-2 );

#ifdef DEB
printf("act_line=%06o \n",act_line);
printf("from=%06o leng=%06o value=%06o\n", get_word( CDEBFLAG+FROM ),
get_word( CDEBFLAG+LENG ), get_word( CDEBFLAG+VALUE ) );
#endif

    language   = act_line>>14;
    act_line  &= (1<<14)-1;
}


run_task()
{
    register int i, w;
    int stat;
    static int iniflag=0, setregflag=0;

    errno = 0;                      /* ������������� ���������� �������� */
    for(i=0; i<9; ++i) {            /*          (������ pc � sp)         */
        if( setregflag  ||  regval[i] != sregval[i] ) {
            ptrace( WUREGS, pid, regadr[i], regval[i] );
            sregval[i] = regval[i];
        }
    }
    if( errno )  fatal_error( 2 );
    setregflag=0;

    set_word( CDEBFLAG+STPADR , stp_adr );      /* ������ ����� �������� */
    if( !jsr_ign )                              /* �� ������� � �������  */
        set_word( CDEBFLAG+PFRAME, regval[5] );
    else if( jsr_ign < 0 )                      /* ����� �� �������      */
        set_word( CDEBFLAG+PFRAME, regval[5]+1 );
    else                                        /* ������� � ��������    */
        set_word( CDEBFLAG+PFRAME, 0 );

    errno = 0;

#ifdef DEB
printf("cdebflag=%06o pframe=%06o stpadr=%06o\n", get_word( CDEBFLAG ),
get_word( CDEBFLAG+PFRAME ), get_word( CDEBFLAG+STPADR ) );
#endif

    ptrace(CONTIN, pid, 1, 0);                      /* ��������� ������� */
    if( errno )  fatal_error( 2 );
    while( (w = wait(&stat)) != pid  &&  w != -1 );    /* � ���� ������� */
    if( w == -1  ) fatal_error( 2 );

    get_reg_value();
    stp_cause = (stat>>8)&0177;                      /* ������� �������� */
    if( stp_cause == SIGTRAP )
        stp_cause = 0;                               /*      ���� ������ */
    else
        setregflag = 1;                              /*      �������     */

    set_word( CDEBFLAG+FROM, 0 );

    look_file(act_adr);
}

#ifdef SEPID
get_iword(adr)
{
    register int word;
    errno=0;
    word = ptrace(RIUSER, pid, adr, 0);
    if( errno ) {
        errno = 0;                               /* ������ ����������� ? */
        ptrace(RUSER, pid, 0, 0);
        if( errno )  exitdeb();                          /* ��  */
        error( 6 );                                      /* ��� */
    }
    return( word );
}
#endif

get_word(adr)
{
    register int word;
    errno=0;
    word = ptrace(RUSER, pid, adr, 0);
    if( errno ) {
        errno = 0;                               /* ������ ����������� ? */
        ptrace(RUSER, pid, 0, 0);
        if( errno )  exitdeb();                          /* ��  */
        error( 6 );                                      /* ��� */
    }
    return( word );
}


set_word(adr,value)
{
    errno=0;
    ptrace( WUSER, pid, adr, value );
    if( errno )  error( 9 );
}

/*
 * Ассемблер  вер. 1.0
 *    ЭВМ СМ-4   МНОС РЛ 1.2
 *    Автор: Антонов В.Г. ( ВМК МГУ, ИПК Минавтопрома )
 *    Последнее изменение: 09.02.1985
 *      26.10.84 Володин и Леонтьев:
 *              реакция на SIGINT и SIGQUIT сделана такой,
 *              какой ей положено быть Богом.
 *
 *    ЭВМ Митра-225  MISS
 *      28.10.88 Зайцев
 *              Выброшен стандартный ввод-вывод (из-за ошибок в нем)
 *    ЭВМ IBM PC  MISS
 *      22.05.92 Зайцев
 *              Приведение к новым (переносимым) заголовкам.
 *      23.05.92 Зайцев
 *              Stdio вернулся обратно, так как на PC он работает
 *              вполне нормально.
 *
 *    Sun-4/65   SunOS 4.1.2
 *    zaitcev
 *    07/21/1993
 */

#include <stdlib.h>           /* exit() */
#include <stdio.h>
#include <signal.h>
#include <ediag.h>
#include "as.h"

#define DFN     "/tmp/natmp1xxxxxx"
#define TFN     "/tmp/natmp2xxxxxx"
#define RFN     "/tmp/natmp3xxxxxx"
#define AOUT    "a.out"
#define MARGS   40

char    LexL[] = {              /* Таблица длин пром. представлений лексем */
    3, 3, 0, 0, 3, 1, 0, 0, 0, 0,   /*  0 -  9 */
    0, 0, 1, 1, 0, 3, 3, 0, 0, 3,   /* 10 - 19 */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   /* 20 - 29 */
    1, 1, 1, 1, 1, 1, 1, 1, 3, 5,   /* 30 - 39 */
    1, 3, 3, 3                      /* 40 - 43 */
};
FILE    *infile;
FILE    *textfile;
FILE    *datafile;
FILE    *outfile;
FILE    *relocf;
char    *ofn, *tfn, *dfn, *refname;
int     ARGC;           /* Копии argv и argc */
char    **ARGV;
int     arglines[MARGS];
int     wasop = 0;      /* Флаг -- 0 - вх. файл не открыт
                   1 - вх. файл открыт
                   2 - вх. файл = ст.ввод */
int     cfil = 1;       /* Тек. аргумент */

int     errcnt = 0;     /* Счетчик ошибок */
char    success = 0;    /* Флаг успеха */
int     uflag = 0;      /* Если установлен, то все undefined
                           заменяет на external undefined */

void pass1();
void ubrs();
void pass2();
void wrsymtab();

#if 0
/*********** ЗАТЫЧКИ ДЛЯ stdio ************/
char    *_ibuf;
char    *malloc()  { return(NULL); }
char    *realloc() { return(NULL); }
     free()    { }
     isatty()  { return(0); }
char _sibuf, _sobuf;
struct  _iobuf  _iob[_NFILE] = {
    {NULL, 0, NULL, _IOREAD+_IONBF, 0},
    {NULL, 0, NULL, _IOWRT+_IONBF, 1},
    {NULL, 0, NULL, _IOWRT+_IONBF, 2},
};
struct  _iobuf  *_lastbuf = { &_iob[_NFILE] };
/******************************************/
#else
char    *_ibuf;
#endif


/*
 * Главная программа - открывает файлы
 */
main( argc, argv )
char    **argv;
{
    int     acnt;
    extern int secnd;
    int     END();
    char   *p;
    char    IBUF[BUFSIZ], TBUF[BUFSIZ], DBUF[BUFSIZ],
        RBUF[BUFSIZ], OBUF[BUFSIZ];

    ARGC = argc;
    ARGV = argv;
    if( argc == 1 ) {
           msg( ediag( "Usage: % [-s] [-] file... [-o outfile]",
               "Вызов: % [-s] [-] исх.файл... [-o вых.файл]" ), argv[0] );
           exit( 1 );
    }
    _ibuf = IBUF;
    if( argc >= MARGS ) {
        msg( ediag( "Too many files.\n",
               "Слишком много файлов.\n" ), 0 );
        exit( 1 );
    }
    if( signal( SIGINT, SIG_IGN ) != SIG_IGN )
        signal( SIGINT, END ) ;
    if( signal( SIGQUIT, SIG_IGN ) != SIG_IGN )
        signal( SIGQUIT, END ) ;
    ofn = NULL;
    infile = NULL;
    acnt = 0;
    while( ++acnt < argc ) {
        if( sequal( argv[acnt], "-o" ) ) {
            if( ofn != NULL ) {
                msg( ediag( "The outfile mentioned twice.",
                       "Выходной файл задан дважды." ), 0 );
                exit( 1 );
            } else if( acnt < argc-1 )
                ofn = argv[++acnt];
            else {
                msg( ediag( "Bad outfile name.",
                       "Плохое имя выходного файла." ), 0 );
                exit( 1 );
            }
        } else if( sequal( argv[acnt], "-" ) || sequal( argv[acnt], "-u" ) ) {
            /* P3: в разных вариантах cc встречаются разные варианты -u. */
            uflag++;
        }
    }

    acnt = getpid();
    tfn = TFN;
    dfn = DFN;
    refname = RFN;
    tfn[16] = dfn[16] = refname[16] = (acnt & 07) + '0' ; acnt >>= 3;
    tfn[15] = dfn[15] = refname[15] = (acnt & 07) + '0' ; acnt >>= 3;
    tfn[14] = dfn[14] = refname[14] = (acnt & 07) + '0' ; acnt >>= 3;
    tfn[13] = dfn[13] = refname[13] = (acnt & 07) + '0' ; acnt >>= 3;
    tfn[12] = dfn[12] = refname[12] = (acnt & 07) + '0' ; acnt >>= 3;
    tfn[11] = dfn[11] = refname[11] = (acnt & 07) + '0' ;

    if( (textfile = fopen( tfn,     "w+" )) == NULL ||
        (datafile = fopen( dfn,     "w+" )) == NULL ||
        (relocf   = fopen( refname, "w+" )) == NULL ) {
        msg( ediag( "Can't create temp files.",
               "Нельзя создать рабочие файлы." ), 0 );
        exit( 1 );
    }
    setbuf( textfile, TBUF );
    setbuf( datafile, DBUF );
    setbuf( relocf,   RBUF );

#ifdef MSGS
    fputs( stderr, ediag( "  First pass\n",
                 "  Первый проход\n" ), stderr );
#endif MSGS
    arglines[0]++;
    pass1();
    if( errcnt )
        END();
#ifdef MSGS
    fputs( ediag( "  Table processing\n",
             "  Обработка таблиц\n" ), stderr );
#endif MSGS
    pass15();
    if( errcnt )
        END();

#ifdef MSGS
    fputs( ediag( "  Second pass\n",
             "  Второй проход\n" ), stderr );
#endif MSGS
    fseek( textfile, 0l, 0 );
    fseek( datafile, 0l, 0 );
    if( ofn == NULL )
        ofn = AOUT;
    unlink( ofn );
    if( (outfile = fopen( ofn, "w" )) == NULL ) {
        msg( ediag( "Can't create %.",
               "Нельзя создать %." ), ofn );
        END();
    }
    setbuf( outfile, OBUF );
    secnd++;
    wrhead();
    pass2( textfile );
    pass2( datafile );
    wrsymtab();
    if( !errcnt )
        success++;
    END();
}

/*
 * Открыть вх. файл
 */
FILE *
openinf()
{
    extern int linecnt;

    switch( wasop ) {
    case 1:
        fclose( infile );
    case 2:
        wasop = 0;
        break;
    }
TRY:
    if( cfil >= ARGC )
        return( NULL );
    if( sequal( ARGV[cfil], "-o" ) ) {
        arglines[cfil++] = linecnt;
        arglines[cfil++] = linecnt;
        goto TRY;
    }
    if( sequal( ARGV[cfil], "-" ) || sequal( ARGV[cfil], "-u" ) ) {
        arglines[cfil++] = linecnt;
        goto TRY;
    }
    if( sequal( ARGV[cfil], "-s" ) ) {
        arglines[cfil++] = linecnt;
        wasop = 2;
        return( infile = stdin );
    }
    if( (infile = fopen( ARGV[cfil], "r" )) == NULL ) {
        msg( ediag( "%: can't open.",
               "%: нельзя открыть." ), ARGV[cfil] );
        END();
    }
    setbuf( infile, _ibuf );
    arglines[cfil++] = linecnt;
    wasop++;
    return( infile );
}

/*
 * Выход из ассемблера
 */
END()
{
    unlink( tfn );
    unlink( dfn );
    unlink( refname );
    if( success ) {
#ifdef MSGS
        fputs( "  OK\n", stderr );
#endif MSGS
        exit( 0 );
    }
    unlink( ofn );
    exit( 1 );
}

/*
 * Запись лексемы в файл промежуточного представления
 */
wrilex( cl, cv )
int cl, cv;
{
    extern int Segflg;
    register FILE *f;

    switch( Segflg ) {
    case 0 :
        f = textfile;
        break;
    case 1 :
        f = datafile;
        break;
    default :
        msg( "IE02", 0 );
        exit( 1 );
    }
    putc( cl, f );
    if( LexL[cl] > 1 ) {
        putc( cv, f );
        putc( cv >> 8, f );
    }
}

/*
 * Запись константы в промежуточное представление
 */
wricons( cn, len )
char    *cn;
{
    extern int Segflg;

    if( fwrite( cn, 1,len, Segflg? datafile: textfile ) < len )
        END();   /* MISS ErrDiskFull removed -> hope that Unix tells us */
}

putcons( datum )
    int datum;
{
    extern int Segflg;
    FILE *f;

    putc( (char)datum, f = Segflg? datafile: textfile );
    if( ferror(f) ) END();
}

/*
 * Печать диагностического сообщения
 */
msg( xmsg, arg )
char    *xmsg;
char    *arg;
{
    extern int linecnt;
    char    lnum[5], *q, *r;
    register int i, j;
    int     argn;

    if( !(*arglines) )
        goto BMS;
    i = linecnt;
    argn = 0;
    while( i > (j = arglines[++argn]) && j != 0 );
    if( j == 0 )
        j = arglines[--argn];
    else {
        while( j == arglines[argn] )
            argn++;
        argn--;
    }
    i -= j;
    i++;
    for( q = ARGV[argn] ; *q ; q++ )
        putc( *q, stderr );
    putc( ':', stderr );

    j = 4;
    do {
        lnum[j--] = '0' + (i%10);
        i = i/10;
    } while( i > 0 );
    while( ++j < 5 )
        putc( lnum[j], stderr );
    putc( ':', stderr );
    putc( ' ', stderr );
BMS:
    for( q = xmsg ; *q ; q++ ) {
        if( *q == '%' )
            for( r = arg ; *r ; r++ )
                putc( *r, stderr );
        else
            putc( *q, stderr );
    }
    putc( '\n', stderr );
    errcnt++;
}

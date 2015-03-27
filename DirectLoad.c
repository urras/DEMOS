/**
 ** Загрузка программы в LSI-11 через пультовой дисплей
 **/
#include <Display>
#include <SysCalls>           /* _exit_ */
#include <SysStrings>         /* MVS(), _cmps() */
#include <SysConv>            /* _conv() */
#include <FileStruct>         /* FileItem* */
#include <FileSystem>         /* GetFileItem() */
#include <Librarian>          /* HidBit */
#define LIBASK 0
#include <StartOpers>         /* StartLibr() */
#include <MemSystem>          /* ExpandStorage() */
#include <FileIO>             /* _io_ */
#include <SysErrors>          /* ErrDevFail */
#include <cioDef>

 /* Описатель загрузочного модуля */
struct module {
    unshort leng, lbuf, base, pc0;
};

void OpenImage( char*, struct module* );
void GetDevice( void );
void InitDial( void );
void LoadImage( char**, char, struct module* );
void SendImage( char*, struct module*, int );
void SetAddr( unshort );
int PutWord( unshort );
% void Run( struct module* );
void Error( int );
void InpFlush( short );

cdevice cline;

#ifdef i_86
#pragma STARTER(), EXIT();
#endif
#ifdef R_11
#pragma STARTER(NULL);
#endif

main( scode )
    int scode;
{
    char imglab;
    struct module imgpar;
    char *buff;
    int check = (scode != '!' && scode != PR);

    OpenImage( &imglab, &imgpar );      /* Узнаем, что и куда грузить */
    GetDevice();                        /* "Line:0"                   */
    InitDial();                         /* Получаем первую лягушку    */
    LoadImage( &buff,imglab,&imgpar );  /* Грузим в память            */
    SendImage( buff, &imgpar, check );  /* Передаем в LSI-11          */
%%  if( scode != '!' ) Run( &imgpar );  /* Запускаем программу        */
    dpc( "\f\n\nO.K." );    dpq( '_' );
    _exit_(0);
}

/*
 * Запрос пользователя на предмет файла для загрузки
 */
void OpenImage( plab, pdesc )
    char *plab;
    struct module *pdesc;
{
    static char pdp_11[8] = { 0x02,0x23,0x61,0xF8,0x0C,0xD0,0,0 };
    int rc;
    int lab;
    int lcnt;
    char *fp, *s;
    FileItem *ip;

    fp = AZC;
    *fp++ = 'I';
    *fp++ = HidBit;
    *fp++ = 0;                          /* Место под количество файлов*/
    *fp++ = 1;                          /* Выбираем один файл         */
    lcnt = 0;
    for( lab = 1; lab < 0x7F; lab++ ){
        if( (ip = GetFileItem( lab )) != NULL &&
             (ip->type&0x7F) == 'I' &&
             _cmps( (char*)&ip->processor, pdp_11, 8 ) == 0 )
        {
            if( lcnt > 90 ) break;      /* Чтобы текст поместился     */
            lcnt++;
            *fp++ = (char)lab;
        }
    }
    AZC[2] = (char)lcnt;
    MVS( "\tStand-alone program to load", 29, fp );
    if( (rc = StartLibr( LIBASK, 0 )) < 1 ) _exit_(rc);
    *plab = AZC[1];

    ip = GetFileItem( *plab );
    s = (char*) &ip->info;
    pdesc->leng = (s[0]<<8) + (s[1]&0xFF);
    pdesc->lbuf = (s[2]<<8) + (s[3]&0xFF);
    pdesc->base = (s[4]<<8) + (s[5]&0xFF);
    pdesc->pc0  = (s[6]<<8) + (s[7]&0xFF);
}

/*
 * Запрос пользователя и захват устройства
 */
void GetDevice()
{
    short chan;
    char c0, c;

    c0 = '0';
    for(;;){
        dpc( "\037Line:" );
        c = dpq( c0 );
        if( c == FinSym ) _exit_( 0 );
        if( c >= '0' && c <= '9' ){
            dpo( c );
            if( ctake( &chan, 'U', c-'0' ) == 0 ) break;
        }else if( c == ET ){
            dpo( c0 );
            if( ctake( &chan, 'U', c0-'0' ) == 0 ) break;
        }
    }
    copend( &cline, chan );
}

/*
 * Запуск диалога с LSI-11, проверка работоспособности
 */
void InitDial()
{
    static char lRN[] = { RN };
    int rc;
    char c0;
    int isecho;
    int ntry;

    dpc( "\f\n." );
    ntry = 0;
    for(;;){
        InpFlush( 1 );
        if( (rc = cwrite( &cline, lRN, 1 )) < 0 ) _abort_( rc );
        isecho = 0;
        for(;;){
            cstime( &cline, 2 );
            if( (rc = cread( &cline, &c0, 1 )) < 0 ) _abort_( rc );
            if( rc < 1 ) break;
            if( c0 == LF || c0 == RN ){
                isecho = 1;
            }else if( c0 == '@' ){
                if( isecho ){
                    cnotime( &cline );
                    dpc( "\f\n " );
                    return;
                }
            }else{
                isecho = 0;
            }
        }
        if( ++ntry >= 2 ) Error(4);
    }
}

/*
 * Читаем с диска
 */
void LoadImage( pbuff, label, pmod )
    char **pbuff;
    char label;
    struct module *pmod;
{
    IOl_block icb;
    int rc;

    if( (rc = ExpandStorage( pmod->leng+1 ))>>8 == (~0) ) _abort_(rc);
    *pbuff = _SliceAddr;

    icb.label = label;
    icb.dir = 2;
    icb.bufadr = *pbuff;
    icb.buflen = pmod->leng;
    icb.sector = 0l;
    if( (rc = _io_( &icb ))>>8 == (~0) ) _abort_(rc);
}

/*
 * Загружаем программу в объекную машину
 */
void SendImage( mem, pmod, check )
    char *mem;
    struct module *pmod;
    int check;
{
    static char addrout[] = {HO,CD," 000000"};
    static char cntout[] = {SP,1,9,"000000 -"};
    static char repc;
    char openc;
    unsigned cnt;             /* Счетчик смещения в образе */
    int rcnt;                 /* Прочтенные байты */
    int rc;
    int word, word1;
    int ntry;

    /*
     * Передаем данные в машину
     */
     /* Задаем начальный адрес и смотрим на ответ */
    MVS( _conv( pmod->base, 0x86, 8 ), 6, addrout+3 );
    dpc( addrout );
    SetAddr( pmod->base );
    cntout[10] = '+';
    for( cnt = 0; cnt < pmod->leng; cnt += 2 ){
        word = (mem[cnt]&0xFF) + (mem[cnt+1]<<8);
        if( cnt%0200 == 0 ){
            MVS( _conv( cnt, 6, 8 ), 6, cntout+3 );    dpc( cntout );
        }
        for( ntry = 0; PutWord( word ) != 0; ntry++ ){
            if( ntry >= 5 ) Error(4);
            InitDial();
            SetAddr( pmod->base+cnt );
        }
    }
    cnotime( &cline );

    /*
     * Считываем и сравниваем
     */
    if( !check ) return;
    SetAddr( pmod->base );
    openc = '/';
    for( cnt = 0; cnt < pmod->leng; cnt += 2 ){
        if( cnt%0200 == 0 ){
            cntout[10] = '?';
            MVS( _conv( cnt, 6, 8 ), 6, cntout+3 );    dpc( cntout );
        }
        InpFlush( 0 );
         /* Открываем ячейку */
        if( cwrite( &cline, &openc, 1 ) < 1 ) goto eio;
         /* Читаем ответ */
        rcnt = 0;
        for(;;){
            cstime( &cline, 2 );
            if( (rc = cread( &cline, &repc, 1 )) < 0 ) goto eio;
            if( rc < 1 ) Error(2);                /* Тайм-аут         */
            if( repc == '/' ) break;
            if( ++rcnt > 16 ) Error(1);
        }
        rcnt = 0;
        word1 = 0;
        for(;;){
            cstime( &cline, 2 );
            if( (rc = cread( &cline, &repc, 1 )) < 0 ) goto eio;
            if( rc < 1 ) Error(2);
            if( repc == ' ' ) break;
            if( repc < '0' || repc > '7' ) Error(1);
            if( ++rcnt > 7 ) Error(1);
            word1 = (word1<<3) + repc-'0';
        }
        cnotime( &cline );

        word = (mem[cnt]&0xFF) + (mem[cnt+1]<<8);
        if( word != word1 ){
            static char ecmpout[] = {SP,1,9,"000000/000000 <>000000"};
            MVS( _conv( cnt, 6, 8 ), 6, ecmpout+3 );
            MVS( _conv( word1, 0x86, 8 ), 6, ecmpout+10 );
            MVS( _conv( word,  0x86, 8 ), 6, ecmpout+19 );
            dpc( ecmpout );
            Error(3);
        }

        openc = LF;
    }

    return;

eio:
    if( (rc = cerror(&cline)) < 0 ) _abort_(rc);
    _abort_(ErrDevFail);
}

/*
 * Открыть ячейку в удаленной машине
 */
void SetAddr( addr )
    unshort addr;
{
    static char addrtxt[] = "000000/";
    static char reply[]   = "000000/000000 ";
%d  static char repout[]  = {
%d      SP,3,0,"addr: ^0^0^0^0^0^0^/^0^0^0^0^0^0^ "
%d  };
    int rc;

    InpFlush( 0 );
    MVS( _conv( addr, 0x86, 8 ), 6, addrtxt );
    if( cwrite( &cline, addrtxt, 8 ) < 8 ) goto eio;
    cstime( &cline, 2 );
    if( (rc = cread( &cline, reply, sizeof(reply)-1 )) < 0 ) goto eio;
    if( rc < sizeof(reply)-1 ) Error(2);          /* Тайм-аут         */
    if( reply[6] != '/' ) goto esynt;
     /* Проверка на восьмеричную цифру на случай всяких "BUS ERROR" */
    if( reply[7] < '0' || reply[7] > '7' ) goto esynt;
    cnotime( &cline );
    return;

eio:
    if( (rc = cerror(&cline)) < 0 ) _abort_(rc);
    _abort_(ErrDevFail);

esynt:
%d  { int i, j;
%d      j = 9;
%d      for( i = 0; i < rc; i++ ){
%d         if( ((repout[j] = reply[i])&0xE0) == 0 ){
%d             repout[j++] = '^';
%d             repout[j] = reply[i] - 0x40;
%d         }
%d         j++;
%d      }
%d      repout[j] = 0;
%d  }
%d  dpc( repout );
    Error(1);
}

/*
 * Запись слова в память объектной машины
 * При ошибках тайм-аута и синтаксиса возвращает ненулевой код.
 * При других ошибках отваливает как обычно.
 */
int PutWord( word )
    unshort word;
{
%d  static char diagb[] = {SP,3,0,
%d      "word: -------- 50 символов -----------------------------"
%d  };
%d  static char *diagend = diagb+sizeof(diagb)-2;  /* с учетом "^0" */
    static char datum[] = "000000\n";
    static char repc;
    int rcnt;
%d  char *dcp;
    int rc;

    InpFlush( 0 );
    MVS( _conv( word, 0x86, 8 ), 6, datum );
    if( cwrite( &cline, datum, 7 ) < 7 ) goto eio;
     /* Читаем ответ */
%d  dcp = diagb+9;
    cstime( &cline, 2 );
    rcnt = 0;
    for(;;){
        if( (rc = cread( &cline, &repc, 1 )) < 0 ) goto eio;
        if( rc < 1 ) goto etime;                  /* Тайм-аут         */
%d      if( dcp < diagend ){
%d          if( (repc & 0xE0) == 0 ){  *dcp++ = '^';  repc += 0x40; }
%d          *dcp++ = repc;
%d      }
        if( repc == '/' ) break;
        if( ++rcnt > 16 ) goto esynt;
    }
    cstime( &cline, 2 );
    if( (rc = cread( &cline, &repc, 1 )) < 0 ) goto eio;
    if( rc < 1 ) goto etime;
%d  if( dcp < diagend ){
%d      if( (repc & 0xE0) == 0 ){  *dcp++ = '^';  repc += 0x40; }
%d      *dcp++ = repc;
%d  }
    if( repc < '0' || repc > '7' ) goto esynt;
    rcnt = 0;
    for(;;){
        cstime( &cline, 2 );
        if( (rc = cread( &cline, &repc, 1 )) < 0 ) goto eio;
        if( rc < 1 ) goto etime;
%d      if( dcp < diagend ){
%d          if( (repc & 0xE0) == 0 ){  *dcp++ = '^';  repc += 0x40; }
%d          *dcp++ = repc;
%d      }
        if( repc == ' ' ) break;
        if( ++rcnt > 7 ) goto esynt;
    }
    cnotime( &cline );
    return 0;

esynt:
%d  *dcp = 0;
%d  dpc( diagb );
    cnotime( &cline );
    return -1;

etime:
    cnotime( &cline );
    return -1;

eio:
    if( (rc = cerror(&cline)) < 0 ) _abort_(rc);
    _abort_(ErrDevFail);
    return -1;      /*NOTREACHED*/
}

/*
 * Выдача сообщения об ошибке и выход с ненулевым кодом
 */
static char *etexts[] = {
    /* 0*/   "Unknown error",
    /* 1*/   "Dialogue syntax error",
    /* 2*/   "Reply time-out",
    /* 3*/   "Comparison error",
    /* 4*/   "No initial responce"
};
#define ELIM (sizeof(etexts)/sizeof(char*))

void Error( ecode )
    int ecode;
{
    dpc( "\f\n\n" );
    dpc( etexts[ ((unsigned)ecode >= ELIM)? 0: ecode ] );
    dpq( '!' );
    _exit_( ecode );
}

/*
 * Очистка входной очереди на устройстве згрузки
 */
void InpFlush( intertime )
    short intertime;          /* Зазор между символами                */
{
    static char rdbuff[ 40 ];

    for(;;){
        do{
            cstime( &cline, 0 );
        }while( cread( &cline, rdbuff, sizeof(rdbuff) ) > 0 );
        if( intertime == 0 ) break;
        cstime( &cline, intertime );
        if( cread( &cline, rdbuff, 1 ) < 1 ) break;
    }
    cnotime( &cline );
}

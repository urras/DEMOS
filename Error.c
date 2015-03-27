#include <Display>
#include <SysCalls>
#include <SysConv>
#include <SysStrings>
#include "LinkerDefs"         /* Проверка прототипов */
#ifndef LFNAME
#   define LFNAME 10
#endif
#ifndef NULL
#  define NULL 0
#endif

void lino( char* );
void linolf( void );

static void NewLine();
static void NamePrint();

static char *(Emess[]) = {
  /*0*/  "Internal: <No error text>",
  /*1*/  "Bad object module format",
  /*2*/  "Segment or module is longer than 64K",
  /*3*/  "CSV FILE error due opening a library",
  /*4*/  "Object file open/close error",
  /*5*/  "Too many object modules",
  /*6*/  "Object module reading error",
  /*7*/  "Catalogue or Sublibrary reading error",
  /*8*/  "Relocation points to the not external",
  /*9*/  "Symbol table overflow",
  /*10*/ "Symbol list pool overflow",
  /*11*/ "Temp file open error",
  /*12*/ "Temp file writing error",
  /*13*/ "Disk full",
  /*14*/ "Internal: Output file seek error",
  /*15*/ "Too large sector"
};

static char *(Diags[]) = {
  /*0*/  "Internal: Unsupported diagnostic",
  /*1*/  "Object module '%' isn't found",
  /*2*/  "Reference to the undefined external '%'",
  /*3*/  "Double definition of external '%'",
  /*4*/  "Syntax error in control file, line %"
};

static int Init = 1;          /* <>0, чтобы не попасть в bss */
static int NamePrinted;
static char
    NameBuff[ 2+LFNAME+1 ] = { ' ',' ', "          \0" },
    *ModuleName = NameBuff+2;

static dpsize ScreenSize;
static int Vcurr;

static int ErrCnt;

static void NewLine( void );
static void NamePrint( void );

void InitDiag()
{
    ErrCnt = 0;
    ScreenSize = GetScreenSize();
    Vcurr = 2;
}

void SetDiag( Name )
    char *Name;
{
    NamePrinted = 0;          /* Если и было, то неактуально */
    MVS( Name, LFNAME, ModuleName );
}

void Error( ErrNum )
{
    if( ErrNum < 0 || ErrNum >= sizeof(Emess)/sizeof(char *) ){
        ErrNum = 0;
    }

    lino( NULL );
    NamePrint();
    NewLine();
    dpc( " *Fatal*  " );   dpc( Emess[ ErrNum ] );   dpq( '!' );
    _exit_(2);
}

void Diagnose( MessNum, Arg )
    unsigned MessNum;
    char *Arg;
{
    char *cp, *cp0;

    ErrCnt++;

    if( MessNum >= sizeof(Diags)/sizeof(char*) ) MessNum = 0;

    if( Init ){
        Init = 0;
        /**ScreenSize = GetScreenSize();**/
        NewLine();
        cp = "         -(Errors)-";
        lino( cp );    dpc( cp );
    }

    NamePrint();              /* Выводим имя файла, если не выведено */
    NewLine();
    cp0 = Diags[ MessNum ];
    for( cp = cp0; *cp; cp++ ){
        if( *cp == '%' ){
            if( Arg != NULL ){
                *cp = 0;   lino( cp0 );  dpc( cp0 );   *cp++ = '%';
                lino( Arg );  dpc( Arg );
                lino( cp );   dpc( cp );
                return;
            }
        }
    }
    lino( cp0 );    dpc( cp0 );
    return;
}

static void NewLine()
{
    static char Hid[] = {  HO,CU,CU,EL,' ','.','.','.','.', 0 };
    static int Feed = 1;

    linolf();
    if( ++Vcurr >= ScreenSize.v ){
        Vcurr = ScreenSize.v - 1;
        if( Feed ){   Feed = 0;  dpc( Hid ); };
    }
    SetCursorPos( Vcurr, 0 );
}

static void NamePrint()
{
    int i;

    if( !NamePrinted ){
        NamePrinted++;
        NewLine();
        for( i = LFNAME; --i >= 0; ){
            if( ModuleName[ i ] != ' ' ) break;
        }
        ModuleName[ ++i ] = 0;
        lino( NameBuff );    dpc( NameBuff );
    }
}

void ErrTerm()
{
    static char outbuff[] = {
        SP, 0, 10, "Errors:NNNN\0"
    };
    dpsize ds;

    if( ErrCnt ){
        lino( NULL );

        ds = GetScreenSize();
        outbuff[2] = (char)((ds.h - 11) >> 1);
        MVS( _conv( ErrCnt, 4, 10 ), 4, & outbuff[10] );
        dpc( outbuff );   dpq('_');
        _exit_(1);
    }
    _exit_(0);
}

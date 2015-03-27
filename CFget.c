/*
 * LSI-11 Linker
 * ������ ������������ �����.
 * ����� ������� �������� � ���������� ����� � ��������� �������,
 * ����������� ��������� � ������� �����. � ������ �������� ���.
 */
#include <SysCalls>           /* _exit_() */
#include <SysConv>            /* conv()   */
#include <FileSystem>         /* GetFileItem() */
#define LFNAME (sizeof(Fname))
#include <SysIO>              /* _inp1(), _stfi1() */
#include <StartOpers>         /* StartLibr() */
#define LIBASK 0
#include "LinkerDefs"         /* ��������� */

#define LDA_DFL  02000        /*����� �������� �� ���������*/

static int cback;
static int backflag;
#define GET1()  ( (backflag)?(backflag=0,cback):(cback=_inp1()) )
#define UNGET()  backflag++;

static int Lcnt;

/*
 * ������ ������. ������ ������ ��������
 */
char *CFopen()
{
    static char CFname[ LFNAME+1 ];

    /*
     * ��������� ���� � �������
     */
    {
        FileItem *ip;
        int rc;

        MVS( "S\0\0\1\tLSI-11 Linker control file", 31, AZC );
        if( (rc = StartLibr( LIBASK, 0 )) < 1 ) _exit_( rc );
        _stfi1( rc = ((int)AZC[ 1 ] & 0xFF) );
        ip = GetFileItem( (char) rc );
        MVS( ip->name.c, LFNAME, CFname );
        CFname[ LFNAME ] = 0;
    }

    /*
     * ������ ���������� ����������
     */
    {
        unsigned int Laddr;
        int radix;
        int c;

        backflag = 0;
        Lcnt = 1;

        Laddr = LDA_DFL;
        radix = 8;
        if( (c = GET1()) == '=' ){      /* ����� �������� */
            Laddr = 0;
            for(;;){
                c = GET1();
                if( c == '\n' ){  Lcnt++;  GET1();  break; }
                if( (c & 0x60) == 0x60 ) c -= 0x20;
                Laddr *= radix;
                if( c >= '0' && c <= '7' ){
                    Laddr += (c - '0');
                }else if( c >= '8' && c <= '9' ){
                    if( radix < 10 ) goto BadAddr;
                    Laddr += (c - '0');
                }else if( c >='A' && c <='F' ){
                    if( radix < 16 ) goto BadAddr;
                    Laddr += (c - 'A'+10);
                }else if( c == '&' ){
                    radix = 16;
                }else{
                    goto BadAddr;
                }
            }
        }
        UNGET();
        Tseta( Laddr );           /* ������ ����� ������� */
        return CFname;

BadAddr:
        Diagnose( 4, _conv( Lcnt, 2, 10 ) );
        while( c != '\n' && c >= 0 ) c = GET1();
        UNGET();
        Tseta( LDA_DFL );
        return CFname;
    }
}

char *CFget()       /* ���������� ��������� �� ��� � �������
                     * ��� NULL, ���� ����� ����� */
{
    static char modname[ LFNAME+1 ];
    int c;
    int len;

    while( (c = GET1()) == ' ' || c == '\n' ){
        if( c == '\n' ) Lcnt++;
        if( c <= 0 ){
            UNGET();
            return NULL;
        }
    }
    UNGET();
    for( len = 0; len < LFNAME; len++ ){
        c = GET1();
        if( c == '\n' ) Lcnt++;
        if( c <= 0 ){
            UNGET();
            return NULL;
        }
        if( c == ' ' || c == '\n' ) break;
        modname[ len ] = (char) c;
    }
    modname[ len ] = '\0';

    return modname;
}

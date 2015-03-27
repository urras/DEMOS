/*
 * LSI-11 Linker
 * ��������� ������������ ������
 */
#include <SysStrings>
#include "LinkerDefs"

#define INTSIZE  2    /* ������ "�����" � �������� ������� �����.
                       * �������, ��� �� ���� ������ ������ OutWget */

void Reloc( mp )
    Module *mp;
{
    char *relbuff;            /* ����� � ����������� � ����������� ..*/
    int relblen;              /* .. � ��� ���������� ����� */
    int relindex;             /* ������ � ������ */

    unsigned int x, modbase, modleng;
    unsigned outseek;         /* ����� ���� ������ � ����� */

    unsigned rword;
    int datum,datum0;
    Symbol *sp;

    outseek = mp->Mseek;      /* ����� � ����� */
    modbase = mp->Mldbase;    /* ����� � ������ ������������ ���������*/
    modleng = mp->Mtextsize + mp->Mdatasize;
    ModSeek( modleng );               /* �� ������� ����������� */
    relindex = relblen = 0;           /* ��������� ���������� ������ */
    for( x = 0; x < modleng; x += INTSIZE ){

        if( relindex >= relblen ){    /* ����� �������� */
            ModRead( &relbuff, &relblen );
            /** if( relblen >  �������_������  ) relblen = ??; **/
            relindex = 0;
        }
        rword = (relbuff[ relindex++ ] & 0xFF);
        rword += (relbuff[ relindex++ ] << 8);

        if( rword == RABS ) continue; /* ��������, �� ��������� */

         /* ����� ��������� �/� ����� */
        datum0 = datum = OutWGet( outseek + x );

        switch( rword & REF_TO ){
        case RTEXT:
        case RDATA:
            datum += modbase;             /* �������� ����� ������ */
            break;
        case RBSS:
             /* �������� � �������� � bss � ����� � ������� ������ */
            datum = datum0 - modleng + Tabss() + mp->Mbssoff;
            break;
        case REXT:
             /* ����� ������� �� ��� ������� ��� ��������� ������� */
            sp = LookES( SYMNUM( rword ), mp );
            if( sp == NULL ) Error(8);    /* ��������, ��� ��������� */
            if( LOCATION( sp->Stype ) == UNDEF ){
                char diagstr[ SNLENG+1 ];
                MVS( sp->Sname, SNLENG, diagstr );
                diagstr[ SNLENG ] = '\0';
                Diagnose( 2, diagstr );   /* ������ �� �������������� */
            }else{
                datum += sp->Sval;
                if( LOCATION( sp->Stype ) == BSS ){
                    datum += Tabss();
                }
            }
            break;
        case RABS:
            break;                        /*   movb $'*,ConsDBR */
        default:
            Error(1);
        }
        if( rword & RPCBIT ) datum -= modbase;

        if( datum0 != datum ){
            char outbuff[ INTSIZE ];
            outbuff[0] = (char)datum;
            outbuff[1] = (char)(datum>>8);
            OutWrite( outbuff, INTSIZE );
        }

    } /*loop*/
}

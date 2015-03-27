/*
 * LSI-11 Linker
 * Настройка загрузочного модуля
 */
#include <SysStrings>
#include "LinkerDefs"

#define INTSIZE  2    /* Размер "слова" в терминах формата файла.
                       * Заметим, что на него зашита секция OutWget */

void Reloc( mp )
    Module *mp;
{
    char *relbuff;            /* Буфер с информацией о перемещении ..*/
    int relblen;              /* .. и его актуальная длина */
    int relindex;             /* Индекс в буфере */

    unsigned int x, modbase, modleng;
    unsigned outseek;         /* Адрес тела модуля в файле */

    unsigned rword;
    int datum,datum0;
    Symbol *sp;

    outseek = mp->Mseek;      /* Адрес в файле */
    modbase = mp->Mldbase;    /* Адрес в памяти конкатенации сегментов*/
    modleng = mp->Mtextsize + mp->Mdatasize;
    ModSeek( modleng );               /* На таблицу перемещений */
    relindex = relblen = 0;           /* Имитируем исчерпание буфера */
    for( x = 0; x < modleng; x += INTSIZE ){

        if( relindex >= relblen ){    /* Буфер исчерпан */
            ModRead( &relbuff, &relblen );
            /** if( relblen >  остаток_модуля  ) relblen = ??; **/
            relindex = 0;
        }
        rword = (relbuff[ relindex++ ] & 0xFF);
        rword += (relbuff[ relindex++ ] << 8);

        if( rword == RABS ) continue; /* Банально, но эффктивно */

         /* Здесь указатель в/в стоит */
        datum0 = datum = OutWGet( outseek + x );

        switch( rword & REF_TO ){
        case RTEXT:
        case RDATA:
            datum += modbase;             /* Сегменты слиты вместе */
            break;
        case RBSS:
             /* Приводим к смещению в bss и затем к полному адресу */
            datum = datum0 - modleng + Tabss() + mp->Mbssoff;
            break;
        case REXT:
             /* Поиск символа по его индексу для локальной таблицы */
            sp = LookES( SYMNUM( rword ), mp );
            if( sp == NULL ) Error(8);    /* Наверное, был локальный */
            if( LOCATION( sp->Stype ) == UNDEF ){
                char diagstr[ SNLENG+1 ];
                MVS( sp->Sname, SNLENG, diagstr );
                diagstr[ SNLENG ] = '\0';
                Diagnose( 2, diagstr );   /* Ссылка на неопределенный */
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

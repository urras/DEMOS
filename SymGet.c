/*
 * LSI-11 линкер.
 * Чтение из файла таблицы символов и
 * обновление таблицы линкера.
 */
#include "LinkerDefs"

static void SymRel( Symbol*, Module* );

void SymGet( mp )
    Module *mp;
{
    Symbol *sp;                     /* Указатель на статику Next'а */
    Symbol *tp;                     /* Указатель в таблицу символов */
    int lct;                        /* Индекс в локальной таблице */

    Next0( (mp->Mtextsize+mp->Mdatasize) << 1, mp->Msymsize );
    lct = 0;
    while( (sp = NextSymbol()) != NULL ){
        if( sp->Stype & EXTERN ){
            if( LOCATION( sp->Stype ) != UNDEF ){
                SymRel( sp, mp );   /* Правим Sval на базу сегмента */
            }
            tp = NewSym( sp );      /* Обновляем таблицу символов*/
            AddES( tp, mp, lct );   /* Пытаемся поставить в очерель */
        }
        lct++;
    }
}

static void SymRel( sp, mp )
    Symbol *sp;
    Module *mp;
{
    switch( LOCATION( sp->Stype ) ){
    case TEXT:
    case DATA:
        sp->Sval += mp->Mldbase;
        break;
    case BSS:
        /* Получаем смещение в локальном сегменте модуля */
        sp->Sval -= mp->Mtextsize+mp->Mdatasize;
        /* Передвигаем в общие буфера */
        sp->Sval += mp->Mbssoff;
        break;
    default:                  /* ABS */
        ;
    }
}

/*
 * LSI-11 Linker
 * Обновление таблицы символов
 */
#include <SysStrings>
#include "LinkerDefs"

#define HASHSIZE (MAXSYMS/2+3)

static Symbol *htab[ HASHSIZE ];
static Symbol symtab[ MAXSYMS ];
static int Nsyms;

void ClrSym()
{
    int i;
    for( i = 0; i < HASHSIZE; i++ ) htab[ i ] = NULL;
    Nsyms = 0;
}

Symbol *NewSym( sp )
    Symbol *sp;
{
    Symbol *tp;

    if( !(sp->Stype & EXTERN) ) return NULL;

    /*
     * Ищем в таблице и, если надо, создаем запись о символе
     */
    {
        int hval;
        int i;
        char *askname;

        askname = sp->Sname;
        hval = 0;
        for( i = 0; i < SNLENG; i++ ) hval = (hval<<1) + askname[ i ];
        hval = (hval & 0x7FFF) % HASHSIZE;
        for(; (tp = htab[ hval ]) != NULL; hval = (hval+1) % HASHSIZE){
            if( _cmps( tp->Sname, askname, SNLENG ) == 0 ) break;
        }

        if( tp == NULL ){     /* Надо создавать вход в таблице */
            if( Nsyms == MAXSYMS ) Error(9);
            tp = & symtab[ Nsyms++ ];
            htab[ hval ] = tp;
            MVS( askname, SNLENG, tp->Sname );
            tp->Stype = EXTERN+UNDEF;
            tp->Sval = 0;
        }
    }

    /*
     * Прописываем значение
     */
    {
        char diagbuff[ SNLENG+1 ];

        if( LOCATION( sp->Stype ) != UNDEF ){
            if( LOCATION( tp->Stype ) == UNDEF ){
                tp->Stype = sp->Stype;
                tp->Sval = sp->Sval;
            }else{
                MVS( sp->Sname, SNLENG, diagbuff );
                diagbuff[ SNLENG ] = '\0';
                Diagnose( 3, diagbuff );
            }
        }
    }

    return tp;
}

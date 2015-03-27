/*
 * LSI-11 ������.
 * ������ �� ����� ������� �������� �
 * ���������� ������� �������.
 */
#include "LinkerDefs"

static void SymRel( Symbol*, Module* );

void SymGet( mp )
    Module *mp;
{
    Symbol *sp;                     /* ��������� �� ������� Next'� */
    Symbol *tp;                     /* ��������� � ������� �������� */
    int lct;                        /* ������ � ��������� ������� */

    Next0( (mp->Mtextsize+mp->Mdatasize) << 1, mp->Msymsize );
    lct = 0;
    while( (sp = NextSymbol()) != NULL ){
        if( sp->Stype & EXTERN ){
            if( LOCATION( sp->Stype ) != UNDEF ){
                SymRel( sp, mp );   /* ������ Sval �� ���� �������� */
            }
            tp = NewSym( sp );      /* ��������� ������� ��������*/
            AddES( tp, mp, lct );   /* �������� ��������� � ������� */
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
        /* �������� �������� � ��������� �������� ������ */
        sp->Sval -= mp->Mtextsize+mp->Mdatasize;
        /* ����������� � ����� ������ */
        sp->Sval += mp->Mbssoff;
        break;
    default:                  /* ABS */
        ;
    }
}

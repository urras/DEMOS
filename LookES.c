/**
 * LSI-11 Linker
 * ������ ���������� ������������ �������� ����������
 **/
#include "LinkerDefs"

static ExtQue qpool[ EXTPOOLSIZE ];
static int Nelems = 0;

void ClrES(){  Nelems = 0;  }       /* �� ������ ������������ bss */

/*
 * ������� ������� ������: ����������� ��������� �� ��������� ExtEnv
 */
void AddES( ExtSym, ExtEnv, LocNum )
    Symbol *ExtSym;
    Module *ExtEnv;
    int LocNum;
{
    ExtQue *ExtDesc, **SearchPtr;

    if( ExtSym == NULL ) return;    /* ��������, ���������������� */
    if( (ExtSym->Stype & EXTERN ) == 0 ) return;   /* ��������� */

    if( Nelems == EXTPOOLSIZE ) Error(10);
    ExtDesc = & qpool[ Nelems++ ];

    ExtDesc->Enext = NULL;
    ExtDesc->Eptr = ExtSym;
    ExtDesc->Elocnum = LocNum;

    for( SearchPtr = & ExtEnv->Mbegsym;
         *SearchPtr != NULL;
           SearchPtr = & (*SearchPtr)->Enext )    ;
    *SearchPtr = ExtDesc;
}

/*
 * ����� LocNum � ������ ��� ��������� ExtEnv
 */
Symbol *LookES( LocNum, ExtEnv )
    unsigned LocNum;
    Module *ExtEnv;
{
    ExtQue *Pquele;

    for( Pquele = ExtEnv->Mbegsym;
         Pquele != NULL;
         Pquele = Pquele->Enext )
    {
        if( Pquele->Elocnum == LocNum ) return Pquele->Eptr;
    }

    return NULL;
}

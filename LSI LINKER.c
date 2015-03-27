/*------------------------------------------------*
 *                                                *
 *            �������� ������ ��� LSI-11          *
 *                                                *
 *------------------------------------------------*/
/*
 * ������ ��������� �������� �� ��������� � ld:
 * �������� ������ ��������� � ������� ���� �, ����� �������,
 * ���������� ������������ ����������� ������ ������ 64K ���
 * ������ � ����������� ����� (��� �� LSI-11 �������������).
 */
#include "LinkerDefs"

#ifdef R_11
#   pragma STARTER(NULL);
#endif
#ifdef i_86
#   pragma STARTER(), EXIT();
#endif
main()
{
    InitDiag();
    ClrSym();    ClrES();     /* �������� ������� */
    ModIni();                 /* �������������� ������������ ����� */

    Pass1();

    Pass2();

    ErrTerm();                /* ����� � ������������, ���� ���� */
}

void Pass1()
{
    char *mn;                 /* ��� ������ */
    Module *m;                /* ��������� ������ */
    char *cfn;                /* ��� ��������� ����� */

    cfn = CFopen();           /* �������� ������ ���. ����� */
    OutIni( cfn );            /* ��������� �������� ���� */
    Pict( cfn );              /* ������ �������� �� ������� */
    Tinit();                  /* ����� ��������� � Tcopy */
    while( (mn = CFget()) != NULL ){  /*������ ���. ����, ���� �� EOF*/
        if( (m = ModOpen( mn )) != NULL ){  /*���� � ��������� ������*/
            Tcopy( m );       /* �������� ����, ������� m->Moutbase */
            SymGet( m );      /* ������ ���������� ����������� */
            ModClose();       /* ����������� ����� ����� */
        }
    }
}

void Pass2()
{
    Module *m;

    while( (m = ModNext()) != NULL ){ /* ��������� ��������� ������ */
        Reloc( m );           /* ... � ���������� ��� */
        ModClose();
    }
    OutClose();               /* ��������, ��������������� ���. ���� */
}

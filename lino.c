/*
 * LSI-11 Linker
 * ������ � ���� �����������
 */
#include <SysIO>
#ifndef NULL
#   define NULL 0
#endif

static int First = 1;
static char mapname[] = "!Maplist  ";

/*
 * �������� ������ � ����� ������� ������ �����.
 * �������� ������: ������� "!Maplist" ��� ������ ��������� � ������
 * ���� �������� == NULL, �� ��������� ����.
 * ����������� ������� �� ���������.
 */
void lino( Arg )
    char *Arg;
{
    if( First ){
        if( Arg == NULL ) return;
        First = 0;
        _linop( mapname );    _lino( "\0" );
    }else{
        if( Arg == NULL ){
            _lino( (char*) (-1) );
            First = 1;
            return;
        }
    }
    _zlino( Arg );
}

/*
 * ������� ������ � �����
 */
void linolf()
{
    if( First ){
        First = 0;
        _linop( mapname );
    }
    _lino( "\0" );
}
              
   
 o&  �" �+
b 
  " �f%,%E
�
� 
%"�
`�g�+�b	+�fQ"0�%�@+ f�" ���" �%�o%  � � *    �     .    ��                                  HELLO:BYE �4" �"�
/*
 * LSI-11 ������
 * ��������������� ��������� ��� ModRead ��� ������ ������� ��������
 */
#include "LinkerDefs"

static char *readbuff;      /* ��������� � ����� */
static int   readblen;      /* ����� ���������� ������ */
static int   readindex;     /* ��������� ������ (� ������� ������) */

static unsigned  readcount; /* � ���������� ������� ��������... ���� */
static unsigned  readlimit; /* ����� ���� �������� */

void Next0( sym_off, sym_leng )
    unsigned int sym_off;
    unsigned int sym_leng;
{
    if( sym_off & 1 ) Error(1);
    if( sym_leng == 0 ){
        readbuff = NULL;    readblen = 0;
    }else{
        ModSeek( sym_off );
        ModRead( &readbuff, &readblen );
    }
    readindex = 0;

    readcount = 0;
    readlimit = sym_leng;
}

Symbol *NextSymbol()
{
    static Symbol rval;
    static char symbuff[ SYMSIZEOF ];

    char *getptr;
    int i;

    if( readcount+readindex >= readlimit ){     /* Logical EOF */
        return NULL;
    }
    if( readindex+SYMSIZEOF > readblen ){ /* ����� ���������� ������ */
        for( i = 0; i < SYMSIZEOF; i++ ){ /* �������� � ���������� */
            if( readindex >= readblen ){  /* ���������� */
                readcount += readblen;
                ModRead( &readbuff, &readblen );
                if( readblen <= 0 ) Error(1);
                readindex = 0;
            }
            symbuff[ i ] = readbuff[ readindex++ ];
        }
        getptr = symbuff;
    }else{                                /* ����� ������ �������� */
        getptr = & readbuff[ readindex ];    readindex += SYMSIZEOF;
    }

    for( i = 0; i < SNLENG; i++ ){
        rval.Sname[ i ] = getptr[ i ];
    }
    rval.Stype = (getptr[SYMTYPE+1] << 8) | (getptr[SYMTYPE] & 0xFF);
    rval.Sval  = (getptr[SYMVAL +1] << 8) | (getptr[SYMVAL ] & 0xFF);
    return &rval;
}
         
 {
            bool Msyslib;    
 * ����� � C.LANGUAGES ����� � �          bool Msyslib;    
 * ����� � C.LANGUAGES �����  ������ */
} Stat} Stat/
} Statsyslib;    
 * ����� � C.LANGUAGES �����  ������ */
} Stat
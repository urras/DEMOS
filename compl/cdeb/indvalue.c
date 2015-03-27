/* === CDEBUGGER === ����� � ����������� ���������� */

/* �����������: ������ �.�. ��� ���
   ...
   20.06.85 ������ �.�. ��� - �������� ���� ����������� ����
                              ������������� ����������.
*/

#include "cdeb.h"

/* #define DEBF            /* ������� ������ � �������-������� */
/* #define DEBC            /* ������� �������� �������� ���������� */

unsigned value_address;         /* ����� ����������                      */
int      value_size;            /* ����� 1-��� �������� � ������         */
int      size_flag;             /* ����: ������ �� ����� ��������        */
int      value_length;          /* ����� ������� � ���������             */
int      type_flag;             /* ��� ����������: ���., ����., �������. */
int      adr_flag;              /* ����: ����� �� ����������, � �� ����� */
char     *fmt;                  /* ������ ������ ����������              */
char     *name_beg, *name_end;  /* ������� ����� ����������              */
int      cmplx_flag;            /* ���� - ����������� ����� (�������)    */
int      logic_flag;            /* ���� - ���������� ���������� (�������)*/
int      indir_flag;            /* ���� - ��������� ��������� (�������)  */

enum valtypes {reg, global, local};


/* ���� ����� ���������� �� ��������� � ����� ��� ����������� �� */

ind_value()
{
#   define MAXFMT       20
    static unsigned old_adr, old_lgt;
    static char fmt_save[MAXFMT+1];

    if( *pcmn == '='  ||  *pcmn == ' ' ) { /* �������� ������ ���������� */
        value_address = old_adr;
        value_length  = old_lgt;
        fmt = fmt_save;
        if( *pcmn == ' ' ) {
            ++pcmn;
            get_modifiers();
        }

    } else {                              /* ������ ��� ����� ���������� */
        get_value();
        old_adr = value_address;
        old_lgt = value_length;
    }

    if( *pcmn == '=' )  {
        ++pcmn;
        change_value();

    } else {
        get_format();
        strncpy( fmt_save, fmt, MAXFMT );
        print_value();
    }
}


/* �������� ���������� � �� �������� */

get_value()
{
    register char *ptr;

    fmt = adr_flag = size_flag = cmplx_flag = logic_flag = indir_flag = 0;
    value_size   = 2;
    value_length = 1;

    name_beg = pcmn;

    if( *pcmn == '%' ) {                        /* ������� */
        ptr = ++pcmn;
        value_address = convi10();  type_flag = reg;
        if( ptr == pcmn )  error( 7 );
        if( value_address > 7 )  error( 5 );

    } else if( *pcmn == '$' ) {                 /* ����� ������ ������ */
        ptr = ++pcmn;
        value_address = convi10();
        if( ptr == pcmn )  error( 10 );
        type_flag = global;

    } else {                                    /* ��� ���������� */
        value_address = get_name_address( &type_flag );
        if( indir_flag )   ind_address();
    }

    get_modifiers();
    name_end = pcmn;
}


/* ��������� ���� ������������� ���������� */

#define CDEBUG
#include "f77types.h"
#undef CDEBUG

#define SYMLEN 8
struct tabentry {                       /* ��������� ������ ������� ���� */
    char symname[SYMLEN];
    int  symflag;
    unsigned  symvalue;
};

#define f_size  0077                    /* ������ � ������ */
#define f_cmplx 0100                    /* ���� - ����������� ����� */
#define f_logic 0200                    /* ���� - ���������� ���������� */
#define f_addr  0400                    /* ���� - �������� ���� */
#define f_(x,y,z,t,f)  {x + f_cmplx*y + f_logic*z + f_addr*t, f}

static struct {                 /* ������� ������ ���������� ������ �����*/
    unsigned f_flags;                   /* ������ � ������ � ����� */
    char    *f_fmt;                     /* ������ ������ �� ��������� */

} tftypes[NTYPES] = {
    f_( 2, 0, 0, 0, "%o"     ),         /*  TYUNKNOWN 0   */
    f_( 2, 0, 0, 0, "%o"     ),         /*  TYADDR 1      */
    f_( 2, 0, 0, 0, "%d"     ),         /*  TYSHORT 2     */
    f_( 4, 0, 0, 0, "%D"     ),         /*  TYLONG 3      */
    f_( 4, 0, 0, 0, "%g"     ),         /*  TYREAL 4      */
    f_( 8, 0, 0, 0, "%g"     ),         /*  TYDREAL 5     */
    f_( 8, 1, 0, 0, "(%g %g)"),         /*  TYCOMPLEX 6   */
    f_( 16,1, 0, 0, "(%g %g)"),         /*  TYDCOMPLEX 7  */
    f_( 2, 0, 1, 0, "t"     ),          /*  TYLOGICAL 8   */
    f_( 1, 0, 0, 0, "'"      ),         /*  TYCHAR 9      */
    f_( 2, 0, 0, 0, "%o"     ),         /*  TYSUBR 10     */
    f_( 2, 0, 0, 0, "%o"     ),         /*  TYERROR 11    */
};


ftypes( item )
    register struct tabentry *item;
{
    char diglength[SYMLEN-2+1];
    union cdebdesc ftype;
    int            i, type;

    item += 1;

/* � �������� f77 ��� ������� ���������� ��� v.n ����� ���������� �����
   ������ ���������� � �� ����������, ��� ��� ������ �� v.* ����������
   �� �� ����������� */
    if(!strncmp(item->symname,"v.",2)) item += 1;

    if( !strncmp(item->symname,"l~",2) ) {
        ftype.cdebword = item->symvalue;
        indir_flag     = ftype.cdbfarg;
        type           = ftype.cdbtype;
        if( type<0 || type>TYERROR )  type = TYUNKNOWN;
        if( !(value_size = ftype.cdbleng) ) {
            value_size = tftypes[type].f_flags & f_size;
        }
        size_flag      = 1;
        cmplx_flag     = tftypes[type].f_flags & f_cmplx;
        logic_flag     = tftypes[type].f_flags & f_logic;
        fmt            = tftypes[type].f_fmt;
        if( tftypes[type].f_flags & f_addr )  adr_flag = 1;
        if( ftype.cdbfarr ) {
            strncpy(diglength, item->symname+2, SYMLEN-2);
            if( sscanf(diglength,"%o6",&i) == 1 ) value_length *= i;
        }
    }

#ifdef DEBF
printf("type=%d size=%d length=%d iscmplx=%d islogic=%d format=%s\n",
        type,value_size,value_length,cmplx_flag,logic_flag,fmt);
#endif
}


/* ��������� ������������ ���������� :
   *             - ��������� ���������,
   &             - ������ ������ ����������,
   :�����        - ������� ����� ���������� � ������,
   [�����]       - �������� ������� � �������,
   [�����:�����] - �������� ��������� �������� � �������,
   .+-�����      - ��������� ���� � ��������� */


get_modifiers()
{
    register int  from, till;
    register char *ptr;
    char c;

    for(;;) {
        switch( c = *pcmn++ ) {

            case ':':
                value_size = convi10();
                if( !value_size )  value_size = 1;
                size_flag = 1;
                break;

            case '&':
                if( adr_flag )  error( 4 );
                adr_flag   = 1;
                value_size = 2;
                break;

            case '*':
                ind_address();
                break;

            case '[':
                if( type_flag==reg || adr_flag )  error( 4 );
                ptr = pcmn;
                from = convi10();
                if( ptr == pcmn )  error( 7 );
                value_address += (value_size * from);
                value_length = 1;
                if( *pcmn == '-' || *pcmn == ':' ) {
                    ptr = ++pcmn;
                    till = convi10();
                    if( ptr == pcmn )  error( 7 );
                    value_length = till - from + 1;
                    if( value_length <= 0 )  error( 4 );
                }
                if( *pcmn++ != ']' )  error( 7 );
                if( language == FORTRAN )  value_address -= value_size;
                break;

            case '.':  case '+':  case '-':
                if( type_flag==reg || adr_flag )  error( 4 );
                ptr = pcmn;  from = convi10();
                if( ptr == pcmn )  error( 7 );
                if( c!='-' ) value_address+=from; else value_address-=from;
                break;

            default:
                --pcmn;
                goto end_of_modifiers;
        }
    }
end_of_modifiers: ;
}


/* ��������� 'value_address' �� ��������� 'value_address' */

ind_address()
{
    if( adr_flag ) {
    } else if( type_flag == reg ) {
        type_flag = local;
        value_address = regval[ value_address ];
    } else {
        value_address = get_word( value_address );
    }
    adr_flag = 0;
}


/* ���������� ������ ���������� ���������� */

get_format()
{
    register char *ptr;

    if( !*pcmn && !fmt ) {                      /* ������ �� ��������� */
        switch( value_size ) {
            case  1:  fmt="'";   break;
            case  2:  fmt="%d";  break;
            case  4:  fmt="%D";  break;
            case  8:  fmt="%g";  break;
            default:  fmt="$";   break;
        }
        if(adr_flag)  fmt="0%o";

    } else if( *pcmn=='"' || *pcmn=='\'' || *pcmn=='$' ) { /* ���������� */
        fmt  = pcmn;
        if( *fmt == '\'' && !size_flag )  size_flag = value_size = 1;
        if( *++pcmn )  error( 8 );

    } else if( *pcmn == '%' ) {
        fmt = pcmn;
        ptr = pcmn+1;
        while( *ptr ) {
            if( !size_flag ) {           /* ���������� ������ �� ������� */
                switch( *ptr ) {
                    case 'D':  case 'O':  case 'X':  case 'U':  case 'l':
                        size_flag = value_size = 4;  break;
                    case 'e':  case 'g':  case 'f':
                        size_flag = value_size = 8;  break;
                    case 'c':
                        size_flag = value_size = 1;  break;
                }
            }
            if( *ptr++ == '%' )  error( 8 );
        }

    } else {
        if( !fmt ) error( 7 );
    }
}


/* ������ �������� ���������� */

#define MAXPRINT 80

print_value()
{
    register char v;
    register int w, i;
    char toprint[MAXPRINT];
    char *load_value(), *p;

    v = *name_end;                         /* ��������: ���_���������� = */
    *name_end = 0;
    dpc( name_beg );  dpc( " = " );
    *name_end = v;
    if( (w=value_size) > 16  &&  *fmt != '"'  &&  *fmt != '\'' ) fmt = "$";

    if( *fmt == '$'  ||  value_length != 1 ) dpo( '\n' );

    for(; value_length>0; --value_length, value_address += value_size ) {
        if( *fmt=='$' )  {                    /* ������ � ������� �����  */
            cdump( value_address&(~1), (value_size+1)&(~1) );

        } else if( *fmt=='\'' ) {             /* ������ �� �������� */
            if( adr_flag )  error( 8 );
            dpo( '\'' );
            if( type_flag == reg ) {
                i=regval[value_address];
                ind_char( i&0377 );
                if( (i>>8) ) ind_char( i>>8 );
            } else {
                for( i=0; i<value_size; ++i )  {
                    ind_char( get_byte(value_address+i) );
                }
            }
            dpo( '\'' );

        } else if( *fmt=='"' )  {             /* ������ ������ �� \0 */
            if( adr_flag )  error( 8 );
            dpo( '"' );
            for(; v=get_byte(value_address); ++value_address)  ind_char(v);
            dpo( '"' );

        } else {                              /* �������.������ printf'� */
            p = load_value();
            if( cmplx_flag  &&  *fmt == '(' ) {
                sprintf( toprint, fmt, *((double*)p), *(((double*)p)+1) );

            } else if( logic_flag  &&  *fmt == 't' ) {
                strcpy(toprint, *((int*)p) ? "t" : "f");

            } else {
                switch( value_size ) {
                    case 1:  sprintf(toprint,fmt,(*((char*)p))&0377);break;
                    case 2:  sprintf( toprint, fmt, *((int*)p)    ); break;
                    case 4:  sprintf( toprint, fmt, *((long*)p)   ); break;
                    case 8:  sprintf( toprint, fmt, *((double*)p) ); break;
                    default: error( 8 );
                }
            }
            dpc( toprint );
        }

        if( value_length > 1 )                /* ��������� � ����. ����. */
            dpo( (*fmt=='$' || w > 16) ? '\n' : '\t' );
    }
}


/* ��������� �������� ���������� � ����� � ���������� ��������� �� ���� */

char *load_value()
{
    static int valbuf[8];
    register int i;

    for(i=0; i<8; ++i)  valbuf[i] = 0;

    if( adr_flag )                            /* ��������� ����� ��������*/
        valbuf[0] = value_address;
    else if( type_flag == reg )               /* ����������� ����������  */
        valbuf[0] = regval[ value_address ];
    else if( value_size == 1 )                /* �������� ����������     */
        valbuf[0] = get_byte( value_address ) & 0377;
    else {                                    /* ������� 1 �����         */
        if( value_size > sizeof valbuf )  fatal_error( 2 );
        for( i = value_size/2; i--; )  {
            valbuf[i] = get_word( value_address + (i*2) );
        }
        if( cmplx_flag && value_size==8 ) {
            valbuf[4] = valbuf[2];  valbuf[5] = valbuf[3];
            valbuf[2] = valbuf[3] = 0;
        }
    }
    return( valbuf );
}


/* ����������� ����������:
   ����������� ::= ���������� = �������� [������ ��������]
   ��������    ::= ����� | '������' | "������" */

change_value()
{
    register char *ptr, *fm;
    int valbuf[8], va, i, j;
    register char c;

    va = value_address;
    if( adr_flag )  error( 4 );

    for(;;) {
        while( *pcmn == ' ' )  ++pcmn;
    if( !(c = *pcmn) )  break;

        if( c == '"'  ||  c == '\'' ) {            /* "������" '�������' */
            ++pcmn;
            if( type_flag == reg ) {
                i=0;  j=1;
                while( *pcmn && *pcmn != c ) {i |= j*get_char();  j <<= 8;}
                regval[ va++ ] = i;
            } else {
                while( *pcmn && *pcmn != c )  set_byte( va++, get_char() );
                if( c == '"' )  set_byte( va++, 0 );
            }
            if( *pcmn++ != c )  error( 7 );

        } else if( c == '(' ) {                     /* ����������� ����� */
            if( !cmplx_flag )  error( 10 );
            if( sscanf(pcmn,"(%E %E)",&valbuf[0],&valbuf[4])!=2) error(10);
            while( *pcmn++ != ')' ) ;
            set_word( va, valbuf[0] );  va += 2;
            set_word( va, valbuf[1] );  va += 2;
            if( value_size != 8 ) {
                set_word( va, valbuf[2] );  va += 2;
                set_word( va, valbuf[3] );  va += 2;
            }
            set_word( va, valbuf[4] );  va += 2;
            set_word( va, valbuf[5] );  va += 2;
            if( value_size != 8 ) {
                set_word( va, valbuf[6] );  va += 2;
                set_word( va, valbuf[7] );  va += 2;
            }

        } else {                                    /* ����� */
            switch( value_size ) {
                case 1:  case 2:
                    ptr = pcmn;
                    valbuf[0] = conv_int();
                    if( ptr == pcmn )  error( 10 );

                    if( type_flag==reg ) {
                        regval[va] = valbuf[0];
                    } else {
                        if( value_size == 1 ) {
                            set_byte( va, valbuf[0] );
                        } else {
                            set_word( va, valbuf[0] );
                        }
                    }
                    break;

                case 4:  case 8:
                    if( type_flag == reg ) error( 4 );
                    ptr = pcmn;
                    while( *ptr && *ptr!=' ' && *ptr!='.' ) ++ptr;
                    if( *ptr != '.' ) {                          /* long */
                        if( !sscanf( pcmn, "%D", &valbuf[0] ) ) error(10);
                        set_word( va,   valbuf[0] );
                        set_word( va+2, valbuf[1] );

                    } else {                         /* float ��� double */
                        if( sscanf(pcmn, "%E", &valbuf[0]) != 1) error(10);
                        set_word( va,   valbuf[0] );
                        set_word( va+2, valbuf[1] );
                        if( value_size == 8 ) {
                            set_word( va+4, valbuf[2] );
                            set_word( va+6, valbuf[3] );
                        }
                    }
                    break;

                default:
                    error( 4 );
            }
            va += value_size;
            while( *pcmn && *pcmn!=' ' ) ++pcmn;
        }

        if( *pcmn && *pcmn!=' ' )  error( 7 );
        if( *pcmn == ' ' ) ++pcmn;
    }
}


/* ������� �������� ��� ������� � ��������� �� ������� */

set_check()
{
    char *load_value();
    char *p1;
    int  *p2;
    int  length, value, type;
    enum conrol {noch, eq_word, eq_byte, eq_reg, ne_word, ne_byte, ne_reg};

    get_value();
    length = value_size * value_length;

    if( !*pcmn ) {                      /* ������� �� ��������� */
        type = -1;

        if( type_flag == reg ) {                         /* ... �������� */
            type = ne_reg;
        } else if( length == 1 )  {                      /* ... ����� */
            type = ne_byte;
        } else if( length == 2 && !(value_address&1) ) { /* ... ����� */
            type = ne_word;
        }

        if( type >= 0 ) {
            p2 = load_value();
            set_word( CDEBFLAG+VALUE, *p2);
            set_word( CDEBFLAG+LENG,  value_address );
            set_word( CDEBFLAG+FROM,  type );

        } else {                                         /* ... ������� */
            set_word( CDEBFLAG+FROM,  value_address );
            set_word( CDEBFLAG+LENG,  length );
            value = 0;
            if( value_address&1 ) {                   /* ���� ����.����� */
                value = get_byte( value_address++ );
                --value_length;
            }
            if( value_length&1 ) {
                value += get_byte( value_address + (--value_length) );
            }
            while( value_length ) {
                value += get_word( value_address++ );
            }
            set_word( CDEBFLAG+VALUE, value );
        }


    } else if( *pcmn++ == '=' ) {       /* ������� �� �������� = ������� */
        p1 = pcmn;                                    /* ������ �������� */
        value = conv_int();
        if( !*p1 || *pcmn )  error( 7 );

        if( length>2 || length<=0 || (length=2 && (value_address&1)) ) {
            error( 13 );
        }

        set_word( CDEBFLAG+VALUE, value);
        set_word( CDEBFLAG+LENG,  value_address );

        if( type_flag == reg ) {
            set_word( CDEBFLAG+FROM, eq_reg );
        } else if( length == 1 ) {
            set_word( CDEBFLAG+FROM, eq_byte );
        } else {
            set_word( CDEBFLAG+FROM, eq_word );
        }


    } else {
        error( 7 );
    }
    stp_adr = 0;

#ifdef DEBC
printf( "from=%06o length=%06o value=%06o\n", get_word( CDEBFLAG+FROM ),
get_word( CDEBFLAG+LENG ), get_word( CDEBFLAG+VALUE ) );
#endif

}

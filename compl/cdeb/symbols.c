/* === CDEBUGGER === РАБОТА С ТАБЛИЦЕЙ ИМЕН */

/* точки входа:
        get_symbol_table()      - ввод таблицы имен из файла с задачей
        look_file(adr)          - поиск файла в котором расположен этот adr
        look_function(name)     - поиск первой строки указанной функции
        look_symbol(value,type) - поиск символа 'value' типа 'type'
                                  возвращает указатель на имя или 0
        get_name_address(&flag) - определить адрес переменной, имя которой
                                  в директиве и вернуть ее тип (0 - регист.
                                  1 - глобальная, 2 - локальная )        */

/*      Разработчик: Флеров М.Н. ИПК МАП
        ...
        23.06.85 Руднев А.П. ИАЭ - Вставлен поиск переменной / элемента
                                   common + исправлено несколько ошибок */

/* #define DEB             /* отладка поиска имени переменной */

#include <a.out.h>
#include <ctype.h>
#include "cdeb.h"
#include "f77types.h"

#define tyoffset  unsigned
#define SYMLEN  8               /* предикаты для работы с таблицей имен  */
#define is_absolute(p)  (  p->symflag          == N_ABS  )
#define is_text(p)      (((p->symflag)&N_TYPE) == N_TEXT )
#define is_data(p)      (((p->symflag)&N_TYPE) == N_DATA )
#define is_bss(p)       (((p->symflag)&N_TYPE) == N_BSS  )
#define is_static(p)    ( is_data(p) || is_bss(p)        )
#define is_reg(p)       (  p->symflag          == N_REG  )
#define is_file(p)      (  p->symflag          == N_FN   )
#define is_extern(p)    ( (p->symflag)&N_EXT             )
#define is_extext(p)    (  p->symflag  == (N_EXT|N_TEXT) )
#define is_loctext(p)   (  p->symflag          == N_TEXT )

/* структура статьи таблицы имен         */
/*struct tabentry {
/*    char symname[SYMLEN];
/*    int  symflag;
/*    unsigned  symvalue;
/* };                           */

#define tabentry nlist
#define symname  n_name
#define symflag  n_type
#define symvalue n_value

char *symtab, *symtabend;       /* начало и конец таблицы имен           */


/* СЧИТЫВАЕТ ТАБЛИЦУ ИМЕН И ИЩЕТ ЯЧЕЙКУ 'cdebflag=1' В ОТЛАЖ. ПРОГРАММЕ */

get_symbol_table(argc,argv)
    int argc;
    char **argv;
{
    int   fsym;
    long  offset;
    register struct tabentry *p;
    struct exec bufex;

    if( argc < 2 )  fatal_error( 5 );
    if( (fsym=open(argv[1],0)) < 0 )  fatal_error( 3 ); /* читаем таблицу*/
    read(fsym, &bufex,(sizeof bufex));

    symtab = sbrk(0);
    offset = (long)bufex.a_text + bufex.a_data;
    if( bufex.a_flag != 1 )  offset *= 2;

    lseek(fsym, offset+(sizeof bufex), 0);
    if( sbrk( bufex.a_syms ) == -1 )  fatal_error( 4 );
    symtabend = symtab + read(fsym, symtab, bufex.a_syms);
    close(fsym);

    for(p=symtab; p<symtabend; ++p)     {        /* ищем ячейку cdebflag */
        /*db+ printf("%o %.8s \n",p->symflag,p->symname);       */
        if(p->symflag == (N_EXT|N_BSS)
        && !strncmp(p->symname,"cdebflag",SYMLEN)) {
            CDEBFLAG = p->symvalue;
            return;
        }
    }
    fatal_error( 1 );
}


/* ПОИСК ФАЙЛА, В КОТОРОМ НАХОДИТСЯ АДРЕС 'adr' */

struct tabentry *filebeg, *fileend;   /* границы табл.имен данного файла */
char file_name [SYMLEN+3];

look_file(adr)
    unsigned register adr;
{
    register struct tabentry *p;
    static char *oldname;
    register char *name, *pn;

    name = 0;  fileend = symtabend;
    for(p=symtab; p<symtabend; ++p)     {
        if( is_file(p) ) {
            if( p->symvalue > adr ) {
                fileend = p;
                break;
            }
            name = p->symname;
            filebeg = p;
        }
    }
    if( !name )  fatal_error( 0 );

    if( name != oldname)  {                        /* вошли в новый файл */
        if( act_file )  fclose(act_file);
        strncpy(file_name,name,SYMLEN);
        file_name[SYMLEN] = 0;                       /* формируем: имя.c */
        pn = name = &file_name[strlen(file_name)];   /*    вместо: имя.o */
        while( --name >= file_name  &&  *name != '.' ) {};
        if( name < file_name )  name = pn;
        *name++ = '.';
        switch(language) {
            case C:          *name++ = 'c';  break;
            case FORTRAN:    *name++ = 'f';  break;
        }
        *name = 0;
        act_file = fopen( file_name, "r" );
        if( !act_file )  fatal_error( 0 );
        line_ini( act_file );
        oldname = name;
    }
}


/* ПОИСК АДРЕСА ПЕРЕМЕННОЙ ПО ЕЕ ИМЕНИ */

enum valtypes {reg, global, local};

tyoffset get_name_address( pflag )
    int                   *pflag;
{
    char for_name[SYMLEN+2];
    register char *ptr;
    int flag;
    register struct tabentry *p;
    struct tabentry *item;
    tyoffset myoffset;

    myoffset=0;
    ptr = for_name;                     /* пересылаем имя в 'for_name' */
    *ptr++ = '_';
    while( isalpha(*pcmn) || isdigit(*pcmn) || (*pcmn=='_') )  {
        if( ptr < &for_name[SYMLEN+1] )  *ptr++ = *pcmn;
        ++pcmn;
    }
    *ptr++ = 0;

    if( language == FORTRAN ) {
        item = fget_name( for_name+1 );
        if( item ){
            /*Если это элемент "common", то определяем его истинный адрес*/
            if( myoffset = fget_common( item)) {
                ftypes( ++item);
                *pflag = global;
                return (myoffset);
            }
            ftypes( item );
        }
        goto end_look;
    }

    flag = item = 0;                       /* поиск имени в таблице имен */

#ifdef DEB
printf("ищем C имя: \"%s\"\n",for_name);
#endif

    for(p=filebeg; p<symtabend; ++p) {

#ifdef DEB
printf("\"%8.8s\" \t%06o \t%02o \tflag=%d\n",
p->symname,p->symvalue,p->symflag,flag);
#endif

        if( p >= fileend )  flag = 1;

        if( is_extern(p) ) {               /* глобальные имена */
            if( !strncmp( p->symname, for_name, SYMLEN ) ) {
                item = p;
                break;
            }

        } else if( is_file(p) ) {        /* ищем только в пределах файла */
            if( p->symvalue > act_adr ) {
                if( item ) break;              /* найдено */
                flag = 1;                      /* конец поиска локальных */
            } else {                           /* начало нового файла */
                item = 0;
            }

        } else if( !flag ) {                 /* ищем локальные имена */
            if( is_text(p) ) {               /* в пределах функции */
                if( p->symvalue > act_adr  &&  *p->symname != '_' ) {
                    if( item ) break;           /* найдено */
                    flag = 1;                   /* конец поиска локальных*/
                } else if( item && !is_static(item) ) {
                    item = 0;
                }

            } else {
                if(               !strncmp(p->symname,for_name+1,SYMLEN)
                ||(is_static(p)&& !strncmp(p->symname,for_name,  SYMLEN))){
                    item = p;
                }
            }
        }
    }

end_look:
    if( !item )  error( 3 );                    /* не найдено имя */

    if( is_reg(item) ) {
        *pflag = reg;
        return( item->symvalue );

    } else if( is_absolute(item) ) {
        *pflag = local;
        return( item->symvalue + regval[5] );

    } else {
        *pflag = global;
        return( item->symvalue );
    }
}


/* ПОИСК ФОРТРАНОВСКИХ ПЕРЕМЕННЫХ */

char *fget_name( pname )
    char        *pname;
{
    register struct tabentry *p, *pf, *item;
    char fname[SYMLEN];
    int state;

#ifdef DEB
printf("ищем F77 имя \"%s\"   ",pname);
#endif

    pf=0;                                     /* опеределяем имя функции */
    for(p=filebeg; p<fileend; ++p) {
        if( is_text(p) ) {
            if( p->symvalue > act_adr )  break;
            pf = p;
        }
    }
    strncpy(fname, pf->symname, SYMLEN);
    *fname = '~';

#ifdef DEB
printf("в функции \"%.8s\" \n",fname);
#endif

    item = state = 0;
    for(p=filebeg; p<symtabend; ++p) {

#ifdef DEB
printf("\"%8.8s\" \t%06o \t%02o \tstate=%d\n",
p->symname,p->symvalue,p->symflag,state);
#endif

        if( is_loctext(p) ) {
            if( !strncmp(fname,p->symname,SYMLEN) ) {
                state = 1;
            } else {
                state = 0;
            }
        } else if( is_extern(p) ) {
            state = 0 ;
        } else if( !strncmp(pname,p->symname,SYMLEN) && state) {
            item = p;
            break;
        }
    }

    return( item );
}


/* ОПРЕДЕЛЕНИЕ АДРЕСА ЭЛЕМЕНТА COMMON-БЛОКА */

tyoffset fget_common( item )
    struct tabentry  *item;
{
    register struct tabentry *p;
    char cname[SYMLEN];
    tyoffset offset;
    if( is_absolute(item) && item->symvalue == FRMCOMMON
    &&  (item+1)->symname[0] == CDBOFFS[1] ) {
        item++;
        offset = item->symvalue;
        strncpy(cname,item->symname,SYMLEN);
        cname[0] = '_';
        item = 0;
        for(p=filebeg; p<symtabend; ++p) {
           if(is_extern(p) && !strncmp(cname,p->symname,SYMLEN) ) {
                item = p;
                break;
           }
        }
        if( !item )  return( (tyoffset)0 );
        return(offset + item->symvalue);
    }
    return( (tyoffset)0 );
}



/* ПОИСК СТРОКИ НОМЕР n В ТЕКУЩЕЙ ПОДПРОГРАММЕ (!flag) ИЛИ ФАЙЛЕ (flag) */

look_line(n,flag)
    int n, flag;
{
    register struct tabentry *p;
    unsigned jsr_pc=04737, cdeb_adr, word;
    unsigned funcbeg, funcend;
    register unsigned adr;

    funcbeg = 0;  funcend = fileend->symvalue;
    for( p=filebeg; p<fileend; ++p) {
        if(  (!flag && is_text(p))  ||  (flag && is_file(p))  ) {
            if( p->symvalue <= act_adr ) {
                funcbeg = p->symvalue;
            } else {
                funcend = p->symvalue;
                break;
            }
        }
    }

    n |= language<<14;
    cdeb_adr = get_iword( act_adr - 4 );      /* ищем последовательность */
    for( adr=funcbeg; adr<funcend;  adr += 6 ) {
        word = get_iword( adr );
        if(        word == jsr_pc ) {
            if( get_iword(adr+2) == cdeb_adr && get_iword(adr+4) == n )  {
                return( adr+6 );
            }
        } else if( word == cdeb_adr ) {
            if( get_iword(adr-2) == jsr_pc   && get_iword(adr+2) == n )  {
                return( adr+4 );
            }
        } else if( word == n ) {
            if( get_iword(adr-4) == jsr_pc   && get_iword(adr-2)==cdeb_adr ){
                return( adr+2 );
            }
        }
    }
    error( 11 );
}


/* ПОИСК ПЕРВОЙ СТРОКИ ФУНКЦИИ 'name'. ВОЗВРАЩАЕТ АДРЕС 1-ОГО ВЫРАЖЕНИЯ */

look_function(name)
    register char *name;
{
    register struct tabentry *p;
    unsigned jsr_pc=04737, cdeb_adr;
    unsigned funcbeg, funcend;
    register unsigned adr;

    if( language == FORTRAN )  strcat( name, "_" );

    funcbeg = funcend = 0;                /* ищем начало и конец функции */
    for( p=symtab; p<symtabend; ++p ) {
        if( is_extext(p) && !strncmp(p->symname+1,name,SYMLEN-1) ) {
            funcbeg = p->symvalue;
            break;
        }
    }
    for( p=symtab; p<symtabend; ++p ) {
        if( is_extext(p) && p->symvalue>funcbeg
        && (!funcend  ||  p->symvalue<funcend) )  funcend = p->symvalue;
    }

    if( !funcbeg || !funcend )  error( 12 );

    cdeb_adr = get_iword( act_adr - 4 );                 /* ищем команду: */
    for( adr=funcbeg; adr<funcend;  adr += 2 ) {        /* jsr pc,cdebug */
        if( get_iword(adr+2)==cdeb_adr && get_iword(adr)==jsr_pc ) {
            return( adr+6 );
        }
    }
    error( 16 );
}


/* ПОИСК ИМЕНИ СО ЗНАЧЕНИЕМ 'value' ТИПА 'type'. ВОЗВРАЩАЕТ ИМЯ ИЛИ 0 */

look_symbol(value,type)
    register unsigned value, type;
{
    register struct tabentry *p;

    for( p=symtab; p<symtabend; ++p) {
        if( p->symflag == type  &&  p->symvalue == value )
            return( p->symname );
    }

    return( 0 );
}

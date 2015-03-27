/* ВЫДАЧА СТРОКИ, СОДЕРЖАЩЕЙ НАЗВАНИЯ КОНТРОЛЬНЫХ КЛАВИШ */

#include "tty_descr.h"

extern char *getenv(), *strcat();

hdps(     st )
    char *st;
{
    while( *st )  hdpo(*st++);
}


hdpo(            cc )
    register int cc;
{
    register char *p, *pe;
    static   int   state, l;
    enum         { lbuf=300 };       /* длина описаний названий клавиш */
    static   char  sp_ico[lbuf];

    static   char  ico[] = {
        'c','u', cu,   'c','d', cd,   'c','r', cr,   'c','l', cl,
        'i','c', ic,   'd','c', dc,   'i','l', il,   'd','l', dl,
        'p','u', pu,   'p','d', pd,
        'l','f', lf,   'r','n', rn,   'r','e', 32,   'b','l', bl,
        'e','s', es,   'e','l', el,   't','a', ta,   'd','e', 32,
        'f','i', s_finish,  's','p', s_spec,   'e','r', s_erase,
        0
    };

    if( sp_ico[0] == 0 ) {
        strcpy( sp_ico+1, HELPDIR );
        strcat( sp_ico+1, "ttys/" );
        strcat( sp_ico+1, getenv("TERM") );
        if((l=open(sp_ico+1,0)) > 0) {
            /* есть файл с именами */
            read( l, sp_ico, sizeof sp_ico );
            close( l );
        } else {
            gen_names(sp_ico);
        }
    }

    cc &= 0xff;
    switch( state ) {
        case 0:                                       /* ждем символ ^ */
usual:      if( cc == '^'  &&  sp_ico[0] != 0 )  ++state;
            else                                dpo(cc);
            break;

        case 1:                                       /* был символ ^ */
            if(      cc == '\'' )  {++state;}
            else if( cc == '^'  )  {state=0;  dpo(cc);}
            else                  {state=cc;}
            break;

        case 2:                                       /* было ^' */
            cc &= 0x1f;
        look:                                /* поиск названия клавиши */
            p = sp_ico;  pe = p + sizeof sp_ico;
            while( --cc>=0 )  while( p<pe && *p++ != '\n' ) {};
            l = -3;
            while( p < pe && *p != '\n' )  {dpo(*p++); ++l;}
            while( l < 0 )  {dpo(' '); ++l;}
            state=3;
            break;

        default:                                      /* было ^буква */
            for( p=ico; *p; p += 3 ) {
                if( p[0]==state && p[1]==cc ) {cc = p[2]; goto look;}
            }
            dpo('^');  dpo(state);  dpo(cc);              /* не найдено */
            state = 0;
            break;

        case 3:                                       /* идет ____ */
            if( cc != '_' )  {state=0; goto usual;}
            if( !l )  dpo(' ');
            else      --l;
            break;
    }
}

static struct {int code; char *liter;} names[] = {
        {cu,"up"},
        {cd,"down"},
        {cl,"<-"},
        {cr,"->"},
        {ic,"ic"},
        {dc,"dc"},
        {il,"il"},
        {dl,"dl"},
        {pu,"PgUp"},
        {pd,"PgDn"},
        {de,"del"},
        {lf,"l.feed"},
        {rn,"return"},
        {ta,"tab"},
        {es,"erase"},
        {s_spec,"home"},
        {033,"esc"}
};
#define n_names (sizeof names/sizeof names[0])
/* генерация имен клавиш по умолчанию. для настройки по termcap */
gen_names(sb)
char *sb;
{
        register int i,j,k,incod;
        char pp[13];
        for(i=0; i<31; ++i) {
            incod=dpincod(i);
            k = incod & 037;
            switch( (incod>>5) & 7 ) {
            case cnt:     sprintf(pp,"ctrl-%c",k+'@');
                          break;
            case esc:     sprintf(pp,"esc+%c",k+'@');
                          break;
            case esc_cnt: sprintf(pp,"esc+ctrl-%c",k+'@');
                          break;
            case esc_sgn: sprintf(pp,"esc+%c",k+' ');
                          break;
            case prf:     sprintf(pp,"P+%c",k+'@');
                          break;
            case prf_cnt: sprintf(pp,"P+ctrl-%c",k+'@');
            }
            strcat(sb,pp);
            /* ищем литеральное обозначение */
            for(j=0; j<n_names; ++j)
                if(names[j].code == i) break;
            if(j<n_names) { /* нашли...*/
                sprintf(pp,"(%s)",names[j].liter);
                strcat(sb,pp);
            }
            strcat(sb,"\n");
        }
}

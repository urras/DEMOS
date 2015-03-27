/* === CDEBUGGER === πολαϊ βακτα σ πςεοβςαϊοχαξιεν σπεγ-σινχομοχ χ \XXX */

static char spchar[] = "\n\t\r\f\b\v\0";
static char spicon[] = "ntrfbv0";


ind_char( ch )
    register int ch;
{
    register int i;

    if( (ch>=' ' && ch<0177)  ||  (ch&0300)==0300 ) {
        dpo( ch );

    } else {
        dpo( '\\' );
        for( i=0; i < (sizeof spicon); ++i) {
            if( ch == spchar[i] ) {
                dpo( spicon[i] );
                return;
            }
        }
        dpc( conv(ch&0377,3,8,'0') );
    }
}


/* χχοδ βακτα πο υλαϊατεμΰ 'pcmn' σ υώετον \ σινχομοχ */

char get_char()
{
    register char c;
    register unsigned i;
    extern char *pcmn;

    if( (c = *pcmn++) == '\\' ) {
        if( !(c = *pcmn++) )  error( 7 );                       /* \ Ι 0 */

        for( i=0; i < (sizeof spchar); ++i) {
            if( c == spicon[i] )  return( spchar[i] );
        }

        if( c>='0' && c<='9' ) {
            i = c -'0';
            while( (c = *pcmn)>='0' && c<='7' ) {
                i = i<<3 + (c-'0');
                ++pcmn;
            }
            if( i<0 || i>=377 ) error( 17 );    /* ΞΕΧΕÒΞΟΕ \000 ήΙΣΜΟ */
            return( i );
        }
    }
    return( c );
}


/* χχοδ οδξοηο βακτα πο αδςεσυ 'adr' */

get_byte(adr)
    register unsigned adr;
{
    if( adr&1 ) {
        return( get_word( adr-1 ) >> 8 );
    } else {
        return( get_word( adr ) & 0377 );
    }
}


/* χωχοδ βακτα 'value' πο αδςεσυ 'adr' */

set_byte(adr,value)
    register unsigned adr;
    register char value;
{
    register unsigned word;
    if( adr&1 ) {
        word = get_word( --adr );
        set_word( adr, (word&0377) | (value<<8) );
    } else {
        word = get_word( adr );
        set_word( adr, (word&(~0377)) | value );
    }
}

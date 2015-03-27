/* === CDEBUGGER === πολαϊ τελυύεκ στςολι ξα όλςαξε */

extern int  act_line;
extern char file_name[];

ind_act_line()
{
    ind_line(act_line);
}


ind_line(nline)
    int nline;
{
    register char *plin;
    plin = line(nline);
    if( plin ) {
        if( nline == 1 )  {
            dpc( " <<< ");   dpc( file_name );   dpc( " >>>\n" );
        }
        dpo( act_line==nline ? '>' : ' ' );
        dpc( conv(nline,5,10,' ') );  dpc( "  " );
        dpc( plin );
    } else {
        dpc( " <<< end of " );   dpc( file_name );  dpc( " >>>" );
    }
    return( plin );
}

/* χωδαώα λςατλοκ τεςνιξαμο-ξεϊαχισινοκ σπςαχοώξοκ ιξζοςναγιι */

help(     name, symbol )
    char *name, symbol;
{
    static char ssym[2];
    unsigned    stt;
    char        help[50];

    ssym[0] = symbol;

    dpend();
    if( fork() == 0 ) {
        strcpy( help, HELPDIR );
        strcat( help, "HELP" );
        execl( help, "HELP", name, ssym, 0 );
        exit(1);
    }
    wait(&stt);
    dpbeg();
}

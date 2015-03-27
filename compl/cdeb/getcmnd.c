/* === CDEBUGGER === ВВОД ОЧЕРЕДНОЙ КОМАНДЫ

   возвращает указатель на команду, кончающуюся нулем
   сама обрабатывает команду ":" и множитель.
   команды разделяются запятыми.
*/

#define MAXLIN  80

/* #define DEB     /* показ команд */

static char inplin[MAXLIN+2];                      /* введенная строка   */
static char saved[MAXLIN+1] = "1+20,";             /* запомненная строка */
static char command[MAXLIN+2];                     /* очередная команды  */
static char *pmult;

char *pcom=inplin;
int  mult=0;


get_next_command()
{
    register char *ptr, *cptr, c;
    int inpflag, lgt;

    extern char *pcmn;
    extern int   jsr_ign, multi_flag, protocol;

    inpflag = 1;
    if( !*pcom && !mult ) {                     /* ввод строки с экрана  */
        inpflag=0;                              /*      в 'inplin'       */
        if( jsr_ign || multi_flag ) {
            dpo( ' ' );
            if( jsr_ign )     dpo( '\'' );
            if( multi_flag )  dpo( '"' );
        }
        dpo( protocol > 0 ? '[' : ':' );        /* выдаем приглашение    */
        lgt = dpr(inplin,MAXLIN);               /*      ","-ой           */
        inplin [lgt] = ',';  inplin [lgt+1] = 0;
        pcom = inplin;

        if( *pcom == '(' ) {                         /* запомнить строку */
            for( ptr = ++pcom, cptr=saved;  *cptr = *ptr; ++ptr, ++cptr ) {
                if( *ptr == ')' )  {*pcom=mult=0; error(7);}
            }
        }

        pcmn = pcom;  mult = 0;                        /* ввод множителя */
        lgt = convi10();
        if( lgt > 0  &&  pcmn != pcom  &&  *pcmn == '*' ) {
            pmult = pcom = ++pcmn;  mult = lgt - 1;
        }

    } else if( !*pcom ) {                 /* есть коэффициент повторения */
        --mult;  pcom = pmult;
    }

    if( *pcom == ')' ) {                        /* выполнить запомненное */
        pcmn = pcom = saved;
        lgt = convi10();
        if( lgt > 0  &&  pcmn != pcom  &&  *pcmn == '*' ) {
            pmult = pcom = ++pcmn;  mult = lgt - 1;
        }
    }

    pcmn = command;
    move_cmnd(&pcom,&pcmn);
    *pcmn = 0;

    if( inpflag ) {                            /* вывести команду на tty */
        dpo( ' ' );  dpc( command );  dpo( '\n' );
    }

#ifdef DEB
printf("command=\"%s\" mult=%d save=%s\n", command, mult, saved );
#endif

    return( command );
}


/* ПЕРЕСЫЛАЕТ КОМАНДУ С АДРЕСА '*pfrom' ПО АДРЕСУ '*pto' ДО РАЗДЕЛИТЕЛЯ */

move_cmnd(pfrom,pto)
    char **pfrom, **pto;
{
    register char *from, *to, *sep;
    static char separator[] = ",~^\|?";

    from = *pfrom;  to = *pto;
    if( *from != ',' )  {
        *to++ = *from++;
        for(;;) {
            sep = separator;
            while( *sep )  if( *from == *sep++ ) goto endcom;
            *to++ = *from++;
        }
    }
endcom:
    if( *from == ',' )  ++from;
    *pfrom = from;  *pto = to;
}

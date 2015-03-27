/* === CDEBUGGER === ���� ��������� �������

   ���������� ��������� �� �������, ����������� �����
   ���� ������������ ������� ":" � ���������.
   ������� ����������� ��������.
*/

#define MAXLIN  80

/* #define DEB     /* ����� ������ */

static char inplin[MAXLIN+2];                      /* ��������� ������   */
static char saved[MAXLIN+1] = "1+20,";             /* ����������� ������ */
static char command[MAXLIN+2];                     /* ��������� �������  */
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
    if( !*pcom && !mult ) {                     /* ���� ������ � ������  */
        inpflag=0;                              /*      � 'inplin'       */
        if( jsr_ign || multi_flag ) {
            dpo( ' ' );
            if( jsr_ign )     dpo( '\'' );
            if( multi_flag )  dpo( '"' );
        }
        dpo( protocol > 0 ? '[' : ':' );        /* ������ �����������    */
        lgt = dpr(inplin,MAXLIN);               /*      ","-��           */
        inplin [lgt] = ',';  inplin [lgt+1] = 0;
        pcom = inplin;

        if( *pcom == '(' ) {                         /* ��������� ������ */
            for( ptr = ++pcom, cptr=saved;  *cptr = *ptr; ++ptr, ++cptr ) {
                if( *ptr == ')' )  {*pcom=mult=0; error(7);}
            }
        }

        pcmn = pcom;  mult = 0;                        /* ���� ��������� */
        lgt = convi10();
        if( lgt > 0  &&  pcmn != pcom  &&  *pcmn == '*' ) {
            pmult = pcom = ++pcmn;  mult = lgt - 1;
        }

    } else if( !*pcom ) {                 /* ���� ����������� ���������� */
        --mult;  pcom = pmult;
    }

    if( *pcom == ')' ) {                        /* ��������� ����������� */
        pcmn = pcom = saved;
        lgt = convi10();
        if( lgt > 0  &&  pcmn != pcom  &&  *pcmn == '*' ) {
            pmult = pcom = ++pcmn;  mult = lgt - 1;
        }
    }

    pcmn = command;
    move_cmnd(&pcom,&pcmn);
    *pcmn = 0;

    if( inpflag ) {                            /* ������� ������� �� tty */
        dpo( ' ' );  dpc( command );  dpo( '\n' );
    }

#ifdef DEB
printf("command=\"%s\" mult=%d save=%s\n", command, mult, saved );
#endif

    return( command );
}


/* ���������� ������� � ������ '*pfrom' �� ������ '*pto' �� ����������� */

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

/* === CDEBUGGER === �������� ������ */

/* ������������ ��� ��������� ������� �������� �� C.

   ����������:
        ����� ������ ����������� ���������� C ��������� �������:
        jsr pc,*$cdebug; (����� ������). cdebug �������� ���������� "bpt".
        ������������ ������� ���� ������������ ���������
        �������� �� ������� ��������� ������������, ��������� 'ptrace'
        ������������� �������, ���������� � ������������ ���������
        ��� ������ ������� ���������� ������ 'ctrl-\' (������ 'quit')

   ��������� � �������� ��������� ����������� � ������ 'dpcdpr'.
   ��������� � ������������ ������� ��������� ����������� � 'getword'.   */


#include "cdeb.h"
#include <setjmp.h>
jmp_buf  err_reset;

main(argc, argv)                        /* �������������: ������ ������  */
    char **argv;                        /*        � ����� ������ ������  */
    int  argc;
{
    register int i;

    if( setjmp( err_reset ) )  fatal_error( 7 );
    if( *argv[1] == '-' ) {
        ++argv;  --argc;
    } else {
        dsp_inp = dsp_out = open("/dev/tty",2);
    }
    if( dsp_inp == -1 )  dsp_inp = dsp_out = 2;
    get_symbol_table(argc, argv);
    init_task(argc, argv);
    ind_act_line();
    last_line = act_line;
    command_interpreter();
}


/* ������������� �������� ��������� */

int multi_flag, m_line, protocol = -1;
FILE *m_file;

command_interpreter()
{
    register int i, flag;
    int value, toadd;
    char letter, *ptr;
    static char devtty[] = "/dev/0123456789abcd";
    FILE *savefile;
    extern int ttyflag;

    for(;;) {
        setjmp( err_reset );
        pcmn = get_next_command();

        if( (flag=value=line_number())             /* ����� ������ ����� */
        ||  *pcmn == '+'  ||  *pcmn == '-' ) {
            if( !value )  value = last_line+1;     /*     �� ����. ������*/
            ptr = pcmn;
            if( *ptr=='+' || *ptr=='-' )  {
                ++pcmn;  toadd = convi10();
                if( ptr+1 == pcmn ) {
                    toadd = 10;
                } else if( toadd <= 0  ||  *pcmn ) {
                    error( 10 );
                }
            } else {
                value -= 5;  toadd = 9;
            }
            if( !flag )  --toadd;
            if( *ptr == '-' )  value -= toadd;
            toadd += value;  if( toadd < 0 )  toadd = 0;
            if( value < 1 )    value  = 1;
            if( *pcmn )  error( 7 );

            for( last_line=value; ; ) {
                if( !ind_line( last_line ) )  {--last_line; break;}
                if( last_line != toadd ) {
                    dpo( '\n' );  ++last_line;
                } else {
                    break;
                }
            }

        } else {
            switch( *pcmn++ ) {

                case '\0':                            /* ��������� 1 ��� */
one_step:           stp_adr = 0;  run();
                    break;

                case '\'':             /* �������� ����� ����� � ��������*/
                    if( *pcmn )  error( 7 );
                    jsr_ign = !jsr_ign;
                    ind_act_line();
                    break;

                case '"':        /* �������� ����� N ���������� � ������ */
                    multi_flag = !multi_flag;
                    ind_act_line();
                    break;

                case '>':                       /* ���� � ������������   */
into_f:             if( !*pcmn ) {                      /* � �������     */
                        value   = jsr_ign;
                        jsr_ign = 1;
                        stp_adr = 0;    run();
                        jsr_ign = value;
                    } else {                            /* � ���������   */
                        stp_adr = look_function( pcmn );
                        value   = jsr_ign;
                        jsr_ign = 1;
                        run();
                        jsr_ign = value;
                    }
                    break;

                case '<':                       /* ����� �� ������������ */
from_f:             if( *pcmn )  error( 7 );
                    value   = jsr_ign;
                    jsr_ign = -1;
                    stp_adr = 0;    run();
                    jsr_ign = value;
                    break;

                case '?':                           /* ����� ���� HELP'� */
                    savefile = act_file;
                    act_file = fopen( "/usr/lib/minihelp/cdeb","r");
                    if( act_file ) {
                        line_ini( act_file );
                        value = 0;
                        for( value=1; ptr=line(value); ++value) {
                            dpc( ptr );  dpo( '\n' );
                        }
                        act_file = savefile;
                        line_ini( act_file );
                    }
                    ind_act_line();
                    break;

                case '!':                   /* ��������� ������� shell'� */
                    system( pcmn );
                    ind_act_line();
                    break;

                case '~':  case '^':            /* ������ ��������� �� ..*/
                    if( !*pcmn ) {                   /* �����������      */
                        stp_adr = 1;
                    } else if( *pcmn == '>' ) {      /* ����� � �������  */
                        ++pcmn;  goto into_f;
                    } else if( *pcmn == '<' ) {      /* ������ �� �������*/
                        ++pcmn;  goto from_f;
                    } else if( value=line_number()){ /* �������� ������ */
                        if( *pcmn )  error( 7 );
                        stp_adr = look_line( value, 1 );
                    } else {                         /*���������� �������*/
                        set_check();
                    }
                    run();
                    break;

                case '|':  case '\\':                  /* ������ ������� */
                    letter = (*pcmn >= 'a' ? *pcmn-('a'-'A') : *pcmn );
                    ++pcmn;

                    switch( letter )  {

                        case 'G':            /* ������� �� ������ ������ */
                            ptr = pcmn;  value = convi10();
                            if( ptr != pcmn  &&  *pcmn )  error( 7 );
                            regval[7] = act_adr = look_line( value, 0);
                            act_line  = value;
                            ind_act_line();
                            break;

                        case 'W':            /* �������� ������� ������� */
                            if( *pcmn )  error( 7 );
                            ind_act_line();
                            break;

                        case 'S':                 /* ����� ����� ������� */
                            if( *pcmn )  error( 7 );
                            ind_stack();
                            break;

                        case 'D':                     /* ����� ��������� */
                            strncpy( &devtty[5], pcmn, 14 );
                            if( (value=open(devtty,2)) < 0 )  error(0);
                            dsp_inp = dsp_out = value;  ttyflag = 0;
                            ind_act_line();
                            break;

                        case 'P':            /* �������/������� �������� */
                            switch( *pcmn++ ) {
                                case '+':
                                    if((protocol=open("protocol",2)) < 0
                                    &&(protocol=creat("protocol",0666))<0){
                                        error( 14 );
                                    } else {
                                        lseek( protocol, 0l, 2 );
                                    }
                                    break;
                                case '-':
                                    close( protocol );
                                    protocol = -1;
                                    break;
                                default:
                                    error( 7 );
                            }
                            break;

                        default:                  /* ����������� ������� */
                            error( 1 );
                    }
                    break;

                default:               /* ����� � ����������� ���������� */
                    --pcmn;
                    ind_value();
            }
        }
    }
}


/* ������ ������: ������ ��������� �� �������� */

#include <signal.h>

run()
{
    do {
        run_task();
    } while( !stp_cause
    &&       multi_flag  &&  act_line==m_line  &&  act_file==m_file );

    if( stp_cause == SIGQUIT ) {            /* ������� �� CTRL-\     */
        dpo( '\n' );
    } else if( stp_cause ) {                /* ������� �� �������    */
        ind_signal(stp_cause);
    }

    ind_act_line();
    m_line = last_line = act_line;
    m_file = act_file;
}

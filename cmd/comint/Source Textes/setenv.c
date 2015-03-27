/*
 * setenv - ��������� �������� ���������� �����.
 * 
 * $Log:        setenv.c,v $
 * Revision 1.4  89/12/18  19:39:19  rygoff
 * ����� �����������. ��� ��������.
 * 
 * Revision 1.3  89/12/02  13:32:05  rygoff
 * ������� ��������.
 * ������� �����������: ����� ���������� ������ �� ����������,
 * ������� ����� �������������� � �����.
 * ����� ���������� ������� ������.
 * 
 */
#include <tty_codes.h>
#include <string.h>
#include "comint.h"
#define ENVSIZE 64        /* Max. ����� ���������� � ����� */
#define VARLEN 1024       /* ����� �����=�������� */

extern  char **environ;
char env1[ENVSIZE][VARLEN];
char *new_env[ENVSIZE];

int inienv() {
    register i;
    for(i=0; i<ENVSIZE;) new_env[i++]=0;
    for(i=0; i<ENVSIZE and environ[i]; i++) {
        if( strlen(environ[i])<VARLEN )
            new_env[i]=strcpy(env1[i], environ[i]);
    }
    environ=new_env;
}



char *setenv(s) char *s; {
    register int i;
    int llen=index(s,'=')-s+1;    /* ����� ����� ���������� � '=' */

    if( ! llen ) return((char *)-1);
    for(i=0; environ[i]; i++) { /* ���� ����� ���������� � ����� */
        if(!strncmp(environ[i],s,llen)) {  /* ����� */
            if(strlen(s)>=VARLEN) {
                dpp(0, 0); dpo(es);
                dpp(0, ysize-1);
                dps("������� ������� ���: "); dps(s); dpi();
                return((char *)-1);
            }
            environ[i]=strcpy(environ[i],s);
            return((char *)0);
        }
    }
                                            /* �� ����� */
    if(i<ENVSIZE) return( environ[i]=strcpy(env1[i],s) ); else {
        dpp(0, 0); dpo(es);
        dpp(0, ysize-1);
        dps("����� ����������� "); dpi();
        return((char *)-1);
    }
}

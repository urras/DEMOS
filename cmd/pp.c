#include <stdio.h>
#include <ctype.h>
#define NOT_ASCII       "%s: there isn'i ASCII.\n"
#define BAD_OPTION      "usage: %s [-(0-9)].\n"
#define NONSENSE        "%s: nonsense!!!\n"
#define F_F     '\f'
#define N_L     '\n'
#define C_R     '\r'
#define S_P     ' '

struct option {
        int flag;
        int value;
        int max_value;
};

struct option repeat = {
        0, 0, 9};

char *pname;

main (argc,argv)
int argc;
char *argv[];
{
        int c, j, jj, i, uc, flag;
        char ln[300], lnn[300], *s;
        j = jj =  0;
        pname = *argv;
        while (*(++argv)) {
                s = *argv;
                if (*s != '-') pp_error ('b');
                else {
                        if (repeat.flag) pp_error ('b');
                        while (*(++s))
                                if (!isdigit(*s)
                                || (repeat.value = repeat.value*10 + (*s - '0'))
                                > repeat.max_value) pp_error ('b');
                }
        }
        while ((c = getchar ()) != EOF) {
                if (!isascii(c)) pp_error ('a');
                if (c == N_L) {
                        if (flag) {
                                ln[j++] = C_R;
                                lnn[jj++] = C_R;
                        }
                        ln[j++] = NULL;
                        lnn[jj++] = NULL;
                        if (flag) for (i = 0; i < repeat.value; i++) printf ("%s",lnn);
                        printf ("%s\n",ln);
                        j = jj = flag = 0;
                        continue;
                }
                if (c == F_F) {
                        printf ("\f");
                        continue;
                }
                ln[j++] = c;
                if (isalpha(c) && isupper(c)) {
                        flag = 1;
                        uc = 1;
                }
                else uc = 0;
                if (uc || !isprint(c)) lnn[jj++] = c;
                else lnn[jj++] = S_P;
        }
        exit (0);
}

pp_error (sign)
char sign;
{
        switch (sign) {
                case 'a':
                        fprintf (stderr,NOT_ASCII,pname);
                        break;
                case 'b':
                        fprintf (stderr,BAD_OPTION,pname);
                        break;
                default :
                        fprintf (stderr,NONSENSE,pname);
                        break;
        }
        exit (0);
}

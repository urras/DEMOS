#
/*
 *
 */
#define   SP   0001
#define   EOR  0002
#define   X    0004
#define   L    0010
#define   R    0020
#define   DS   0040
#define   S    0100

#include <stdio.h>

char    ctype[] {
/*      000     001     002     003     004     005     006     007     */
           0,   0,      0,      0,      0,      0,      0,      0,

/*      bs      ht      nl      vt      np      cr      so      si      */
        0,      SP,     EOR,    0,      0,      EOR,    0,      0,

        0,      0,      0,      0,      0,      0,      0,      0,

        0,      0,      0,      0,      0,      0,      0,      0,

/*      sp      !       "       #       $       %       &       '       */
        SP,     S,      S,      DS,     DS,     DS,     DS,     S,

/*      (       )       *       +       ,       -       .       /       */
        S,      S,      DS,     DS,     S,      S,      S,      S,

/*      0       1       2       3       4       5       6       7       */
        DS,     DS,     DS,     DS,     DS,     DS,     DS,     DS,

/*      8       9       :       ;       <       =       >       ?       */
        DS,     DS,     S,      S,      DS,     DS,     DS,     S,

/*      @       A       B       C       D       E       F       G       */
        DS,     X,      X,      X,      L,      X,      L,      L,

/*      H       I       J       K       L       M       N       O       */
        X,      L,      L,      X,      L,      X,      L,      X,

/*      P       Q       R       S       T       U       V       W       */
        X,      L,      L,      L,      X,      L,      L,      L,

/*      X       Y       Z       [       \       ]       ^       _       */
        X,      L,      L,      DS,     DS,     DS,     DS,     DS,

/*      �       �       �       �       �       �       �       �       */
        R,      X,      R,      R,      R,      X,      R,      R,

/*      �       �       �       �       �       �       �       �       */
        X,      R,      R,      X,      R,      X,      X,      X,

/*      �       �       �       �       �       �       �       �       */
        R,      R,      X,      X,      X,      R,      R,      R,

/*      �       �       �       �       �       �       �       del     */
        R,      R,      R,      R,      R,      R,      R,      0
};

char    xtype[] {
/*      000     001     002     003     004     005     006     007     */
          0,     0,      0,      0,      0,      0,      0,      0,

/*      bs      ht      nl      vt      np      cr      so      si      */
        0,      0,      0,      0,      0,      0,      0,      0,

        0,      0,      0,      0,      0,      0,      0,      0,

        0,      0,      0,      0,      0,      0,      0,      0,

/*      sp      !       "       #       $       %       &       '       */
        0,      0,      0,      0,      0,      0,      0,      0,

/*      (       )       *       +       ,       -       .       /       */
        0,      0,      0,      0,      0,      0,      0,      0,

/*      0       1       2       3       4       5       6       7       */
        0,      0,      0,      0,      0,      0,      0,      0,

/*      8       9       :       ;       <       =       >       ?       */
        0,      0,      0,      0,      0,      0,      0,      0,

/*      @       A       B       C       D       E       F       G       */
        0,      '�',    '�',    '�',    0,     '�',     'f',    0,

/*      H       I       J       K       L       M       N       O       */
        '�',    0,      0,      '�',    0,      '�',    0,      '�',

/*      P       Q       R       S       T       U       V       W       */
        '�',    0,      0,      0,      '�',    0,      0,      0,

/*      X       Y       Z       [       \       ]       ^       _       */
        '�',    0,      0,      0,      0,      0,      0,      0,

/*      `       a       b       c       d       e       f       g       */
        '�',    '�',    '�',    '�',    '�',    '�',    '�',    '�',

/*      h       i       j       k       l       m       n       o       */
        '�',    '�',    '�',    '�',    '�',    '�',    '�',    '�',

/*      p       q       r       s       t       u       v       w       */
        '�',    '�',    '�',    '�',    '�',    '�',    '�',    '�',

/*      x       y       z       {       |       }       ~       del     */
        '�',    '�',    '�',    '�',    '�',    '�',    '�',    0,
};


main()
{
        char    line[101], rb[100], lb[100];
        register char    *p, *j;
        char    kk;
        int     k, n, fl, fr;
        int     dot = 1;
        line [100] = '\n';
        while (fgets(line,100,stdin) != NULL) {
                p=line;
                if(*p == '.'){
                        while(! ((n = ctype[*p]) & (SP | EOR)))
                                putchar(*p++);
                }
                while((n = ctype[*p]) != EOR ){
                        if(n & (SP | DS | S)){
                                putchar(*p++);
                        }
                        if (!ctype[*p]) {
                                p++;
                                continue;
                        }
                        k = n = 0;
                        fl = fr = 0;
                        while(! ((n = ctype[*p]) & (SP | EOR | S | DS))) {
                                lb[k]= *p;
                                rb[k]= *p;
                                if(n & (X|R))
                                        rb[k] = (kk=xtype[*p])?kk:*p;
                                if(n & (L|X))
                                        lb[k] |= 040;
                                p++;
                                k++;
                                if(n == R)  fr=1;
                                if(n == L)  fl=1;
                        }
                        rb[k] = '\0';
                        lb[k] = '\0';
                        j =lb;
                        if(fr || !fl) j=rb;
                        if (dot == 2) {
                                if (j == rb && rb[0]) rb[0] |= 040;
                                if (rb[0] || !(n & (SP | EOR))) dot = 0;
                        }
                        if ((*p == '.' || *p == '!' || *p == '?') && !dot) dot = 1;
                        else if ((n & (SP | EOR)) && dot) dot = 2;
                        else dot = 0;
                        while (*j)
                                putchar(*j++);
                }
                putchar('\n');
        }
}

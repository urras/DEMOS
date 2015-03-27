/*
**      Получение символьной таблицы об'ектного файла.
**
**      nm [ -goprun ] [ файл ... ]
**
**      Стандартное местоположение в системе:  /bin
**
**      Метод сборки:  cc -O -n -s
**
**      $Header$
**      $Log$
**
*/

static  char Rcs_id[] = "$Header$";

#include        <stdio.h>
#include        <ctype.h>
#ifdef sparc
#include        "/home/mellorn/zaitcev/d22/include/ar.h"
#include        "/home/mellorn/zaitcev/d22/include/a.out.h"
#else
#include        <ar.h>
#include        <a.out.h>
#endif

/* Manage values in PDP-11 byte order */
#ifdef sparc
#define GET16(v)   ((((v)&0377)<<8)+((v)>>8&0377))
#define GET32(v)   ((((v)&0xFF0000)<<8)+((v)>>8&0xFF0000)+(((v)&0377)<<8)+((v)>>8&0377))
#else
#define GET16(v)   (v)
#define GET32(v)   (v)
#endif

/* Incore representation of member header */
struct ar_xhdr {
	long	x_size;
	char    x_name[15];
};

#define BADMAG  badmag()

#define SELECT  (arch_flg ? arp.x_name : *argv)

int     numsort_flg;
int     undef_flg;
int     revsort_flg = 1;
int     globl_flg;
int     nosort_flg;
int     arch_flg;
int     prep_flg;
struct ar_xhdr   arp;
struct exec exp;
FILE * fi;
long    off;
long    ftell ();
char   *malloc ();
char   *realloc ();

main (argc, argv)
char  **argv;
{
    int     narg;
    int     compare ();

    if (--argc > 0 && argv[1][0] == '-' && argv[1][1] != 0) {
        argv++;
        while (*++*argv)
            switch (**argv) {
                case 'n':       /* sort numerically */
                    numsort_flg++;
                    continue;

                case 'g':       /* globl symbols only */
                    globl_flg++;
                    continue;

                case 'u':       /* undefined symbols only */
                    undef_flg++;
                    continue;

                case 'r':       /* sort in reverse order */
                    revsort_flg = -1;
                    continue;

                case 'p':       /* don't sort -- symbol table
                                   order */
                    nosort_flg++;
                    continue;

                case 'o':       /* prepend a name to each line 
                                */
                    prep_flg++;
                    continue;

                default:        /* oops */
                    fprintf (stderr, "nm: invalid argument -%c\n", *argv[0]);
                    exit (1);
            }
        argc--;
    }
    if (argc == 0) {
        argc = 1;
        argv[1] = "a.out";
    }
    narg = argc;
    while (argc--) {
        fi = fopen (*++argv, "r");
        if (fi == NULL) {
            fprintf (stderr, "nm: cannot open %s\n", *argv);
            continue;
        }
        off = sizeof (exp.a_magic);
        fread ((char *) & exp, 1, sizeof (exp.a_magic), fi);
                                /* get magic no. */
        exp.a_magic = GET16(exp.a_magic);
        if ((unsigned short)exp.a_magic == (unsigned short)ARMAG)
            arch_flg++;
        else
            if (N_BADMAG(exp)) {
                fprintf (stderr, "nm: %s-- bad format\n", *argv);
                continue;
            }
        fseek (fi, 0L, 0);
        if (arch_flg) {
            nextel (fi);
            if (narg > 1)
                printf ("\n%s:\n", *argv);
        }
        do {
            long    o;
            register    i,
                        n,
                        c;
            struct nlist   *symp = NULL;
            struct nlist    sym;

            fread ((char *) & exp, 1, sizeof (struct exec) , fi);
#ifdef sparc
	    exp.a_magic = GET16(exp.a_magic);
	    exp.a_text = GET16(exp.a_text);
	    exp.a_data = GET16(exp.a_data);
	    exp.a_bss = GET16(exp.a_bss);
	    exp.a_syms = GET16(exp.a_syms);
	    exp.a_entry = GET16(exp.a_entry);
	    exp.a_flag = GET16(exp.a_flag);
#endif
            if (N_BADMAG(exp))  /* archive element not in  */
                continue;       /* proper format - skip it */
            o = (long) exp.a_text + exp.a_data;
            if ((exp.a_flag & 01) == 0)
                o *= 2;
            fseek (fi, o, 1);
            n = exp.a_syms / sizeof (struct nlist);
            if (n == 0) {
                fprintf (stderr, "nm: %s-- no name list\n", SELECT);
                continue;
            }
            i = 0;
            while (--n >= 0) {
                fread ((char *) & sym, 1, sizeof (sym), fi);
                sym.n_type = GET16(sym.n_type);
                sym.n_value = GET16(sym.n_value);
                if (globl_flg && (sym.n_type & N_EXT) == 0)
                    continue;
                switch (sym.n_type & N_TYPE) {

                    case N_UNDF: 
                        c = 'u';
                        if (sym.n_value)
                            c = 'c';
                        break;

                    default: 
                    case N_ABS: 
                        c = 'a';
                        break;

                    case N_TEXT: 
                        c = 't';
                        break;

                    case N_DATA: 
                        c = 'd';
                        break;

                    case N_BSS: 
                        c = 'b';
                        break;

                    case N_FN: 
                        c = 'f';
                        break;

                    case N_REG: 
                        c = 'r';
                        break;
                }
                if (undef_flg && c != 'u')
                    continue;
                if (sym.n_type & N_EXT)
                    c = toupper (c);
                sym.n_type = c;
                if (symp == NULL)
                    symp = (struct nlist   *) malloc (sizeof (struct nlist));
                else {
                    symp = (struct nlist   *) realloc (symp, (i + 1) * sizeof (struct nlist));
                }
                if (symp == NULL) {
                    fprintf (stderr, "nm: out of memory on %s\n", *argv);
                    exit (2);
                }
                symp[i++] = sym;
            }
            if (nosort_flg == 0)
                qsort (symp, i, sizeof (struct nlist)  , compare);
            if ((arch_flg || narg > 1) && prep_flg == 0)
                printf ("\n%s:\n", SELECT);
            for (n = 0; n < i; n++) {
                if (prep_flg) {
                    if (arch_flg)
                        printf ("%s:", *argv);
                    printf ("%s:", SELECT);
                }
                c = symp[n].n_type;
                if (!undef_flg) {
                    if (c == 'u' || c == 'U')
                        printf ("      ");
                    else
                        printf (FORMAT, symp[n].n_value);
                    printf (" %c ", c);
                }
                printf ("%.8s\n", symp[n].n_name);
            }
            if (symp)
                free ((char *) symp);
        } while (arch_flg && nextel (fi));
        fclose (fi);
    }
    exit (0);
}

compare (p1, p2)
struct nlist   *p1,
               *p2;
{
    register    i;

    if (numsort_flg) {
        if (p1 -> n_value > p2 -> n_value)
            return (revsort_flg);
        if (p1 -> n_value < p2 -> n_value)
            return (-revsort_flg);
    }
    for (i = 0; i < sizeof (p1 -> n_name); i++)
        if (p1 -> n_name[i] != p2 -> n_name[i]) {
            if (p1 -> n_name[i] > p2 -> n_name[i])
                return (revsort_flg);
            else
                return (-revsort_flg);
        }
    return (0);
}

nextel (af)
FILE * af;
{
    register       r;
    struct ar_hdr  ab;

    fseek (af, off, 0);
    r = fread ((char *) & ab, 1, sizeof (struct ar_hdr), af);
                                /* read archive header */
    if (r <= 0)
        return (0);
    memcpy (arp.x_name, ab.ar_name, 14);
#ifdef sparc
    arp.x_size = ((long)GET16(ab.ar_size[0])<<16) + (unsigned)GET16(ab.ar_size[1]);
#else
    arp.x_size = ab.ar_size;
#endif
    if (arp.x_size & 1)
        ++arp.x_size;
    off = ftell (af) + arp.x_size;
                                /* offset to next element */
    return (1);
}

#include        <stdio.h>
#include        <sys/types.h>
#include        <dir.h>
#include        <stat.h>
#include        <ctype.h>

#define         reg     register
#define         bool    char

#define         SYSTEM(str)     (pflag ? printf("%s\n", str) : system(str))

char            buf[BUFSIZ],
                pflag = 0,
                nflag = 0,
                wflag = 0;

main(ac, av)
int     ac;
char    *av[]; {

        reg char        *tsp, *msp, *csp, *man, *cat, *sp;
        reg FILE        *mdir, *inf;
        reg long        time;
        reg char        *sections, *dirman, *dirwhatis;
        reg int         exstat = 0;
        reg bool        changed = 0;
        char            *getenv ();
        static struct dir       dir;
        static struct stat      sbuf;

        while (ac > 1) {
                av++;
                if (strcmp(*av, "-p") == 0)
                        pflag++;
                else if (strcmp(*av, "-n") == 0)
                        nflag++;
                else if (strcmp(*av, "-w") == 0)
                        wflag++;
                else if (*av[0] == '-')
                        goto usage;
                else
                        break;
                ac--;
        }
        if (ac == 2)
                sections = *av;
        else if (ac < 2)
                sections = "12345678";
        else {
usage:
                printf("usage: catman [ -p ] [ -n ] [ -w ] [ sections ]\n");
                exit(-1);
        }
        if (wflag)
                goto whatis;
        if ((dirman = getenv ("MAN")) == NULL) dirman = "/mnt/man";
        if (access (dirman, 0) == 0)
                chdir(dirman);
        else {
                printf ("catman: cannot find %s\n", dirman);
                exit (-1);
        }
        man = "manx/xxxxxxxxxxxxxx";
        cat = "catx/xxxxxxxxxxxxxx";
        msp = &man[5];
        csp = &cat[5];
        umask(0);
        for (sp = sections; *sp; sp++) {
                man[3] = cat[3] = *sp;
                *msp = *csp = '\0';
                if ((mdir = fopen(man, "r")) == NULL) {
                        fprintf(stderr, "fopen:");
                        perror(man);
                        exstat = 1;
                        continue;
                }
                if (stat(cat, &sbuf) < 0) {
                        sprintf(buf, "mkdir %4.4s", cat);
                        SYSTEM(buf);
                        stat(cat, &sbuf);
                }
                if ((sbuf.st_mode & 0777) != 0777)
                        chmod(cat, 0777);
                while (fread((char *) &dir, sizeof dir, 1, mdir) > 0) {
                        if (dir.d_ino == 0 || dir.d_name[0] == '.')
                                continue;
                        /*
                         * make sure this is a man file, i.e., that it
                         * ends in .[0-9] or .[0-9][a-z]
                         */
                        tsp = rindex(dir.d_name, '.');
                        if (tsp == NULL)
                                continue;
                        if (!isdigit(*++tsp) || ((*++tsp && !isalpha(*tsp)) || *++tsp))
                                continue;

                        strncpy(msp, dir.d_name, DIRSIZ);
                        if ((inf = fopen(man, "r")) == NULL) {
                                fprintf(stderr, "fopen:");
                                perror(man);
                                exstat = 1;
                                continue;
                        }
                        if (getc(inf) == '.' && getc(inf) == 's'
                            && getc(inf) == 'o') {
                                fclose(inf);
                                continue;
                        }
                        fclose(inf);
                        strncpy(csp, dir.d_name, DIRSIZ);
                        if (stat(cat, &sbuf) >= 0) {
                                time = sbuf.st_mtime;
                                stat(man, &sbuf);
                                if (time >= sbuf.st_mtime)
                                        continue;
                                sprintf(buf, "rm -rf %s\n",cat);
                                SYSTEM(buf);
                        }
                        sprintf(buf, "pif -man %s > %s", man, cat);
                        SYSTEM(buf);
                        changed = 1;
                }
                fclose(mdir);
        }
        if (changed && !nflag) {
whatis:
                if ((dirwhatis = getenv ("WHATIS")) == NULL) dirwhatis = "/usr/lib/makewhatis";
                if (pflag)
                        printf("/bin/sh %s\n", dirwhatis);
                else {
                        execl("/bin/sh", "/bin/sh", dirwhatis, 0);
                        sprintf (buf,"/bin/sh %s", dirwhatis);
                        perror(buf);
                        exstat = 1;
                }
        }
        exit(exstat);
}

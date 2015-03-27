#include <stdio.h>

main ()
{
        register c, nwords, i, skip;
        register char *w;
        char str[256], word[128][32], buf[BUFSIZ], quote;

        setbuf (stdin, buf);
        nwords = skip = quote = 0;
        w = word[0];
        while ((c = getchar ()) != EOF) {
                if (c == '\t') c = ' ';
                if (c == '(' && !skip && !quote) {
                        quote = 1;
                        ungetc (c, stdin);
                        c = ',';
                }
                else if (quote) quote = 0;
                switch (c) {
                case '\n':
                        *w = '\0';
                        for (i = 0; i < (nwords - 1); i++)
                                printf (".xx %s %s%s \"%s\"\n",
                                word[i], word[0], word[nwords - 1], str);
                        nwords = skip = 0;
                        w = word[0];
                        break;
                case ' ':
                        if (skip) goto line;
                        break;
                case '-':
                case ',':
                        if (skip) goto line;
                        *w = '\0';
                        nwords++;
                        if (c == ',') w = word[nwords];
                        else w = str;
                        break;
                default:
line:
                        if (w == str)  skip = 1;
                        *w++ = c;
                        break;
                }
        }
}

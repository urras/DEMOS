#include <stdio.h>
#include <ediag.h>
FILE    *fin;
int     delim   = 0;
/* today's version assumes no delimiters;
they must be explicitly set
*/

main(argc, argv) char **argv; {

	while (argc > 1 && argv[1][0] == '-') {
		switch (argv[1][1]) {
		case 'd':
			delim = argv[1][2];
			break;
		}
		argc--;
		argv++;
	}
	if (argc <= 1)
		check(stdin);
	else
		while (--argc > 0) {
			if ((fin = fopen(*++argv, "r")) == NULL) {
				printf(ediag("Can't open %s\n",
					     "Не могу открыть %s\n"),*argv);
				exit(1);
			}
			printf("%s:\n", *argv);
			check(fin);
			fclose(fin);
		}
}

check(f)
FILE    *f;
{
	int start, line, eq, ndel, totdel;
	char in[600], *p;

	start = eq = line = ndel = totdel = 0;
	while (fgets(in, 600, f) != NULL) {
		line++;
		ndel = 0;
		for (p = in; *p; p++)
			if (*p == delim)
				ndel++;
		if (*in=='.' && *(in+1)=='E' && *(in+2)=='Q') {
			if (eq++)
				printf(ediag("   Spurious EQ, line %d\n",
					     "   Незаконный EQ, строка %d\n"),line);
			if (totdel)
				printf(ediag("   EQ in %c%c, line %d\n",
					     "   EQ в %c%c, строка %d\n"),
				delim, delim,line);
		} else if (*in=='.' && *(in+1)=='E' && *(in+2)=='N') {
			if (eq==0)
				printf(ediag("   Spurious EN, line %d\n",
					     "   Незаконный EN, строка %d\n"),line);
			else
				eq = 0;
			if (totdel > 0)
				printf(ediag("   EN in %c%c, line %d\n",
					     "   EN в %c%c, строка %d\n"), delim, delim, line);

			start = 0;
		} else if (eq && *in=='d' && *(in+1)=='e' && *(in+2)=='l' && *(in+3)=='i' && *(in+4)=='m') {
			for (p=in+5; *p; p++)
				if (*p != ' ') {
					if (*p == 'o' && *(p+1) == 'f')
						delim = 0;
					else {
						delim = *p;
						ndel = totdel = 0;
					}
					break;
				}
			if (delim == 0)
				printf(ediag("   Delim off, line %d\n",
					     "   Нет разделителя, строка %d\n"), line);
			else
				printf(ediag("   New delims %c%c, line %d\n",
					     "   Новые разделители %c%c, строка %d\n"),delim, delim, line);
		}
		if (ndel > 0 && eq > 0)
			printf(ediag("   %c%c in EQ, line %d\n",
				     "   %c%c в EQ, строка %d\n"), delim, delim, line);
		if (ndel == 0)
			continue;
		totdel += ndel;
		if (totdel%2) {
			if (start == 0)
				start = line;
			else {
				printf(ediag("    %d line %c%c, lines %d-%d\n",
					     "    %d строка %c%c, строки %d-%d\n"),
				line-start+1, delim, delim, start, line);
				start = line;
			}
		} else {
			if (start > 0) {
				printf(ediag("   %d line %c%c, lines %d-%d\n",
					     "   %d строка %c%c, строки %d-%d\n"),
				line-start+1, delim, delim, start, line);
				start = 0;
			}
			totdel = 0;
		}
	}
	if (totdel)
		printf(ediag("   Unfinished %c%c\n",
			     "   Неоконченные %c%c\n"), delim, delim);
	if (eq)
		printf(ediag("   Unfinished EQ\n", "   Неоконченный EQ\n"));
}

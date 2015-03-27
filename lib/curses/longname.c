# define        reg     register

/*
 * Получение полного имени терминала (def)
 * из строки описаний (bp).
 */

char *ErAsEd = -1;
char WhAtErAsEd;

char   *
	longname (bp, def)
	reg char   *bp,
		   *def; {

    reg char   *cp;

    while (*bp && *bp != ':' && *bp != '|')
	bp++;
    if (*bp == '|') {
	bp++;
	cp = bp;
	while (*cp && *cp != ':' && *cp != '|')
	    cp++;
/* падла собралась портить bp буфер. Запомним это... */
	ErAsEd = cp;
	WhAtErAsEd = *cp;
	*cp = 0;
	return bp;
    }
    return def;
}

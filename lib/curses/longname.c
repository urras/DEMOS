# define        reg     register

/*
 * ��������� ������� ����� ��������� (def)
 * �� ������ �������� (bp).
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
/* ����� ��������� ������� bp �����. �������� ���... */
	ErAsEd = cp;
	WhAtErAsEd = *cp;
	*cp = 0;
	return bp;
    }
    return def;
}

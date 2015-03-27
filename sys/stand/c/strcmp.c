/*
 * $Header: strcmp.c,v 1.1 86/12/30 19:29:16 root Exp $
 * Сравнение тестовых строк по значениям байт:
 *      s1>s2: >0  s1==s2: 0  s1<s2: <0
 * $Log:	strcmp.c,v $
 * Revision 1.1  86/12/30  19:29:16  root
 * Initial revision
 * 
 */

strcmp(s1, s2)
register char *s1, *s2;
{
	while (*s1 == *s2++)
		if (*s1++=='\0')
			return(0);
	return((unsigned) (*s1) - (unsigned) (*--s2));
}

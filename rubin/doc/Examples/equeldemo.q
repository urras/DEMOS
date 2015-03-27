/*
*  üÔÏ ĞÒÏÓÔÏÊ ĞÒÉÍÅÒ ĞÒÏÇÒÁÍÍÙ ÎÁ EQUEL. ïÎÁ ÒÁÂÏÔÁÅÔ Ó
*  ÏÔÎÏÛÅÎÉÅÍ óìõöáıéå × ÂÁÚÅ ÄÁÎÎÙÈ "demo".
*  ğÏÓÌÅ ×ÙÚÏ×Á ĞÒÏÇÒÁÍÍÙ ××ÅÄÉÔÅ ÆÁÍÉÌÉÀ ÓÌÕÖÁİÅÇÏ, É ĞÒÏÇÒÁÍÍÁ ÌÉÂÏ
*  ×ÙÄÁÓÔ ÅÇÏ ÏËÌÁÄ, ÌÉÂÏ ÓÏÏÂİÅÎÉÅ Ï ÔÏÍ, ŞÔÏ ÔÁËÏÇÏ ÓÌÕÖÁİÅÇÏ ÎÅÔ.
*  óĞÉÓÏË ÓÌÕÖĞİÉÈ ÍÏÖÎÏ ĞÏÌÕŞÉÔØ, ÎÁÂÒÁ× "?" × ÏÔ×ÅÔ ÎÁ ÚÁĞÒÏÓ ÉÍÅÎÉ.
*
*  äÌÑ ×ÙÈÏÄÁ ××ÅÄÉÔÅ CTRL/D.
*
*  ôÒÁÎÓÌÑÃÉÑ É ÚÁĞÕÓË ÜÔÏÊ ĞÒÏÇÒÁÍÍÙ ÎÁ ÓŞÅÔ ÏÓÕİÅÓÔ×ÌÑÀÔÓÑ
*  ËÏÍÁÎÄÁÍÉ:
*
*  equel equeldemo.q
*  cc    equeldemo.c -lq
*  a.out
*/
main(ac,av)
int ac;
##char *av[];
{
##	char	NAME[20];
##	int	SAL;
	char	flag;

	if ( ac != 2)
##      rubin demo
	else
##      rubin av[1]
##      range of e is ÓÌÕÖÁİÉÅ
	while (eread(NAME))
	{
		if(NAME[0] == '?')
		{
##                      retrieve (NAME=e.ÉÍÑ)
##			{
				printf("%s\n",NAME);
##			}
			continue;
		}
		flag = 0;
##              retrieve (SAL = e.ÏËÌÁÄ) where
##              e.ÉÍÑ = NAME
##		{
			printf("ôÏ×. %s ÉÍÅÅÔ ÏËÌÁÄ %d ÒÕÂ.\n",NAME,SAL);
			flag = 1;
##		}
	if(!flag) printf("ôÏ×. %s ÎÅÉÚ×ÅÓÔÅÎ ÓÉÓÔÅÍÅ\n",NAME);
	}
##	exit
}

eread(p)
char	*p;
{
	int    c;
	printf("÷×ÅÄÉÔÅ ÉÍÑ ÓÌÕÖÁİÅÇÏ:");
	while((c = getchar()) > 0)
	{
		if(c == '\n')
		{
			*p = 0;
			return(1);
		}
		*p++ = c;
	}
	return(0);
}

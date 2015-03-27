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
char*av[];
{
char NAME[20];int SAL;

	char	flag;

	if ( ac != 2)
{IIrubin("demo",0);}
	else
{IIrubin(av[1],0);}
{IIwrite("range of e=ÓÌÕÖÁİÉÅ");IIsync(0);}
	while (eread(NAME))
	{
		if(NAME[0] == '?')
		{
{IIwrite("retrieve(NAME=e.ÉÍÑ)");IIsetup();while(IIn_get(0)){IIn_ret(NAME,3);if(IIerrtest())continue;

				printf("%s\n",NAME);
}IIflushtup(0);IIsync(0);}
			continue;
		}
		flag = 0;
{IIwrite("retrieve(SAL=e.ÏËÌÁÄ)where e.ÉÍÑ=");IIcvar(NAME,3,0);IIwrite("");IIsetup();while(IIn_get(0)){IIn_ret(
&SAL,1);if(IIerrtest())continue;

			printf("ôÏ×. %s ÉÍÅÅÔ ÏËÌÁÄ %d ÒÕÂ.\n",NAME,SAL);
			flag = 1;
}IIflushtup(0);IIsync(0);}
	if(!flag) printf("ôÏ×. %s ÎÅÉÚ×ÅÓÔÅÎ ÓÉÓÔÅÍÅ\n",NAME);
	}
{IIexit();}
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

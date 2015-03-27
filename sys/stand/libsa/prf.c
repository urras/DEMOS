/*
 * Процедуры ввода и вывода на консоль
 *
 * $Log:	prf.c,v $
 * Revision 1.7  88/11/02  22:39:55  dvolodin
 * *** empty log message ***
 * 
 * Revision 1.6  88/03/28  11:56:34  korotaev
 * После слияния с AZLK.
 *
 * Revision 1.5  87/05/17  18:34:18  avg
 * Интервал задержки увеличен в 10 раз: на Э79 это порядка 30 сек.
 *
 * Revision 1.4  87/04/24  09:37:03  dmitry
 * При установленном флажке boottmout в getchar будет
 * выдерживаться интервал порядка 10 сек (на 1420 с большой памятью).
 * По истечении этого времени, getchar возвратит '\n'.
 *
 * Revision 1.3  86/11/22  22:10:09  alex
 * Добавлен режим работы с русскими буквами и
 * работа на терминалах с LCASE (см. флаг LCASECO).
 *
 * Revision 1.1  86/07/14  22:00:52  avg
 * Initial revision
 *
 */

#define LCASECO
/*
 * Упрощенная версия printf.
 * Реализованы только форматы %s %u %d (==%u) %o %x и %D.
 * Т.к. прерывания не обслуживаются,
 * процессор будет ждать завершения В/В.
 */
printf(fmt, x1)
register char *fmt;
unsigned x1;
{
	register c;
	register unsigned int *adx;
	char *s;

	adx = &x1;
loop:
	while((c = *fmt++) != '%') {
		if(c == '\0')
			return;
		putchar(c);
	}
	c = *fmt++;
	if(c == 'd' || c == 'u' || c == 'o' || c == 'x')
		printn((long)*adx, c=='o'? 8: (c=='x'? 16:10));
	else if(c == 's') {
		s = (char *)*adx;
		while(c = *s++)
			putchar(c);
	} else if (c == 'D' || c == 'O') {
		printn(*(long *)adx, c == 'D' ?  10 : 8);
		adx += (sizeof(long) / sizeof(int)) - 1;
	} else if (c == 'c')
		putchar((char *)*adx);
	adx++;
	goto loop;
}

/*
 * Печатать беззнаковое число в системе счисления b.
 */
printn(n, b)
long n;
{
	register long a;

	if (n<0) {      /* не бывает */
		putchar('-');
		n = -n;
	}
	if(a = n/b)
		printn(a, b);
	putchar("0123456789ABCDEF"[(int)(n%b)]);
}

struct  device  {
	int     rcsr,rbuf;
	int     tcsr,tbuf;
};
struct  device  *KLADDR {0177560};
int outrus;
#define isalpha(c) (c>='a' && c<='z' || c>='A' && c<='Z' || (unsigned)c>=0300&&(unsigned)c<=0376)
#define islat(c) (c>='a' && c<='z' || c>='A' && c<='Z' )
#define iscirill(c) ((unsigned)c>=0340&&(unsigned)c<=0376)
#define islower(c) (c>='a' && c<='z' || (unsigned)c>=0300&&(unsigned)c<=0337)
#define toupper(c) ((c)^040)

#define CLAT 017
#define CRUS 016
putchar(c)
register c;
{
	register s;
	register unsigned timo;
	extern int video;

	c &= 0377;
	if( video ) {
		v_putchar(c);
		if( c == '\n' )
			v_putchar( '\r' );
		return;
	}
	timo = 60000;
	/*
	 * Ждать готовности консоли (но в течение разумного  времени)
	 */
	while((KLADDR->tcsr&0200) == 0)
		if(--timo == 0)
			break;
	if(c == 0)
		return;
/* Обработка регистров вывода (lcase, cyrill) */
#ifdef LCASECO
	if(islower(c)) c = toupper(c);
#endif
	if(iscirill(c)&&!outrus) {putchar(CRUS); }
	if(islat(c)&&outrus) {putchar(CLAT); }
	if(c==CLAT) outrus=0;
	if(c==CRUS) outrus=1;   c &= 0177;
	s = KLADDR->tcsr;
	KLADDR->tcsr = 0;
	KLADDR->tbuf = c;
	if(c == '\n') {
		putchar('\r');
		putchar(0200);
		putchar(0200);
		putchar(0200);
		putchar(0200);
	}
	putchar(0);
	KLADDR->tcsr = s;
}

int boottmout = 0 ;

getchar()
{
	register c;
	extern int video;
	if(outrus) putchar(CLAT);
	if( video ) {
		c = v_getchar();
	} else {
		KLADDR->rcsr = 1;
		if( boottmout ) {
			int i = 10;
			long l;
			while( --i ) {
				l = 1500000 ;
				while((KLADDR->rcsr&0200)==0 && --l );
				if( l != 0l ) break;
			}
			if( l == 0l ) c = '\n' ;
			else boottmout = 0, c = KLADDR->rbuf&0177;
		} else {
			while((KLADDR->rcsr&0200)==0);
			c = KLADDR->rbuf&0177;
		}
	}
	if (c=='\r')
		c = '\n';
	if( c != 0177 )
	putchar(c);
	return(c);
}

gets(buf)
char    *buf;
{
register char *lp;
register c;

	lp = buf;
	for (;;) {
		c = getchar() & 0177;
		if (c>='A' && c<='Z')
			c -= 'A' - 'a';
		if (lp != buf && *(lp-1) == '\\') {
			lp--;
			if (c>='a' && c<='z') {
				c += 'A' - 'a';
				goto store;
			}
			switch ( c ) {
			case '(':
				c = '{';
				break;
			case ')':
				c = '}';
				break;
			case '!':
				c = '|';
				break;
			case '^':
				c = '~';
				break;
			case '\'':
				c = '`';
				break;
			}
		}
	store:
		switch(c) {
		case '\n':
		case '\r':
			c = '\n';
			*lp++ = '\0';
			return;
		case 0177:
			printf( "\10 \10" );
		case '\b':
		case '#':
			lp--;
			if (lp < buf)
				lp = buf;
			continue;
		case '@':
		case 025:
			lp = buf;
			putchar('\n');
			continue;
		default:
			*lp++ = c;
		}
	}
}


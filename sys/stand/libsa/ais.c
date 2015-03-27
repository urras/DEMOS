/*
 * ������������ ����������� ��������� �� ���������� AIS
 * (����� ���� ������ �� �1630)
 *
 * $Header: ais.c,v 1.1 88/11/15 13:10:14 dvolodin Exp $
 * $Log:	ais.c,v $
 * Revision 1.1  88/11/15  13:10:14  dvolodin
 * Initial revision
 * 
 * Revision 1.1  88/08/04  15:52:31  sys
 * Initial revision
 * 
 * Revision 1.1  88/08/04  15:38:26  sys
 * 1630
 * 
 */

#define AIS  ((struct ais *)0177500)

struct ais {
	short   aisdat;
	short   aiscsr;
};

#define RESTRI  050
#define SR1     001
#define RECCHAR 001
#define TRBE    004

static aisstop;

a_putchar(c)
{
	register s;

	s = AIS->aiscsr;
	while( (AIS->aiscsr & TRBE) == 0 ) ;
	AIS->aiscsr = SR1 | RESTRI;
	s = 0;
	AIS->aiscsr = s;
	while( AIS->aiscsr & RECCHAR ) {
		s = AIS->aisdat;
		s &= 0177;
		if( s == 023 ) { /* ctrl/s */
			aisstop++;
			break;
		}
	}
	while( aisstop ) {
		while( (AIS->aiscsr & RECCHAR) == 0 ) ;
		s = AIS->aisdat;
		s &= 0177;
		if( s == 021 ) /* ctrl/q */
			aisstop = 0;
	}
	AIS->aisdat = c;
	while( (AIS->aiscsr & TRBE) == 0 ) ;
}

a_getchar()
{
	register c;
	extern boottmout;

	for(;;) {
		if( boottmout ) {
			int i = 10;
			long l;
			while( --i ) {
				l = 1500000 ;
				while((AIS->aiscsr&RECCHAR)==0 && --l );
				if( l != 0l ) break;
			}
			if( l == 0l ) return '\n' ;
			else boottmout = 0;
		} else {
			while( (AIS->aiscsr & RECCHAR) == 0 ) ;
		}
		c = AIS->aisdat;
		c &= 0177;
		if( c == 021 )
			aisstop = 0;
		else if( c == 023 )
			aisstop++;
		else
			return(c);
	}
}

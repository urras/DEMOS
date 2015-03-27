/*
 * RW disk driver
 *
 * $Log:	rw.c,v $
 * Revision 1.4  89/03/10  13:29:55  avg
 * ����� ��������� ����� �����������, �.�. �� ������ �������� � �������
 * �� Trap � Bus Error-�� �� ������� �������� ��..��� ������� �����������
 * (�������� �������� � ���� �� ����� ���������� ��������)>
 * >
 * 
 * Revision 1.3  87/03/04  15:03:17  avg
 * �������� �������� �����������.
 * 
 * Revision 1.2  87/03/03  22:29:35  avg
 * ������������� ���������� ������ ��� �����������-85.
 *
 * Revision 1.1  86/09/01  21:45:41  avg
 * Initial revision
 *
 *
 */

#include <sys/param.h>
#include <sys/inode.h>
#include "../saio.h"

#define R(x) (*((int *)(x)))

char worka[512];     /* ������� ������� */

rwstrategy(io, func)
register struct iob *io;
{
	daddr_t bn;
	int  cn, sn, tn;
	unsigned ma, seg = segflag & 3;
	register *wwp, i;
	register unsigned cc;

	ma = io->i_ma;
	bn = io->i_bn;
	sn = bn%16;
	bn /= 16;
	tn = bn%4;
	cn = bn/4;
	cc = io->i_cc;

	while( cc > 0 ) {
		if( func != READ ) {
			/* ������ ���� �� ��� */
			i = cc;
			if( i > 512 ) i = 512;
			cop_from( seg, ma, i/2, worka );
			cc -= i;
			if( ma+i < ma ) seg++;
			ma += i;
		}

		/* ����� ���������� ����������� */
ELoop:
		while( R(0174020) & 0100000 );
		wwp = worka;

		/* ��������� �������� */
		R(0174006) = sn;
		R(0174012) = cn;
		R(0174014) = tn;
		R(0174016) = (func==READ) ? 040 : 060;

		if( func == READ ) {
			for ( i = 256 ; i ; i-- ) {
				while( (R(0174020) & 0200) == 0 );
				*wwp++ = R(0174010);
			}
		} else {
			for ( i = 256 ; i ; i-- ) {
				while( (R(0174020) & 0200) == 0 );
				R(0174010) = *wwp++;
			}
		}
		while( R(0174020) & 0100000 );
		if( R(0174016) & 020400 ) {
			printf( "RW ERR BN %d ER=%o\n", (int)(io->i_bn), R(0174004) );
			goto ELoop;
		}

		if( func == READ ) {
			/* ������ ���� � ��� */
			i = cc;
			if( i > 512 ) i = 512;
			cop_to( seg, ma, i/2, worka );
			cc -= i;
			if( ma+i < ma ) seg++;
			ma += i;
		}
		if( ++sn == 16 ) {
			sn = 0;
			if( ++tn == 4 )
				tn = 0, cn++;
		}
	}
	return(io->i_cc);
}

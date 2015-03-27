/*
 * ����� driver ���������� ��.85 � ���-4, �� ������� �������� RW (avg)
 *
 * $Log:	dw.c,v $
 * Revision 1.1  90/12/12  17:11:57  korotaev
 * Initial revision
 * 
 * Revision 1.2  89/01/12  17:56:15  chech
 * ����� ������ RW � DW.
 * �����������, ��������� ������.
 * 
 * Revision 1.1  88/12/12  14:02:28  chech
 * Initial revision
 * 
 * �����: ����� �.�. & Co (���, ���.���.)
 * ������� ������������, ��� �� �������� � �������� "segflag",
 * � ������� �������������� ��������.
 * ��� ������������� ����� ��� boot (������������ ��-��, ���������� segflag)
 */

#include <sys/param.h>
#include <sys/inode.h>
#include "../saio.h"

#define R(x) (*((int *)(x)))

dwstrategy(io, func)
struct iob *io;
{
	daddr_t bn;
	int  cn, sn, tn;
	register wc, *ma, i;
	int nerr = 10;

	ma = io->i_ma;
	bn = io->i_bn;
	sn = bn%16;
	bn /= 16;
	tn = bn%4;
	cn = bn/4;
	wc = io->i_cc >> 1;

	while( wc > 0 ) {
ELoop:
		/* ����� ���������� ����������� */
		bn = R(0174000);
		while( R(0174020) & 0100000 );

		/* ��������� �������� */
		R(0174006) = sn;
		R(0174012) = cn;
		R(0174014) = tn;
		R(0174016) = (func==READ) ? 040 : 060;

		if( func == READ ) {
			for ( i = 256 ; --i >= 0; ) {
				while( (R(0174020) & 0200) == 0 );
				if (--wc >= 0)
					*ma++ = R(0174010);
				else
					bn = R(0174010);

			}
		} else {
			for ( i = 256 ; --i >= 0; ) {
				while( (R(0174020) & 0200) == 0 );
				R(0174010) = ((--wc >= 0) ? *ma++ : 0);
			}
		}
		while( R(0174020) & 0100000 );
		if( R(0174016) & 020400 ) {
			printf( "RW ERR BN %d ER=%o\n", (int)(io->i_bn), R(0174004) );
			if (--nerr > 0)
				goto ELoop;
			printf("fatal!\n");
			return (-1);
		}

		if( ++sn == 16 ) {
			sn = 0;
			if( ++tn == 4 )
				tn = 0, cn++;
		}
	}
	return(io->i_cc);
}

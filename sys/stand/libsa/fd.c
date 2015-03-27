/*
 * FD floppy disk driver
 *
 * $Log:	fd.c,v $
 * Revision 1.2  87/03/04  15:10:17  avg
 * Добавлена работа с несколькими устройствами.
 * 
 * Revision 1.1  87/03/04  15:06:57  avg
 * Initial revision
 *
 */

#include <sys/param.h>
#include <sys/inode.h>
#include "../saio.h"

#define R(x) (*((int *)(x)))

static char worka[512];     /* рабочая область */

fdstrategy(io, func)
register struct iob *io;
{
	int  bn, unit;
	unsigned ma, seg = segflag & 3;
	register i;
	register char *wwp;
	register unsigned cc;

	unit = io->i_unit << 1;
	ma = io->i_ma;
	bn = (int)(io->i_bn);
	cc = io->i_cc;

	while( cc > 0 ) {
		if( func != READ ) {
			/* Читать блок из ОЗУ */
			i = cc;
			if( i > 512 ) i = 512;
			cop_from( seg, ma, i/2, worka );
			cc -= i;
			if( ma+i < ma ) seg++;
			ma += i;

		}

		/* Ждать готовность контроллера */
		while( (R(0174204) & 010) == 0 ) ;

		wwp = worka;
		if( func != READ ) {
			R(0174222) = 0;
			for ( i = 512 ; i ; i-- )
				R(0174220) = *wwp++;
		}

		/* Вычислить интерливинг */
		R(0174210) = (bn*3)%10 + 1;
		R(0174206) =  bn   /10 + 1;

		/* Запустить операцию */
		if( func == READ ) {
			R(0174204) = 0130|unit; /* EXCOM */
			R(0174216) = 0;         /* READ + repeat */
		} else
			R(0174204) = 0170|unit; /* WRITE */
		R(0174224) = 0;

		/* Ждать готовность контроллера */
		while( (R(0174204) & 010) == 0 ) ;

		if( func == READ ) {
			R(0174222) = 0;
			for ( i = 512 ; i ; i-- )
				*wwp++ = R(0174220);

			/* Писать блок в ОЗУ */
			i = cc;
			if( i > 512 ) i = 512;
			cop_to( seg, ma, i/2, worka );
			cc -= i;
			if( ma+i < ma ) seg++;
			ma += i;
		}
		bn++;
	}
	return(io->i_cc);
}

/*
 * Установка текущей позиции - ЭМ-7052
 * by @VG
 */

int _draws;

move(x, y)
{
	_convert(&x, &y);
	printf( "MA%u,%u", x, y );
	if( _draws == 2 )
		_draws = 1;
}

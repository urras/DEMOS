/*
 * Рисование текста - ЭМ-7052
 * by @VG
 */

int _draws;

label(s)
register char *s;
{
	_draws = 2;             /* was a text */
	if( _draws != 2 )
		printf("MR-80,-100\n"); /* center char */
	printf("T%s\3\n", s);      /* Write Text */
}

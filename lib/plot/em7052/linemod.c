/*
 * Установка типа линии - ЭМ-7052
 * by @VG
 */

linemod(s)
char *s;
{
	char *p;

	if(      !strcmp(s, "dotted") )
		p = "SL1,80";
	else if( !strcmp(s, "dotdashed") )
		p = "SL3,400";
	else if( !strcmp(s, "shortdashed") )
		p = "SL1,200";
	else if( !strcmp(s, "longdashed") )
		p = "SL1,400";
	else
		p = "SL0,400";
	printf(p);
}

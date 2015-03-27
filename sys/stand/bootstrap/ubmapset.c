/*
 * Установка регистров карты ОШ
 *
 * $Log:	ubmapset.c,v $
 * Revision 1.1  86/07/14  20:14:13  avg
 * Initial revision
 * 
 */

#define UBMAP   ((physadr)0170200)

typedef struct { int r[1]; } *physadr;
extern char ubmap;

ubmapset()
{
	unsigned register i;

	if(ubmap)
	for(i=0; i<62; i+=2) {
		UBMAP->r[i] = i<<12;
		UBMAP->r[i+1] = i>>4;
	}
}

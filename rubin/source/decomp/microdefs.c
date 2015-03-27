# include	<defines.h>
# ifdef MICRO
#undef NULL
#include <stdio.h>

char _m_defs      = 0;
char    _sibuf[1] = 0;
char    _sobuf[BUFSIZ];

#if pdp11
struct	_iobuf	_iob[_NFILE] ={
	{  NULL, 0, NULL, _IOREAD+_IONBF,0},
	{  NULL, 0, NULL, _IOWRT, 1},
	{  NULL, 0, NULL, _IOWRT+_IONBF, 2},
};
#else
struct	_iobuf	_iob[_NFILE] ={
	{ 0, NULL, NULL, NULL, _IOREAD+_IONBF,0),
	{ 0, NULL, NULL, NULL, _IOWRT, 1},
	{ 0, NULL, NULL, NULL, _IOWRT+_IONBF, 2},
};
#endif
/*
 * Ptr to end of buffers
 */
struct	_iobuf	*_lastbuf ={ &_iob[_NFILE] };

# include	<aux.h>
# include	<access.h>


/*
**	SYSTEM RELATION DESCRIPTOR CACHE DEFINITION
**
*/

DESC	Reldes;
DESC	Attdes;
DESC	Inddes;


struct desxx	Desxx[] =
{
	"relation",	&Reldes,	&Admin.adreld,
	"attribute",	&Attdes,	&Admin.adattd,
	"indexes",	&Inddes,	NULL,
	NULL
};

#endif /* MICRO */

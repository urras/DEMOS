/*
 * $Log:	c.c,v $
 * Revision 1.7  90/12/12  17:10:39  korotaev
 * Вставлены my и dw для ДВК.
 * 
 * Revision 1.6  89/06/12  16:39:51  korotaev
 * Вставлен флаг: badflg.
 * 
 * Revision 1.5  88/11/02  21:37:37  dvolodin
 * *** empty log message ***
 *
 * Revision 1.4  88/03/28  10:26:39  korotaev
 * После слияния с AZLK.
 *
 * Revision 1.3  87/03/04  15:27:50  avg
 * *** empty log message ***
 *
 * Revision 1.2  87/03/04  14:20:15  avg
 * Добавлен драйвер fd (Э-85).
 *
 * Revision 1.1  86/07/14  19:14:17  avg
 * Initial revision
 *
 */

#include <sys/param.h>
#include <sys/inode.h>
#include <saio.h>

devread(io)
register struct iob *io;
{

	return( (*devsw[io->i_ino.i_dev].dv_strategy)(io,READ) );
}

devwrite(io)
register struct iob *io;
{
	return( (*devsw[io->i_ino.i_dev].dv_strategy)(io, WRITE) );
}

devopen(io)
register struct iob *io;
{
	(*devsw[io->i_ino.i_dev].dv_open)(io);
}

devclose(io)
register struct iob *io;
{
	(*devsw[io->i_ino.i_dev].dv_close)(io);
}

nullsys()
{ ; }

int     xpstrategy();
int     rastratagy(), raopen();
int     rpstrategy();
int     rkstrategy();
int     hkstrategy();
int     rlstrategy();
/* int     rwstrategy();        */
int     dwstrategy();
int     fdstrategy();
int     mystrategy();
int     nullsys();
int     tmstrategy(), tmrew(), tmopen();
int     tsstrategy(), tsopen(), tsclose();
/*
int     htstrategy(), htopen(), htclose();
int     rmstrategy();
 */
struct devsw devsw[] {
	"ra",   rastratagy,     raopen,         nullsys,
	"rp",   rpstrategy,     nullsys,        nullsys,

	/* "hp" и "rm" - синонимы "xp" */
	"hp",   xpstrategy,     nullsys,        nullsys,
	"rm",   xpstrategy,     nullsys,        nullsys,
	"xp",   xpstrategy,     nullsys,        nullsys,

	"rk",   rkstrategy,     nullsys,        nullsys,
	"hk",   hkstrategy,     nullsys,        nullsys,
	"rl",   rlstrategy,     nullsys,        nullsys,
	"tm",   tmstrategy,     tmopen,         tmrew,
	"ts",   tsstrategy,     tsopen,         tsclose,
/*        "rw",   rwstrategy,     nullsys,        nullsys,      */
	"dw",   dwstrategy,     nullsys,        nullsys,
	"rw",   dwstrategy,     nullsys,        nullsys,
	"fd",   fdstrategy,     nullsys,        nullsys,
/*
	"ht",   htstrategy,     htopen,         htclose,
	"rm",   rmstrategy,     nullsys,        nullsys,
 */
	"my",   mystrategy,     nullsys,        nullsys,
	0,0,0,0
};

char badflg;    /* только для boot'а */

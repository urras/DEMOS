# include	"monitor.h"
# include	<defines.h>
# include	<version.h>
# include	<opsys.h>
# include	<pv.h>
# include	<func.h>
# include	<signal.h>
# include	<pipes.h>
# include	<setjmp.h>
# include	<sccs.h>

/*
 * ��������� ������������� �������� ��� ������ ��� ����������� � SQL
 * � ��� �������� �������� �� ������ ������� � ��������� �������������
 * SQL
 */

extern		tm_mon();
extern		tm_init();
extern		tm_intr();
short		tTttymon[100];

struct fn_def	TtyMonFn =
{
	"MONITOR",
	tm_mon,
	tm_init,
	tm_intr,
	NULL,
	0,
	tTttymon,
	100,
	'M',
	0
};

int SQL_flag = 0; /* ������� - ���� �� SQL ����� */

sql_init(argc, argv)
int argc;
char *argv[];
{
return(-1);
}

sql_quit()
{
return;
}

/**  ACC_INIT, PAGEFLUSH -- dummy access method routines
**
**	Since the CM wants to do some basic access method functions,
**	we will let it.
*/

acc_init()
{
}

pageflush(x)
char	*x;
{
	return (0);
}
/*
**  CLOSECATALOG -- dummy catalog close routine.
**
**	To keep from loading access methods.
*/

closecatalog()
{
}

/*
 * ������� ��� ��������� ���������� � ���������� ������� �� SQL
 */
compile_go()
{
}

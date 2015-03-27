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
 * ��������� ������������� �������� ��� ������ � ������������ SQL
 * ����� ����� ��������� ������������� SQL
 */

extern		tm_mon();
extern		tm_init();
extern		tm_intr();
short		tTttymon[100];

struct fn_def   TtySQLFn =
{
	"MONITORSQL",
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

int SQL_flag = 1; /* ������� - ���� �� SQL ����� */
int Readonly = 1; /* ������� ��� readadmin - ��������� ����. ��������� �� ��. */

/*
 * sql_init(ac, av)
 * ������������� SQL - �����������
 * ��������� ������� ����� (� ������ ������ - ����� ��������������)
 */
sql_init(argc, argv)
int	argc;
char	*argv[];
{
    concat("/tmp/RBNS", Fileset, QbnameSQL);
    if ((QryiopSQL = fopen(QbnameSQL, "w")) == NULL)           /* YNP */
        syserr("main: open(%s)", QbnameSQL);                /* YNP */
    return(0);
}

/*
 * sql_quit() - �������� ���� ������ SQL
 * ��������� � ���������� ������� �����
 */
sql_quit()
{
    fclose(QryiopSQL);
    unlink(QbnameSQL);
    return;
}

/*
 * compile_go
 * - ���������  ������ �� SQL
 */
compile_go()
{
	clrline(1);
	fflush(Qryiop);
	if (!sql_quel(QbnameSQL,QbnameQUEL))
	{
		Qryiop=QryiopQUEL;
		Qbname=QbnameQUEL;
		go(0);
		putchar('\n');
		Qryiop=QryiopSQL;
		Qbname=QbnameSQL;
	}
	cgprompt();
}

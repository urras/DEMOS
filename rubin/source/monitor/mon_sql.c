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
 * Заголовок терминального монитора для сборки с транслятором SQL
 * Здесь сидят программы инициализации SQL
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

int SQL_flag = 1; /* Признак - есть ли SQL режим */
int Readonly = 1; /* Признак для readadmin - открывать сист. отношения на чт. */

/*
 * sql_init(ac, av)
 * инициализация SQL - транслятора
 * открывает рабочие файлы (в данном случае - буфер редактирования)
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
 * sql_quit() - закрытие всей работы SQL
 * закрывает и уничтожает рабочие файлы
 */
sql_quit()
{
    fclose(QryiopSQL);
    unlink(QbnameSQL);
    return;
}

/*
 * compile_go
 * - выполнить  запрос на SQL
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

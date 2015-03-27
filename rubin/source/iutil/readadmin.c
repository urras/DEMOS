# include	<defines.h>
# include	<access.h>
# include	<aux.h>
# include	<lock.h>
# include	<sccs.h>

SCCSID(@(#)readadmin.c	7.1	2/5/81)

/*
**  READADMIN -- читать файл admin в структуру Admin
**
**      Файл ищется в справочнике "Dbpath" или, если !Dbpath, в текущем
**      справочнике. Файл содержит следующую информацию:
**
**      Заголовок, в котором хранится DBA и набор битов статуса базы в целом.
**      Эти биты определены в "aux.h"
**      Заголовок включает также поля, которые описывают код версии и
**      длину заголовка.
**
**      Дальше следуют дескрипторы отношений "relation"  и "attribute".
**      В них остаются незаполненными только поля "relfp" и "relopn".
**      Данная функция открывает эти отношения и заполняет указанные поля.
**
**      Режим открытия системных отношения определяется содержимым
**      переменной "Readonly" - если не 0, отношения открываются
**      только на чтение (а иначе на чтение и запись). Это сделано
**      для того, чтобы системной информацией мог пользоваться монитор,
**      который работает от имени пользователя.
**
**	WARNING:
**              Эта программа определена иначе в "creatdb", изменять их
**              нужно синхронно.
**
**	Parameters:
**		none
**
**	Returns:
**		none
**
**	Side Effects:
**      Заполняется структура 'Admin'
**      Открываются системные отношения 'relation....xx' и 'attribute...xx'
**
**	Files:
**		./admin
**
**	Trace Flags:
**		none
*/

int     Readonly;       /* Если 1 - открывать на чтение */
readadmin()
{
	register int	i;
	char            relname[MAXNAME + 5];
	extern long	lseek();
	extern char *Dbpath;
	extern char *ztack();
	/* read the stuff from the admin file */
	if (!Dbpath) Dbpath = ".";
		i = open(ztack(Dbpath,"/admin"),0);
	if (i < 0)
		syserr("readadmin: open admin %d", i);
	checkadmin(i);
	close(i);
	relname[0] = '/';   /* Для ztack */
	/* open the physical files for 'relation' and 'attribute' */
	rubinname(Admin.adreld.reldum.relid, Admin.adreld.reldum.relowner, relname+1);
	if ((Admin.adreld.relfp = open(ztack(Dbpath,relname), Readonly?0:2)) < 0)
		syserr("readadmin: open rel %d", Admin.adreld.relfp);
	rubinname(Admin.adattd.reldum.relid, Admin.adattd.reldum.relowner, relname+1);
	if ((Admin.adattd.relfp = open(ztack(Dbpath,relname), Readonly?0:2)) < 0)
		syserr("readadmin: open att %d", Admin.adattd.relfp);
	Admin.adreld.relopn = (Admin.adreld.relfp + 1) * -5;
	/* we just want to read here create, modify and destroy fix it up */
	Admin.adattd.relopn = (Admin.adattd.relfp + 1) * 5;

	return;
}

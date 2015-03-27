# include	<defines.h>
# include	<access.h>
# include	<aux.h>
# include	<lock.h>
# include	<sccs.h>

SCCSID(@(#)readadmin.c	7.1	2/5/81)

/*
**  READADMIN -- ������ ���� admin � ��������� Admin
**
**      ���� ������ � ����������� "Dbpath" ���, ���� !Dbpath, � �������
**      �����������. ���� �������� ��������� ����������:
**
**      ���������, � ������� �������� DBA � ����� ����� ������� ���� � �����.
**      ��� ���� ���������� � "aux.h"
**      ��������� �������� ����� ����, ������� ��������� ��� ������ �
**      ����� ���������.
**
**      ������ ������� ����������� ��������� "relation"  � "attribute".
**      � ��� �������� �������������� ������ ���� "relfp" � "relopn".
**      ������ ������� ��������� ��� ��������� � ��������� ��������� ����.
**
**      ����� �������� ��������� ��������� ������������ ����������
**      ���������� "Readonly" - ���� �� 0, ��������� �����������
**      ������ �� ������ (� ����� �� ������ � ������). ��� �������
**      ��� ����, ����� ��������� ����������� ��� ������������ �������,
**      ������� �������� �� ����� ������������.
**
**	WARNING:
**              ��� ��������� ���������� ����� � "creatdb", �������� ��
**              ����� ���������.
**
**	Parameters:
**		none
**
**	Returns:
**		none
**
**	Side Effects:
**      ����������� ��������� 'Admin'
**      ����������� ��������� ��������� 'relation....xx' � 'attribute...xx'
**
**	Files:
**		./admin
**
**	Trace Flags:
**		none
*/

int     Readonly;       /* ���� 1 - ��������� �� ������ */
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
	relname[0] = '/';   /* ��� ztack */
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

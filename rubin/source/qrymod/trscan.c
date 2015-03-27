# include	<defines.h>
# include	<symbol.h>
# include	<tree.h>
# include	"qrymod.h"
# include	<sccs.h>

SCCSID(@(#)trscan.c	7.1	2/5/81)

/*
**  AGGCHECK -- check for any aggregate in subtree.
**
**	This routine checks to insure that the view algorithm can
**	proceed safely by checking for aggregates in the view tree.
**
**	Parameters:
**		root -- the root of the tree to check.
**
**	Returns:
**		TRUE -- an aggregate found.
**		FALSE -- no aggregates in tree.
**
**	Side Effects:
**		none
**
**	Trace Flags:
**		none.
*/

aggcheck(root)
QTREE	*root;
{
	register QTREE	*t;

	t = root;

	/* check for no pointer */
	while (t != NULL)
	{
		/* check for this node an AGHEAD */
		if (t->sym.type == AGHEAD)
			return (TRUE);

		/* check left subtree recursively */
		if (aggcheck(t->left))
			return (TRUE);
		
		/* check right subtree iteratively */
		t = t->right;
	}

	return (FALSE);
}
/*
**  VFIND -- find definition for attribute in view tree
**
**	The view tree is scanned for a specified RESDOM; a pointer
**	to the value is returned.
**
**	Parameters:
**		vn -- the variable number to dig out.
**		vtree -- a pointer to the target list of the
**			view definition tree.
**
**	Returns:
**		a pointer to the substitution value for the specified
**			'vn'.
**		NULL -- if not found.
**
**	Side Effects:
**		none
**
**	Trace Flags:
**		none.
*/

QTREE *
vfind(vn, vtree)
int	vn;
QTREE	*vtree;
{
	register int	n;
	register QTREE	*v;

	n = vn;

	for (v = vtree; v->sym.type == RESDOM; v = v->left)
	{
		if (v->sym.value.sym_resdom.resno != n)
			continue;

		/* found the correct replacement */
		return (v->right);
	}

	if (v->sym.type != TREE)
		syserr("vfind: bad RESDOM node %d", v->sym.type);
	return (NULL);
}
/*
**  QSCAN -- find specified VAR node in subtree
**
**	Intended for finding a variable in a qualification, this
**	routine just scans a tree recursively looking for a node
**	with the specified VAR (varno.attno) node.
**
**	Parameters:
**		root -- the root of the tree to scan.
**		vn -- the varno to scan for.
**		an -- the attno to scan for.
**
**	Returns:
**		A pointer to the first found VAR node which matches.
**			Scan is prefix.
**		NULL if not found at all.
**
**	Side Effects:
**		none
**
**	Trace Flags:
**		none.
*/

QTREE *
qscan(root, vn, an)
QTREE	*root;
int	vn;
int	an;
{
	register QTREE	*t;
	register QTREE	*u;

	t = root;

	/* check for null node */
	if (t == NULL)
		return (t);

	/* check to see if this node qualifies */
	if (t->sym.type == VAR && t->sym.value.sym_var.varno == vn && t->sym.value.sym_var.attno == an)
		return (t);

	/* check other nodes */
	if ((u = qscan(t->left, vn, an)) != NULL)
		return (u);
	return (qscan(t->right, vn, an));
}
/*
**  VARSET -- scan tree and set a bit vector of variables
**
**	The return value is a bit vector representing the set of
**	variables used in that subtree.
**
**	Parameters:
**		root -- the root of the tree to check.
**
**	Returns:
**		A bit vector, such that bit zero (on the low order,
**			right-hand end) means var zero.
**
**	Side Effects:
**		none
**
**	Trace Flags:
**		none
*/

varset(root)
QTREE	*root;
{
	register QTREE	*t;
	register int	s;

	t = root;

	if (t == NULL)
		return (0);

	/* scan left and right branches */
	s = varset(t->left);
	s |= varset(t->right);

	/* check out this node */
	if (t->sym.type == VAR)
	{
		/* or in bit corresponding to this varno */
		s |= 1 << t->sym.value.sym_var.varno;
	}

	return (s);
}
/*
**  SUBSVARS -- сканировать дерево и заменять VAR узлы на новое дерево
**
**      Сканирует tree и заменяет все VAR узлы для переменной vn.
**      Эти узла находятся в дереве и заменяются на новое дерево transtree.
**      В случае view соответствующие узлы должны существовать; в остальных
**      случаях создается и вставляется нулевой узел.
**
**      Это одна из двух основных частей обработки view.
**
**      VAR nodes исключаются и заменяются определениями для view.
**      При этом проблем тут быть не может, так как VAR встречается
**      только в контексте retrieve, причем все проверки уже сделаны.
**
**      Делается дополнительная обработка  RESDOM nodes при
**      resno = 0.  Такие узлы специфицируют tid, и
**      вставляются парсером при  REPLACE и DELETE для Декомпа.
**      Сгимфкы допускает такую конструкцию при условии, что attno = 0;
**      В этом случае varno заиеняется на Qt.qt_resvar.
**      Обэясняется это тем, что Qt.qt_resvar - это номер переменной,
**      связанной с (единственным) базовым отношением при операции
**      обновления (иные варианты здесь не допускаются).
**      Vrscan просто принимает это утверждение на веру (оно проверяется
**      в функции view.
**      Эти странные вещи делаются только при mdVIEW.
**
**      Кроме того, если в агрегатной функции встретилась переменная vn,
**      то в квалификацию этой функции добавляется квалификация из view
**      (Раньше этого не было, из за чего агрегаты на view давали самые
**      фантастические результаты)
**
**
**      Параметры:
**              proot -- Указатель на указатель на корень дерева
**              vn -- номер view переменной, которую нужно найти и
**                      заменить.
**              transtree -- указатель на левую часть описания view
**              vmode -- mdVIEW если вызвано из view, mdAPP
**                      если вызвано из integrity, иначе что либо еще.
**                      Влияет на обработку tid, агрегатов
**                      и на диагностику ошибок, если vn не найдена
**                      в transtree.
**
**	Returns:
**              число произведенных подстановок
**		(non-local on error).
**
**	Side Effects:
**              Указатель в *proot изменяется (если нужно) на новый.
**
**	Trace Flags:
**		32
*/

subsvars(proot, vn, transtree, vmode,transqual)
QTREE	**proot;
int	vn;
QTREE   *transtree;
int	vmode;
QTREE   *transqual;
{
	register QTREE	*t;
	register QTREE	*v;
	register int	i;
	extern QTREE	*vfind();
	extern QTREE	*makezero();
	extern QTREE	*treedup();
	int needqual=0, ko=0;
	t = *proot;
	v = transtree;

#	ifdef xQTR3
	if (tTf(32, 0))
		printf("subsvars: vn %d root %u transtree %u\n", vn, t, v);
#	endif

	if (t == NULL)
		return(0);

	/* check left branch of the tree */
	needqual = subsvars(&t->left, vn, v, vmode,transqual);

	/* check for special 'tid' RESDOM (used by DEL and REPL) */
	if (t->sym.type == RESDOM && t->sym.value.sym_resdom.resno == 0)
	{
		/* test for not Qt.qt_resvar, in which case we ignore leaf */
		if (vn != Qt.qt_resvar)
			return(needqual);

		/* t->right better be VAR node, attno 0 */
		t = t->right;
		if (t->sym.type != VAR || t->sym.value.sym_var.attno != 0 || t->sym.value.sym_var.varno != vn)
			syserr("subsvars: RESDOM 0 not VAR 0 %d, %d, %d",
				vn, t->sym.value.sym_var.attno, t->sym.type);
		
		/* change varno to new Qm.qm_newresvar (set by vrscan) */
#		ifdef xQTR3
		if (tTf(32, 1))
			printf("RESDOM 0: Qm.qm_newresvar %d\n", Qm.qm_newresvar);
#		endif
		t->sym.value.sym_var.varno = Qm.qm_newresvar;
		return(needqual);
	}

	/* scan right branch */
	ko = needqual + subsvars(&t->right, vn, v, vmode,transqual);
	if (vmode == mdVIEW && t->sym.type == AGHEAD && needqual && transqual)
	{
#		ifdef xQTR3
		if (tTf(32, 1))
			printf("ADDED AGG QUAL varno %d\n", vn);
#		endif
		appqual(treedup(transqual), t);
	}
	/* check for interesting node */
	if (t->sym.type != VAR || t->sym.value.sym_var.varno != vn)
		return(ko);

	/* test for special 'tid' attribute case */
	if (t->sym.value.sym_var.attno == 0 && vmode == mdVIEW)
	{
		qmerror(3340, Qt.qt_qmode, vn, 0);	/* views do not have tids */
	}

	/* find var in vtree */
	v = vfind(t->sym.value.sym_var.attno, v);
	if (v == NULL)
	{
		if (vmode == mdVIEW)
			syserr("subsvars: attno %d", t->sym.value.sym_var.attno);
		else if (vmode == mdAPP)
			v = makezero();
	}
	else
		v = treedup(v);

	/* replace VAR node */
	if (v != NULL)
		*proot = v;
	return(ko+1);
}

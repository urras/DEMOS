#include <stdio.h>
/* #define TEST     */
/*
 Программы для работы с сегментами-перекрытиями на СМ-4-МНОС.ИАЭ
 Позволяют загружать автоматически и выполнять программы,
 состоящие из нескольких программных сегментов,  загружаемых
 с перекрытием (overlay)
*/
#define MSEG 20 /* макс число сегментов в библиотеке */
#define MDEEP 3 /* макс глубина вложенности сегментов*/

typedef long daddr;
typedef unsigned lmemo;
typedef char *amemo;

long tell(), lseek();
static eqstrl();
/* Таблицы для начальной настройки */
extern struct stentry {
	int segme; 
	lmemo shifte;
}
O_tentry[];
extern char *O_snames[];
/* Основные таблицы */
struct stabs {
	int nf; /* Дескриптор файла*/
	daddr dbegin; /* Начало на диске */
	amemo aload;  /* Начало загрузки в память */
	lmemo lload, lbss; /* Длина T+D , B */
	int segm;
}
O_tabs[MSEG];
/* Таблица загруженных сегментов */
struct  stload {
	int segl; 
	amemo basel;
}
O_tload[MDEEP + 1];
int nsegm, mnsegm, mnentry;
amemo *O_pretn, O_tabrt[MDEEP+1];

#define EPRETN (O_tabrt+MDEEP)
/* Описание таблиц и переменных
  Таблица tabs содержит описатель всей библиотеки сегментов
*
	nf - дескриптор файла
	dbegin - начало Т на диске
	aload - начальный адрес загрузки
	lload - длина информационной части D+T
	lbss - длина группы bss
	segm - номер сегмента
*
	tload - таблица загруженных сегментов
	 текущий индекс определяется по O_pretn,
	 которая устанавливается в O_load, а восстанавливается
	 автоматически при выходе из подпрограммы
	 последний элемент содержит адрес "свободно"
*
	nsegm - число сегментов в библиотеке
	mnsegm - максимальный номер сегмента
	mnentry - число входов (по таблице входов)
*/

/* Исполнитель : обращение О_load(номер_входа)
   возвращает адрес вызываемой программы
   изменяет адрес возврата на O_retn
   старый адрес запоминается, возврат по *++O_pretn
*/
extern O_retn;
/* Несколько описаний для настройки файла сообщений, совмещенного с
 * библиотекой модулей
*/
static Xfd= -1; static long Xs0;
#define Mess "Messages"

amemo O_load(numer,reta)
int numer; amemo reta;
{
	amemo abegin, aload, aend;
	register amemo c;
	register struct stabs *pload;
	int nd,segml; 
	register int i;
	lmemo shiftl;
	if(numer <= 0 || numer > mnentry) O_error("bad entry nomer\n",0,0);
	shiftl = O_tentry[numer -1].shifte;
	segml = O_tentry[numer-1].segme;
	nd = EPRETN - O_pretn;
	/* Если в памяти остался требуемый сегмент */
	if(O_tload[nd].segl == segml) {abegin = O_tload[nd].basel;goto retn;}
	/* Если сегмент с таким номером был загружен еще раньше */
	for(i=0;i<nd;i++)
	{
		if(O_tload[i].segl == segml)
		{ 
			O_tload[nd].segl = 0; 
			O_tload[nd].basel = O_tload[nd-1].basel;
			abegin = O_tload[i].basel;
			goto retn;
		}
	}
	/* Загружаем раздел */
	pload = O_tabs;
	abegin = O_tload[nd].basel;
	while(pload->nf)
	{
		if(pload->segm == segml && pload->aload == abegin) goto yesl;
		pload++;
	}
	O_error("no segment %s loaded on %o found ", O_snames[segml-1], abegin);
yesl: 
	lseek(i=(pload->nf), pload->dbegin, 0);
	if( read(i, abegin, pload->lload) != pload->lload)
		O_error("read segment error\n",0,0);
	c = abegin + pload->lload;
	aend = c + pload->lbss;
	while(c != aend ) *c++ = 0;
	O_tload[nd].segl = segml;
	O_tload[nd+1].segl = 0;
	O_tload[nd+1].basel = abegin + pload->lload + pload->lbss;
retn: 
	*O_pretn-- = reta; /* Адрес возврата к пользователю */
	reta = (amemo)(&O_retn);
	return(abegin + shiftl);
}

/* Подпрограмма O_start служит для начальной
	настройки системы на библиотеку overlay
	параметр - дескриптор файла, содержащего библиотеку.
	Действия программы:
	- настроить счетчики и указатели
	- просмотр библиотеки и заполнение таблицы сегментов
	- отведение памяти и контроль полноты библиотеки
*/
O_start(fd) int fd;     /*NOXXSTR*/
{ 
#include <ar.h>
#include <a.out.h>
struct bufrl {int magic; struct ar_hdr arch; struct exec aouth;}
	buff;
struct exec *aout0 = &buff;
#define LMAA (sizeof buff)
#define LAA ((sizeof buff)-(sizeof(int)))
	extern end;
	char fuse[MSEG];
	register struct stabs *psegm;
	amemo maxa, maxb;
	daddr aobj;
	register int i,j;
	i=0;
	while(O_tentry[i++].segme); 
	mnentry = i;
	psegm = O_tabs + nsegm;
	O_pretn = EPRETN;
	O_tload[0].basel = (amemo) &end;
rhead:  if(read(fd, &buff, LMAA) != LMAA )
		O_error("bad library\n",0,0);
/* Архив может быть записан в хвост выполняемого файла, пропустим этот файл */
	if (!N_BADMAG(*aout0))
	    {aobj = aout0->a_text+aout0->a_data+aout0->a_syms+(sizeof *aout0);
	     lseek(fd,(long)(aobj-LMAA),1); goto rhead;
	    }


	i = j = 0;
	while(O_snames[i])
	{
		mnsegm = i+1; 
		fuse[i++] = 0;   
	}
	/* Основной цикл */
	do
	    { 
		aobj = tell(fd);
		if(eqstrl(&(buff.arch.ar_name[0]),Mess)) {Xfd=fd; Xs0=aobj-(sizeof (struct exec));
			goto next;}
		for( i = 0; i < mnsegm; i++)
			if(eqstrl(&(buff.arch.ar_name[0]), O_snames[i])) goto finds;
		goto next;
finds:
		if(buff.aouth.a_magic != A_MAGIC4)
			O_error("bad file magic number: %s\n",&buff.arch.ar_name,0);
		fuse[i] = 1;
		i++; /* номер сегмента  */
		psegm->nf = fd;
		psegm->dbegin = aobj;
		maxb = psegm->aload = buff.aouth.a_unused;
		maxb += (psegm->lload = buff.aouth.a_text+buff.aouth.a_data);
		maxb += (psegm->lbss = buff.aouth.a_bss);
		psegm->segm = i;
		maxa = (maxb > maxa? maxb: maxa);
		psegm++;
		nsegm ++;
		if (nsegm > MSEG) O_error("segm tab overflow\n",0,0);
next:           lseek(fd, ((buff.arch.ar_size + 1) & ~01)-(sizeof(struct exec)), 1);
	 }
	while (read(fd, &(buff.arch),LAA) == LAA);
	for(i=0; i<mnsegm; i++)
	 if( !fuse[i])O_error( "not found segment: %s\n", O_snames[i], 0);
	if( brk(maxa + 2)) O_error("no memory for overlay\n",0,0);
	/* Отладочная печать  */
#ifdef TEST
	printf("Модуль O_start\n");
	printf(" fd = %d. Всего сегментов %d макс номер сегмента %d входов %d\n",
	fd, nsegm, mnsegm, mnentry-1);
	printf("\n Таблица сегментов \n ");
	printf(
	" fd  имя сегм.  ном   д.адрес   загр.с  T + D  BSS \n");
	psegm = O_tabs;
	while(psegm->nf){printf("%3.3d %10.10s %3.3d %10.10lo %6.6o %6.6o %6.6o \n",
	psegm->nf, O_snames[psegm->segm-1], psegm->segm, psegm->dbegin,
	psegm->aload, psegm->lload, psegm->lbss); psegm++;}
	printf(" --- --- --- --- --- --- --- ---\n");
#endif
	if(Xfd<0) {O_error("Message ar file %s absent\n",Mess);}
	return(1);
}

#ifndef O_error
O_error(f,p1,p2)
register char *f; 
int p1,p2;
{ 
	register char *f1 = f;
	write(2, "Overlay:", 8);
	while(*f1++); 
	write(2,f, f1 -f);
	exit(4);
}
#endif

static eqstrl(s1,s2)
register char *s1, *s2;
{
	while( *s1  && *s1 != '.') if(*s1++ != *s2++) return(0);
	return(1);
}


char *Xsline( sh )
unsigned sh;
{
	static char Xslin[128];
	if( Xfd < 0  )
		{ return( "***" ); }
	lseek( Xfd, (long)sh+Xs0, 0 );
	Xslin[read( Xfd, Xslin, 127 )] = 0;
	return( Xslin );
}

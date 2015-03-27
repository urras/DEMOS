/*
 * ОБРАБОТКА ПАРАМЕТРОВ ВЫЗОВА ТИПА:
 *  BOOT: rk(0,0)demos r=rk2 s=rk4 su
 */

#include <sys/types.h>
#include <sys/reboot.h>

#define P(s)    /* printf(s) */

/*
 * Таблица параметров и типов
 */
#define PDEV  1
#define PUNS  2
#define PLONG 3
#define PFLAG 4
#define PBOOT 5

#define BOOT(set, unset) (long)((set) + (unset)*0400l)

extern int bootopts;
int        waspar;      /* Не 0, если были параметры */

#define ROOTK   keytab[0]
#define SWAPK   keytab[1]
#define SWAPLOK keytab[2]
#define SWAPLEK keytab[3]
#define SUK     keytab[4]
#define PIPEK   keytab[5]

struct keyts {
	char   *keyw;   /* имя параметра */
	char    typew;  /* тип параметра */
	char    flagw;  /* флажок установлен */
	long    valuew; /* значение */
} keytab[] = {
	{"r",      PDEV,   0, 0l},
	{"s",      PDEV,   0, 0l},
	{"slo",    PLONG,  0, 0l},
	{"slen",   PUNS,   0, 0l},
	{" su",    PFLAG,  0, 0l},
	{"p",      PDEV,   0, 0l},
	{"fast",   PBOOT,  0, BOOT(RB_NOFSCK, RB_SINGLE)   },
	{"auto",   PBOOT,  0, BOOT(0, RB_SINGLE|RB_ASKNAME)},
	{"nosu",   PBOOT,  0, BOOT(0, RB_SINGLE)           },
	{"su",     PBOOT,  0, BOOT(RB_SINGLE|RB_ASKNAME, 0)},
	{"fault",  PBOOT,  0, BOOT(RB_POWRFAIL, 0)         },
	{(char*)0, 0,      0, 0l}
};

/*
 * Общие описания
 */
extern long  atodl();
extern dev_t getdev();

#define MAXU  0177777l
#define NODEV (dev_t)(-1)

/* Разбор строки параметров */
getpar(line)
register char *line;
{
	register char *key, *val;
	char  razd;
	long  vl;
	int   rkod = 1;

/*      initpar();      */    /* Now before ask loop in boot.c --P3 */

	/*
	 * Выделяем имя файла
	 */
	while(*line != ' ' && *line)
		line++;
	if(*line == 0)
		return(1);     /* Нет параметров */
	*line++ = 0;

	while(*line == ' ')
		line++;

	/*
	 * Цикл по параметрам
	 */
	while(*line) {
		key = line;
		while(*line && *line != ' ' && *line != '=')
			line++;
		razd = *line;
		if(*line)
			*line++ = 0;
		if(razd != '=')
			val = 0;
		else {
			val = line;
			while(*line != ' ' && *line)
				line++;
			if(*line)
				*line++ = 0;
		}
		if(!setpar(key, val))
			rkod = 0;
		while(*line == ' ')
			line++;
	}
	return(rkod);
}

/*
 * Макро для работы с таблицей параметров
 */

#define REGPT   register struct keyts *
#define FORT(k) for(k = keytab; k->keyw ; k++)

/*
 * Инициализация таблицы
 */
initpar()
{
	REGPT k;

	FORT(k) {
		k->flagw = 0;
		if(k->typew != PBOOT)
			k->valuew = 0;
	}
	waspar = 0;
}

/*
 * Засылка одного параметра в таблицу
 */
setpar(key, val)
register char *key, *val;
{
	REGPT k;

	char *val0 = val;
	long  vl;

	FORT(k) {
		if(!strcmp(k->keyw, key))
			goto found;
	}
	printf("%s: illegal key\n", key);
	return(0);
found:
	switch(k->typew) {
	    case PDEV:
		if(!val)
			goto noverr;
		if((k->valuew = getdev(val)) == NODEV)
			goto errv;
		break;

	    case PFLAG:
		if(val)
			goto errv;
		k->valuew = 1;
		break;

	    case PUNS:
		if(!val)
			goto noverr;
		if((vl = atodl(val)) < 0 || vl > MAXU)
			goto errv;
		k->valuew = (unsigned)vl;
		break;

	    case PLONG:
		if(!val)
			goto noverr;
		if((vl = atodl(val)) < 0)
			goto errv;
		k->valuew = vl;
		break;

	    case PBOOT:
		if(val)
			goto errv;
		bootopts |=  (int)(k->valuew%0400l);
		bootopts &= ~(int)(k->valuew/0400l);
		break;

	    default:
		printf("Error in parameter\n");
		return(0);
	}
	k->flagw = 1;
	waspar++ ;
	return(1);
noverr:
	printf("No value!\n");
	return(0);
errv:
	printf("Illegal  %s\n",  val);
	return(0);
}

long atodl(s)
register char *s;
{
	int      ib = 10; /* Основание счисления */
	register i;
	long     vl = 0;

	if(*s == '0') {
		ib = 8;
		if(*++s == 'x' || *s == 'X') {
			ib = 16;
			s++;
		}
	}
	while(*s) {
		if(*s>='0' && *s <='9') i = *s - '0';
		else if(ib == 16 && *s>='a' && *s<='f') i = *s -'a' + 10;
		else if(ib == 16 && *s>='A' && *s<='F') i = *s -'A' + 10;
		else return(-1l);
		vl = vl*ib + i;
		s++;
	}
	return(vl);
}

int atodn(s)
register char *s;
{
	register i;
	int      vl = 0;

	while(*s && *s >= '0' && *s <='9') {
		i  = *s - '0';
		vl = vl*10 + i;
		s++;
	}
	if ( *s >= 'a' && *s <= 'h' )
		vl = vl*8 + *s++ - 'a';
	else if( *s == '.' && *(s+1) == 'd' ){
		vl += 128;
		s += 2;
	}
	return(*s==0 ? vl : -1);
}

/*
 * Эта таблица согласовывается с mkconfig.c
 */
struct tdevs {
	char *devt;     /* имя типа */
	int   ndevt;    /* мажор типа */
} tdev[] = {
	{"rk",      0},
	{"rp",      1},
	{"my",      2},
	{"tm",      3},
	{"hk",      4},
	{"hs",      5},
	{"xp",      6},
	{"hp",      6},
	{"rm",      6},
	{"ht",      7},
	{"rl",      8},
	{"ts",      9},
#ifdef DVK4
	{"dw",     10},
#else
	{"rw",     10},
#endif
	{"rx",     11},
	{"fd",     12},
	{"vd",     13},
	{"ra",     14},
	{(char*)0,  0}
};

/*
 * Засылка кода устройства
 */
dev_t getdev(s)
register char *s;
{
	register struct tdevs *k;
	char  *dev = s;
	int    mi, mg;

	/* Выделение имени и номера */
	mi = 0;
	while((*s>='a' && *s<='z') ||
	      (*s>='A' && *s<='Z'))
		s++;
	if(*s>='0' && *s<='9') {
		mi = atodn(s);
		if(mi < 0)
			return(NODEV);
		*s = 0;
	} else if(*s)
			return(NODEV);
	for(k = tdev; k->devt ; k++) {
		if(!strcmp(dev, k->devt))
			break;
	}
	if( !k->devt )
		return(NODEV);
	mg = k->ndevt;
	return  makedev(mg, mi);
}

/*
 * НАСТРОЙКА ЗАГРУЖЕННОЙ КОМПОНЕНТЫ (ядра ОС)
 * Заголовок должен быть считан и находиться в "exec"
 * Заголовок оверлея должен находиться в ovlhdr
 * Признак overlayd используется для определения "overlay"
 * В таблицу переменных записываем не истинный адрес, а адрес
 * переменной в сегмента данных.
 */
#include <a.out.h>
#define SPACE 100               /* number of symbols read at a time */

extern bool_t          overlaid;
extern bool_t          xoverlaid;
extern struct  exec    exec;
extern struct  xovlhdr ovlhdr;

/* Число регистров, которые занимает сегмент */
#define ator(a) (((unsigned)a+017777)/020000)

nlisti(f, list)
int f;
struct nlist *list;
{
	register struct nlist *p, *q;
	int n, m, i, nnames = 0;
	long sa;
	extern struct exec exec;
	unsigned data_base;
	int ovl_par, txt_par, tpar;
	static struct nlist space[SPACE];

	for(p = list; p->n_name[0]; p++) {
		p->n_type = 0;
		p->n_value = 0;
	}
	if(f < 0)
		return(-1);
	sa = (long)exec.a_text + (long)exec.a_data;
	txt_par = ator(exec.a_text);
	tpar = ovl_par = 0;
	if ( overlaid )
		for(n=0; n<(xoverlaid?NXOVL:NOVL); n++)
			{
			sa += (long)ovlhdr.xov_siz[n];
			if ((tpar=ator(ovlhdr.xov_siz[n])) > ovl_par)
				ovl_par = tpar;
			}
	if ( exec.a_magic == A_MAGIC1 ||
	     exec.a_magic == A_MAGIC3 ||
	     exec.a_magic == A_MAGIC6 ) data_base = 0;
	else data_base = (txt_par+ovl_par) * (unsigned)020000;
#ifdef TEST
	printf("data_base= %o \n",data_base);
#endif
	if(exec.a_flag != 1) sa *= 2;
	sa += N_TXTOFF(exec);
	lseek(f, sa, 0);
	n = exec.a_syms;
	while(n){
		m = sizeof space;
		if(n < sizeof space)
			m = n;
		read(f, (char *)space, m);
		n -= m;
		for(q = space; (m -= sizeof(struct nlist)) >= 0; q++) {
			for(p = list; p->n_name[0]; p++) {
				for(i=0;i<8;i++)
					if(p->n_name[i] != q->n_name[i]) goto cont;
				p->n_value = q->n_value;
				p->n_type = q->n_type;
				if(p->n_type == (N_EXT|N_DATA) ||
				p->n_type == (N_EXT|N_BSS))
					{
					p->n_value -= data_base;
					nnames++;
					}
				break;
		cont:           ;
			}
		}
	}
	return(nnames);
}

/*
 * Таблица имен для настройки ядра
 */
#define SROOTD  tabsym[0]
#define SSWAPD  tabsym[1]
#define SPIPED  tabsym[2]
#define SSWAPLO tabsym[3]
#define SSWAPLE tabsym[4]
#define SSU     tabsym[5]
#define SMAX           6

struct nlist tabsym[] = {
	{"_rootdev", 0, 0},
	{"_swapdev", 0, 0},
	{"_pipedev", 0, 0},
	{"_swplo",   0, 0},
	{"_nswap",   0, 0},
	{"_suboot",  0, 0},
	{"\0",       0, 0}
};

/*
 * Разные макро, в том числе для отладки
 */
#ifdef TEST
#define setpwl(to,wl) (printf("%O->%s %o\n",wl,to.n_name,to.n_value), \
	mtpi((unsigned)(wl>>16),to.n_value),mtpi((unsigned)wl,to.n_value+2))
#define setpw(to,w) (printf("%o->%s %o\n",(unsigned)w, to.n_name,to.n_value), \
	mtpi((unsigned)w, to.n_value))
#define getpw(from) mfpi(from.n_value)
#else
#define setpwl(to,wl) (mtpi((unsigned)(wl>>16),to.n_value),\
		mtpi((unsigned)wl,to.n_value+2))
#define setpw(to,w) (mtpi((unsigned)w, to.n_value))
#define getpw(from) mfpi(from.n_value)
#endif

/* Настройка ядра
 *
 * В момент обращения приписка должна быть установлена так,
 * чтобы 0 соответствовал началу сегмента данных + bss
 */
nastroi(io) int io;
{
	int i, nossu=0;
	unsigned rootw;

	if(!waspar) return(1);
	P("start of nlist\n");
	i = nlisti(io,tabsym);
	P("end of nlist\n");
#if defined(DEBUG) || defined(TEST)
	printf("boot opts=%o\n",bootopts);
	{
		register struct nlist *k;
		for(k=tabsym;k->n_name[0];k++)
		if(k->n_value) printf("%s:\t %o \t %o w=%o\n", k->n_name,
			k->n_type, k->n_value, getpw((*k)));
	}
#endif
	if(i != SMAX) {
		if(i==SMAX-1 && SSU.n_type == 0) nossu=1;
		else {
			printf("No apropriate names found\n");
			return(0);
		}
	}
	if(ROOTK.flagw)
	{
		rootw = getpw(SROOTD);
		if(getpw(SSWAPD) == rootw) setpw(SSWAPD,ROOTK.valuew);
		if(getpw(SPIPED) == rootw) setpw(SPIPED,ROOTK.valuew);
		setpw(SROOTD,ROOTK.valuew);
	}
	if(SWAPK.flagw)   setpw(SSWAPD,SWAPK.valuew);
	if(PIPEK.flagw)   setpw(SPIPED,PIPEK.valuew);
	if(SWAPLOK.flagw) setpwl(SSWAPLO,SWAPLOK.valuew);
	if(SWAPLEK.flagw) setpw(SSWAPLE,SWAPLEK.valuew);
	if(SUK.flagw) {
		if(nossu) {
			printf("No '_suser' found");
			return(0);
		}
		setpw(SSU,SUK.valuew);
	}
#if defined(DEBUG) || defined(TEST)
	printf("boot opts=%o\n",bootopts);
	{
		register struct nlist *k;
		for(k=tabsym;k->n_name[0];k++)
	     if(k->n_value) printf("%s:\t %o \t %o w=%o\n", k->n_name,
			k->n_type, k->n_value, getpw((*k)));
	}
#endif
	P("end of nastroi\n");
	return(1);
}

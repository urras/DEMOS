/* Режимы тестирования: TEST_DATE TEST_FORM TEST_OUT TEST_IN */
/*
 * datetime.h
 * Заголовок для функция преобразования даты и времени
 * по шаблону
 */

# ifndef NULL
# define NULL (char *)0
#endif

/*
 * Структуры для расшифровки формата
 */
#ifndef TEST_FORM
# define D_COMPL 037 /* Признак скомпилированного шаблона */
#else
# define D_COMPL '~'
#endif
/* Описатель шаблона */
struct tabform {
	char *da_name;  /* Имя поля */
	int  *da_addr;  /* Адрес информации */
	char *da_form;  /* Формат преобразования */
	char da_off;    /* Это прибавляется к полю */
	char da_flen;   /* Длина поля */
};
/* Описатель преобразования */
struct datebase {
	long (*tolong)();
	int (*fromlong)();
	char *convbuf;
	struct tabform *tb;
	char *dflt;
};
extern struct datebase b_date,b_time;
extern char *to_compl(), *do_compl();
extern char *to_char();
extern long from_char();
extern int len_char();

/*
 * datetime.h
 * Описания основных программ преобразования
 * даты и времени
 */
# ifndef MAX_LDATE
# define        MAX_LDATE       48      /* Макс. длина строки */
typedef long t_date,t_time; /* Дней с 000 года, 1 января без учета эр */
/*
 * Описания для datetime.h в ../h
 *
 * 1. Функции для преобразования из даты/времени в строку и обратно
 */
extern char *date_char(), *time_char();
extern t_date char_date();
extern t_time char_time();
/*
 * 2. Функции выдачи длины поля даты/времени
 */
extern int date_len(), time_len();
/*
 * Коды ошибок (только внутри RUBIN)
 */
# define        BADDATE        2144    /* Плохой формат времени */
# define        BADTIME        2140    /* Плохой формат даты */

/*
 * Функции запроса системного времени и даты
 */
extern newdati();
extern long sys_date,sys_time;
# define get_date() (sys_date<0? (newdati(), sys_date):sys_date)
# define get_time() (sys_time<0? (newdati(), sys_time):sys_time)
# define new_datime()  {sys_date = sys_time = -1;}

/*
 * Структуры для расшифровки даты и времени
 * Соответствующие функции описаны чуть ниже
 */
struct cd_date {
	int cd_year;    /* Год   0000  */
	int cd_month;   /* месяц (1-12)*/
	int cd_day;     /* день  (1-31)*/
	int cd_2year;   /* Год века    */
	int cd_week;    /* День недели */
};
struct cd_time {
	int cd_hh;      /* Час         */
	int cd_mm;      /* Минуты      */
	int cd_ss;      /* Секунды     */
};

extern date_cd(/* t_date dd,struct cd_date *cd */ );
extern time_cd(/* t_time dd,struct cd_time *cd */ );
extern t_date cd_date( /* struct cd_date *cd */ );
extern t_time cd_time( /* struct cd_time *cd */ );

/*
 * Переопределения (для генерации под equel)
 */
# ifdef EQUEL
# define length IIlength
# define smove  IIsmove
# endif
#endif

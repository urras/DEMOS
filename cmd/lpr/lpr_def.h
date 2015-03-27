#include <sys/types.h>
#include <dir.h>

/* Заголовок командного файла lpd */
struct j_header {
        char jh_flags;          /* Режимы печати задания */
        char jh_copies;         /* Число копий */
        char jh_indent;         /* Отступ от левого края */
        char jh_person[DIRSIZ]; /* Имя пользователя */
        char jh_name[DIRSIZ];   /* Имя задания */
};

/* Значения jh_flags */
#define F_FTYPE 07      /* Маска на тип фильтра */
#define F_GRAPH 01      /* Фильтр для графики (plot (3)) */
#define F_FTN   02      /* Фильтр для вывода фортрана */
#define F_MAIL  010     /* Послать сообщение после завершения выдачи */
#define F_NOHDR 020     /* Не выдавать титульный лист */

/* Описание файла данных */
struct j_entry {
        char je_name[DIRSIZ];   /* Базовое имя оригинала (для lpq) */
        char je_file[DIRSIZ];   /* Имя файла данных */
};

/* Описание очереди заданий */
struct j_queue {
        char jq_name[DIRSIZ];
        time_t jq_ctime;
        struct j_queue *jq_next;
};

/* Состояние демона */
struct d_stat {
        int d_pid;
        char d_status[64];
};

/* Коды возврата подпрограмм и команд */
#define TRUE            (1)
#define FALSE           (0)
#define OK              (0)
#define ERR             (1)

/* Режимы доступа к файлу (access) */
#define A_E             0       /* существует */
#define A_R             04      /* можно читать */
#define A_W             02      /* можно писать */
#define A_X             01      /* можно выполнять */

/* Режимы открытия файла (open) */
#define O_R             0       /* только чтение */
#define O_W             1       /* только запись */
#define O_RW            2       /* чтение/запись */

/* Основные параметры */
#define MAX_USERS       50      /* Число запросов по имени владельца */
#define MAX_JOBS        50      /* Число запросов по номеру задания */
#define MAX_COPIES      16      /* Число копий задания */
#define MAX_FILES       20      /* Число открытых файлов процесса */
#define BUF_SIZ         1024    /* Размер динамических буферов */
#define CRT_MODE        0660    /* Режим создания файлов */
#define DEF_PRINTER     "lp"    /* Имя принтера по умолчанию */
#define Null            (char *) 0

/* Имена семафорных файлов */
#define STOP    ".STOP."        /* Запрет печати */
#define DISABLE ".DISABLE."     /* Запрет буферизации */
#define LOCK    ".LOCK."        /* Запрет запуска демона */

/* Описание строковых переменных */
struct c_strs {
        char *cs_name;           /* Имя */
        char *cs_value;          /* Значение */
        char *cs_default;        /* Умолчание */
};

/* Описание числовых переменных */
struct c_nums {
        char *cn_name;           /* Имя */
        int   cn_value;          /* Значение */
        int   cn_default;        /* Умолчание */
};

/* Описание флагов */
struct c_flgs {
        char *cf_name;           /* Имя */
        char cf_value;           /* Значение */
};

/* Имена характеристик в /etc/printcap */
#define D_DN    "dn"    /* Имя демона */
#define D_SD    "sd"    /* Имя буферного справочника */
#define D_LF    "lf"    /* Имя файла вывода ошибок демона */
#define D_AF    "af"    /* Имя файла статистики */
#define D_LP    "lp"    /* Имя устройства */
#define D_FF    "ff"    /* Команда прогона листа */
#define D_OF    "of"    /* Имя стандартного фильтра */
#define D_GF    "gf"    /* Имя графического фильтра */
#define D_RF    "rf"    /* Имя фильтра для фортрана */

#define D_PL    "pl"    /* Число строк на лист */
#define D_PW    "pw"    /* Число символов на строку */
#define D_PX    "px"    /* Число точек по горизонтали */
#define D_PY    "py"    /* Число точек по вертикали */

#define D_SH    "sh"    /* Запрет печати титульного листа */
#define D_SF    "sf"    /* Запрет прогона листа между заданиями */
#define D_SC    "sc"    /* Запрет вывода копий */
#define D_FO    "fo"    /* Прогон листа при первом открытии устройства */

/* Значения по умолчанию */
#define DEF_DN  "/usr/lib/lpd"
#define DEF_SD  "/usr/spool/lpd"
#define DEF_LF  "/dev/console"
#define DEF_AF  Null
#define DEF_LP  "/dev/lp"
#define DEF_FF  "\f"
#define DEF_OF  "/usr/lib/lpf"
#define DEF_GF  Null
#define DEF_RF  Null

#define DEF_PL  66
#define DEF_PW  132
#define DEF_PX  -1
#define DEF_PY  -1

#include <sys/types.h>
#include <dir.h>

/* ��������� ���������� ����� lpd */
struct j_header {
        char jh_flags;          /* ������ ������ ������� */
        char jh_copies;         /* ����� ����� */
        char jh_indent;         /* ������ �� ������ ���� */
        char jh_person[DIRSIZ]; /* ��� ������������ */
        char jh_name[DIRSIZ];   /* ��� ������� */
};

/* �������� jh_flags */
#define F_FTYPE 07      /* ����� �� ��� ������� */
#define F_GRAPH 01      /* ������ ��� ������� (plot (3)) */
#define F_FTN   02      /* ������ ��� ������ �������� */
#define F_MAIL  010     /* ������� ��������� ����� ���������� ������ */
#define F_NOHDR 020     /* �� �������� ��������� ���� */

/* �������� ����� ������ */
struct j_entry {
        char je_name[DIRSIZ];   /* ������� ��� ��������� (��� lpq) */
        char je_file[DIRSIZ];   /* ��� ����� ������ */
};

/* �������� ������� ������� */
struct j_queue {
        char jq_name[DIRSIZ];
        time_t jq_ctime;
        struct j_queue *jq_next;
};

/* ��������� ������ */
struct d_stat {
        int d_pid;
        char d_status[64];
};

/* ���� �������� ����������� � ������ */
#define TRUE            (1)
#define FALSE           (0)
#define OK              (0)
#define ERR             (1)

/* ������ ������� � ����� (access) */
#define A_E             0       /* ���������� */
#define A_R             04      /* ����� ������ */
#define A_W             02      /* ����� ������ */
#define A_X             01      /* ����� ��������� */

/* ������ �������� ����� (open) */
#define O_R             0       /* ������ ������ */
#define O_W             1       /* ������ ������ */
#define O_RW            2       /* ������/������ */

/* �������� ��������� */
#define MAX_USERS       50      /* ����� �������� �� ����� ��������� */
#define MAX_JOBS        50      /* ����� �������� �� ������ ������� */
#define MAX_COPIES      16      /* ����� ����� ������� */
#define MAX_FILES       20      /* ����� �������� ������ �������� */
#define BUF_SIZ         1024    /* ������ ������������ ������� */
#define CRT_MODE        0660    /* ����� �������� ������ */
#define DEF_PRINTER     "lp"    /* ��� �������� �� ��������� */
#define Null            (char *) 0

/* ����� ���������� ������ */
#define STOP    ".STOP."        /* ������ ������ */
#define DISABLE ".DISABLE."     /* ������ ����������� */
#define LOCK    ".LOCK."        /* ������ ������� ������ */

/* �������� ��������� ���������� */
struct c_strs {
        char *cs_name;           /* ��� */
        char *cs_value;          /* �������� */
        char *cs_default;        /* ��������� */
};

/* �������� �������� ���������� */
struct c_nums {
        char *cn_name;           /* ��� */
        int   cn_value;          /* �������� */
        int   cn_default;        /* ��������� */
};

/* �������� ������ */
struct c_flgs {
        char *cf_name;           /* ��� */
        char cf_value;           /* �������� */
};

/* ����� ������������� � /etc/printcap */
#define D_DN    "dn"    /* ��� ������ */
#define D_SD    "sd"    /* ��� ��������� ����������� */
#define D_LF    "lf"    /* ��� ����� ������ ������ ������ */
#define D_AF    "af"    /* ��� ����� ���������� */
#define D_LP    "lp"    /* ��� ���������� */
#define D_FF    "ff"    /* ������� ������� ����� */
#define D_OF    "of"    /* ��� ������������ ������� */
#define D_GF    "gf"    /* ��� ������������ ������� */
#define D_RF    "rf"    /* ��� ������� ��� �������� */

#define D_PL    "pl"    /* ����� ����� �� ���� */
#define D_PW    "pw"    /* ����� �������� �� ������ */
#define D_PX    "px"    /* ����� ����� �� ����������� */
#define D_PY    "py"    /* ����� ����� �� ��������� */

#define D_SH    "sh"    /* ������ ������ ���������� ����� */
#define D_SF    "sf"    /* ������ ������� ����� ����� ��������� */
#define D_SC    "sc"    /* ������ ������ ����� */
#define D_FO    "fo"    /* ������ ����� ��� ������ �������� ���������� */

/* �������� �� ��������� */
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

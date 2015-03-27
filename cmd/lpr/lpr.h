/* Номера сообщений */
#define M_AR            MSGS[0]
#define M_CREAT         MSGS[1]
#define M_DIR           MSGS[2]
#define M_EMPTY         MSGS[3]
#define M_EXEC          MSGS[4]
#define M_EXECU         MSGS[5]
#define M_FTN           MSGS[6]
#define M_GRAPH         MSGS[7]
#define M_LINK          MSGS[8]
#define M_OPEN          MSGS[9]
#define M_READ          MSGS[10]
#define M_SC            MSGS[11]
#define M_SPEC          MSGS[12]
#define M_STAT          MSGS[13]
#define M_UNKNOWN       MSGS[14]
#define M_UNLINK        MSGS[15]
#define M_USAGE         MSGS[16]
#define M_USER          MSGS[17]
#define M_WRITE         MSGS[18]
#define M_DISABLE       MSGS[19]

/* Тексты сообщений */
char   *MSGS[] = {
    "archive file",
    "cannot create",
    "directory",
    "empty file",
    "cannot exec",
    "executive program",
    "no fortran filter for printer",
    "no graph filter for printer",
    "cannot rename",
    "cannot open",
    "cannot read",
    "suppress multiple copies",
    "special file",
    "cannot stat",
    "unknown printer",
    "cannot unlink",
    "usage: lpr [-P<printer> [-J<name>] [-<copies>] [-i<indent>]\n\t\t[-m] [-h] [-c] [-g] [-f] [file ...]",
    "who are You?",
    "cannot write",
    "spooling queue disable"
};

/* Характеристики устройства из /etc/printcap */
struct c_strs   STRS[] = {
    { D_DN, Null, DEF_DN },
    { D_SD, Null, DEF_SD },
    { D_GF, Null, DEF_GF },
    { D_RF, Null, DEF_RF }
};

struct c_strs  *END_STRS = &STRS[(sizeof (STRS) / sizeof (struct c_strs))];

#define C_DEAMON        STRS[0].cs_value
#define C_SPOOL         STRS[1].cs_value
#define C_GRAPH         STRS[2].cs_value
#define C_FTN           STRS[3].cs_value

struct c_flgs   FLGS[] = {
    { D_SC, FALSE }
};

struct c_flgs  *END_FLGS = &FLGS[(sizeof (FLGS) / sizeof (struct c_flgs))];

#define C_SCOPIES       FLGS[0].cf_value

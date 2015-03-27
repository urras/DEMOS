/* Номера сообщений */
#define M_CHDIR         MSGS[0]
#define M_COM           MSGS[1]
#define M_OPEN          MSGS[2]
#define M_READ          MSGS[3]
#define M_SUSER         MSGS[4]
#define M_UNKNOWN       MSGS[5]
#define M_RSTOP         MSGS[6]
#define M_CREAT         MSGS[7]
#define M_ENABLE        MSGS[8]
#define M_RENABLE       MSGS[9]
#define M_RDISABLE      MSGS[10]
#define M_ABORT         MSGS[11]
#define M_BADLOCK       MSGS[12]
#define M_EXEC          MSGS[13]
#define M_FORK          MSGS[14]
#define M_NODEAMON      MSGS[15]
#define M_UNLINK        MSGS[16]

/* Тексты сообщений */
char *MSGS[] = {
        "cannot chdir to",
        "unknown command",
        "cannot open",
        "cannot read",
        "invalid command",
        "unknown printer",
        "deamon allready stopped",
        "cannot create",
        "cannot enabled spooling queue",
        "spooling queue allready enable",
        "spooling queue allready disable",
        "cannot abort deamon",
        "bad lock file, deamon was die",
        "cannot exec",
        "cannot fork",
        "deamon not exist",
        "cannot unlink"
};

/* Характеристики устройства из /etc/printcap */
struct c_strs STRS[] = {
        { D_DN, Null, DEF_DN },
        { D_SD, Null, DEF_SD },
        { D_LF, Null, DEF_LF },
        { D_AF, Null, DEF_AF },
        { D_LP, Null, DEF_LP },
        { D_FF, Null, DEF_FF },
        { D_OF, Null, DEF_OF },
        { D_GF, Null, DEF_GF },
        { D_RF, Null, DEF_RF }
};

#define C_DEAMON        STRS[0].cs_value
#define C_SPOOL         STRS[1].cs_value

struct c_strs *ENDSTRS = &STRS[(sizeof (STRS) / (sizeof (struct c_strs)))];

char *str_name[] = {
        "deamon",
        "spool directory",
        "error logging file",
        "accounting file",
        "device",
        "form feed command",
        "standart filter",
        "graphical filter",
        "fortran filter"
};

struct c_nums NUMS[] = {
        { D_PL, 0, DEF_PL },
        { D_PW, 0, DEF_PW },
        { D_PX, 0, DEF_PX },
        { D_PY, 0, DEF_PY }
};

struct c_nums *ENDNUMS = &NUMS[(sizeof (NUMS) / (sizeof (struct c_nums)))];

char *num_name[] = {
        "page length (in lines)",
        "page width (in characters)",
        "page width (in pixels)",
        "page length (in pixels)"
};

struct c_flgs FLGS[] = {
        { D_SH, 0 },
        { D_SF, 0 },
        { D_SC, 0 },
        { D_FO, 0 }
};

struct c_flgs *ENDFLGS = &FLGS[(sizeof (FLGS) / (sizeof (struct c_flgs)))];

char *flg_name[] = {
        "suppress printing of burst page",
        "suppress form feeds",
        "suppress multiple copies",
        "print a form feed no first open"
};

/* Команды и их описание */
int do_abort (), do_restart (), do_enable (), do_disable (),
do_stop (), do_start (), do_caps (), do_inform (), do_help ();

struct _coms {
        char *cm_name;
        char cm_suser;
        int  (*cm_func) ();
};

struct _coms COMS[] = {
        { "help", FALSE, do_help },
        { "abort", TRUE, do_abort },
        { "restart", FALSE, do_restart },
        { "enable", TRUE, do_enable },
        { "disable", TRUE, do_disable },
        { "stop", TRUE, do_stop },
        { "start", TRUE, do_start },
        { "status", FALSE, do_inform },
        { "caps", FALSE, do_caps }
};

struct _coms *ENDCOMS = &COMS[(sizeof (COMS) / (sizeof (struct _coms)))];

char *com_help[] = {
        "print this list",
        "abort current deamon",
        "restart died deamon",
        "enabled spooling queue",
        "disabled spooling queue",
        "stop deamon, disable printing",
        "start deamon, enable printing",
        "print status of spooling queue and deamon",
        "print capabilites of printer"
};

/* Номера сообщений */
#define M_CHDIR         MSGS[0]
#define M_CREAT         MSGS[1]
#define M_EXEC          MSGS[2]
#define M_FORK          MSGS[3]
#define M_OPEN          MSGS[4]
#define M_READ          MSGS[5]
#define M_STATUS        MSGS[6]
#define M_UNKNOWN       MSGS[7]

/* Тексты сообщений */
char *MSGS[] = {
        "cannot chdir to",
        "cannot create",
        "cannot exec",
        "cannot fork",
        "cannot open",
        "cannot read",
        "cannot write deamon status",
        "unknown printer"
};

/* Характеристики устройства из /etc/printcap */
struct c_strs STRS[] = {
        { D_SD, Null, DEF_SD },
        { D_LF, Null, DEF_LF },
        { D_AF, Null, DEF_AF },
        { D_LP, Null, DEF_LP },
        { D_FF, Null, DEF_FF },
        { D_OF, Null, DEF_OF },
        { D_GF, Null, DEF_GF },
        { D_RF, Null, DEF_RF }
};

#define C_SPOOL         STRS[0].cs_value
#define C_LOGERRS       STRS[1].cs_value
#define C_ACCTFILE      STRS[2].cs_value
#define C_DEVICE        STRS[3].cs_value
#define C_FORMFEED      STRS[4].cs_value
#define C_FILTER        STRS[5].cs_value
#define C_GRAPH         STRS[6].cs_value
#define C_FTN           STRS[7].cs_value

struct c_strs *END_STRS = &STRS[(sizeof (STRS)/sizeof (struct c_strs))];

struct c_nums NUMS[] = {
        { D_PL, 0, DEF_PL },
        { D_PW, 0, DEF_PW },
        { D_PX, 0, DEF_PX },
        { D_PY, 0, DEF_PY }
};

#define C_LENGTH        NUMS[0].cn_value
#define C_WIDTH         NUMS[1].cn_value
#define C_XRANGE        NUMS[2].cn_value
#define C_YRANGE        NUMS[3].cn_value

struct c_nums *END_NUMS = &NUMS[(sizeof (NUMS)/sizeof (struct c_nums))];

struct c_flgs FLGS[] = {
        { D_SH, FALSE },
        { D_SF, FALSE },
        { D_FO, FALSE }
};

#define C_SHEADER       FLGS[0].cf_value
#define C_SFORM         FLGS[1].cf_value
#define C_FORMOPEN      FLGS[2].cf_value

struct c_flgs *END_FLGS = &FLGS[(sizeof (FLGS)/sizeof (struct c_flgs))];

/* Номера сообщений */
#define M_BADLOCK       MSGS[0]
#define M_CHDIR         MSGS[1]
#define M_EMPTY         MSGS[2]
#define M_NODEAMON      MSGS[3]
#define M_OPEN          MSGS[4]
#define M_READ          MSGS[5]
#define M_STAT          MSGS[6]
#define M_UNKNOWN       MSGS[7]
#define M_USAGE         MSGS[8]

/* Тексты сообщений */
char   *MSGS[] = {
    "bad lock file, deamon was died",
    "cannot chdir to",
    "empty spooling queue on printer",
    "deamon not exist",
    "cannot open",
    "cannot read",
    "cannot stat",
    "unknown printer",
    "usage: lpq [-l] [-P<printer>] [job# ... | user ...]"
};









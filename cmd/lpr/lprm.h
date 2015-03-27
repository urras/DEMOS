/* Номера сообщений */
#define M_CHDIR         MSGS[0]
#define M_EMPTY         MSGS[1]
#define M_MSGS          MSGS[2]
#define M_OPEN          MSGS[3]
#define M_READ          MSGS[4]
#define M_REQUEST       MSGS[5]
#define M_UNKNOWN       MSGS[6]
#define M_UNLINK        MSGS[7]
#define M_USAGE         MSGS[8]
#define M_USER          MSGS[9]

/* Тексты сообщений */
char   *MSGS[] = {
    "cannot chdir to",
    "empty spooling queue on printer",
    "remove job #",
    "cannot open",
    "cannot read",
    "what shell I do?",
    "unknown printer",
    "cannot unlink",
    "usage: lprm [-P<printer>] [-] { job# ... | user ...}",
    "who are You?"
};


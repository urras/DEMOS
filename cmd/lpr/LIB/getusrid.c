#include <pwd.h>

/*
 * ��������� ���������� � ������������
 * �� ����� ������� (/etc/passwd).
 */

struct passwd  *
                get_usr_id () {
    register int    uid = getuid ();
    register int    gid = getgid ();
    register struct passwd *pwd;
    extern struct passwd   *getpwent ();

    while ((pwd = getpwent ()))
        if (pwd -> pw_uid == uid
                && pwd -> pw_gid == gid)
            break;
    endpwent ();
    return (pwd);
}

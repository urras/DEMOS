#if !defined(_dir_h_) && !defined(_sys_dir_h_)
#   define _dir_h_
#   define _sys_dir_h_

#ifndef _sys_types_h_
#include <sys/types.h>
#endif

#ifndef DIRSIZ
#   define DIRSIZ  14
#endif

#define D_NAME DIRSIZ
#define dir direct

/* Пользователи имеют тенденцию хапать не ту структуру, которую нужно. */
/** #if !defined(IPK_DIRECTORY) && !defined(KERNEL) **/
/* стандартная структура каталога */
/*
 * struct  direct
 * {
 *         ino_t   d_ino;
 *         char    d_name[DIRSIZ];
 * };
 */
/** #else **/

/* расширенная структура каталога */
struct direct
{
        ino_t    d_ino   : 14;     /* i-number */
        unsigned d_class : 2;      /* class of file */
        char     d_name[DIRSIZ];   /* base name     */
};

# define DIR_OLD    00
# define DIR_IFDIR  01
# define DIR_IFLNK  02
# define DIR_IFREG  03

/** #endif IPK_DIRECTORY **/

#endif _dir_h_ _sys_dir_h_


+                  Standalone в Unix (общая структура)

     -- Makefile:

 LIBSA = libsa

 CFLAGS = -o -DSTANDALONE $[INCLUDES]

 libsa.a: $[LIBSA] saio.h
         cd libsa
         make INCLUDES="$[INCLUDES]" COFLAGS="$[COFLAGS]" ../libsa

 mkfs: libsa.a srt0.o mkfs.c c.o
         cc -c $[CFLAGS] mkfs.c
         ld -o mkfs  srt0.o mkfs.o c.o libsa.a -lc

     -- Содержимое и назначение компонент (предположительно):

 libsa.a         Модуль SYS.c содержит функции read,open etc.,
                 а также trap - обработчик уклонений.
 saio.h          Определения для вектора устройств devsw;
                 Размещение iob[] - т.е. описателя открытого файла.
                 Saio.h включается в c.c, а не в main.
 c.c,    c.o     Функции devXXX(iob*); Вектор опис. устр. devsw[].
 srt0.s, srt0.o  Стартер. Определяет наличие CSW, video; чистит bss.
 libc.a (-lc)    Стандартные функции типа strcpy

     -- Последовательность исполнения запросов в/в:

     Программа выдает open("xx",MODE) или read(fd,buff,len) (из SYS.o).
     Функции  open  &  read  читают  карту  диска и/или модиф-ют пар-ры
 файла в структуре iob.
     Вызывается devopen(*iob) или devread(*iob).
 devopen(*iob) вызывает xxopen(*iob), а devread вызывает
 xxstrategy(*iob, OPCODE), где OPCODE - READ,WRITE (SEEK ?).
     Xxopen и xxstrategy - функции физического драйвера.

     -- Имя standalone-файла:

     tm(0,5)  или  rk(0,0)conf-rk  или  rk(1,0)/src/includes/inode.h
     !  ! !        !  ! ! !
     !  ! !        !  ! ! `------- имя файла для SYS'find()
     !  ! !        !  ! !
     !  ! `--------!--!-+--------- т.н. смещение; параметр для devopen.
     !  !          !  !            на ленте - номер файла между TM.
     !  !          !  !
     !  `----------!--+----------- номер устройства.
     !             !
     `-------------+-------------- имя сопряжения (берется из devsw).

     -- Структура диска:

 /* Mitra-225 */
 #if     UCB_NKB == 0
 %- #define CLSIZE  0            /* number of blocks / cluster */
 #define BSIZE   0x100           /* size of secondary block (bytes) */
 #define NINDIR  (BSIZE/sizeof(daddr_t))
 #define BMASK    0xFF           /* BSIZE-1 */
 #define BSHIFT      8           /* LOG2(BSIZE) */
 #define NMASK    0x3F           /* NINDIR-1 */
 #define NSHIFT      6           /* LOG2(NINDIR) */
 #endif
 #define INOPB       4
 #define NADDR      10

     Диск Unix'а состоит из:
 0-й физический блок - загрузчик (и метка тома ?)
 1-й            блок - суперблок (информационная часть метки)
  далее до s_isize   - блоки с i-узлами.
  далее до s_fsize   - дисковое пространство.

     Для  stand-alone  вторая  чифра в имени (i_boff) означает смещение
 вектора i-узлов в физ. блоках от начала физ. тома.

     Не понятно только, где лежит зона свопа.
     Корневой каталог имеет номер i-узла 2.

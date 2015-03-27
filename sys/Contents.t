
     Файлы ядра Unix

 sysent.c       - Таблица системых вызовов
 main.c         - Инициатор системы
 boot.s         - ?
 mch1.s         - Входы для дампа на магнитную ленту, перезагрузки,
                  системных вызовов (sysent), emt, ошибки шины,
                  неверной инструкции, а также секция возврата из
                  аварийных прерываний backup.
                : Для mitra его роль исполняет mch1m
 mch2.s         - Программы типа fuword, t.e. взаимодействие адресных
                  пространств. Кстати, содержит описание u(ser) и ovno.
                : Для mitra его роль исполняют
                  fetchstore, save. Кое-что может попасть в dummies.
 trap.c         - Функции trap & syscall (входы в ядро).
 sys1.c         - Вызовы управления процессами
 sys2.c         - Вызовы файловой системы, кроме stat.
 sys3.c         - Вызовы stat, fstat.
 nami.c         - namei(), uchar(), schar();
  namei -> * Convert a pathname into a pointer to a locked inode,
           * with side effects usable in creating and removing files.
           * This is a very central and rather complicated routine.
 prf.c          - printf, panic etc. Используют putchar консольного
                  терминала.
 machdep.c      - Теоретически здесь должны быть сосредоточены машин-
                  нозависимые функции на "c" (dorti, mapx etc.)
                : Для mitra его роль исполняет machdep.
 slp.c          - Синхронизация и планирование.
                  Секции: qswtch, swtch.
 sched          - Секция sched и процесс swapper.
                : На mitra swapper вынесен в отдельный файл.

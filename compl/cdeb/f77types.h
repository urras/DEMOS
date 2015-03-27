#ifdef CDEBUG
/* Файл ftypes */
/* variable types
 * numeric assumptions:
 *      int < reals < complexes
 *      TYDREAL-TYREAL = TYDCOMPLEX-TYCOMPLEX
 */

#define TYUNKNOWN 0
#define TYADDR 1
#define TYSHORT 2
#define TYLONG 3
#define TYREAL 4
#define TYDREAL 5
#define TYCOMPLEX 6
#define TYDCOMPLEX 7
#define TYLOGICAL 8
#define TYCHAR 9
#define TYSUBR 10
#define TYERROR 11
#define NTYPES (TYERROR+1)
#endif
union cdebdesc {
        unsigned cdebword;
        struct {
unsigned cdbtype: 4;    /* Тип    */
unsigned cdbfarr: 1;    /* Массив */
unsigned cdbfarg: 1;    /* Формальный параметр */
unsigned cdbleng:10;    /* Длина переменной - для character */
        };
};
#define CDBFORM "~l~%06o\t=%o\n"  /* Формат, параметр - длина массива */
#define CDBOFFS "~~%s=%o\n"  /* Формат задания сдвига: common,сдвиг   */
#define FRMCOMMON 0100000
/* Формат записи для cdebug:
   для элементов - не членов common:
   имя = значение
   l~<длина> = описатель
   после "имя" может следовать запись v.*, которую нужно пропускать.
   для членов common:
   имя = FRMCOMMON
   ~~имябл = сдвиг
   l~<длина> = описатель
*/

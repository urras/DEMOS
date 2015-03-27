#define MAXPTR  (char *)-1	/* max value of any pointer
				   variable */

# define EM     t.Em
# define HOR    t.Hor
# define VERT   t.Vert
# define SPS    INCH/10		/* space size */
# define SS     INCH/10		/* " */
# define TRAILER 0
# define UNPAD  0227
# define PO     0		/* page offset */
# define ASCII  1
# define PTID   1
# define LG     0
# define DTAB   0		/* set at 8 Ems at init time */
# define ICS    2*SPS
# define TEMP   512             /* 65K */


#include <signal.h>

#define NARSP   0177		/* narrow space */
#define HNSP    0226		/* half narrow space */
#define PS      10		/* default point size */
#define FT      0		/* default font position */
#define LL      65*INCH/10	/* line length; 39 picas = 6.5
				   in */
#define VS      INCH/6		/* vert space; 12 points */
#define NN      200		/* number registers */

#define NNAMES  14		/* predefined reg names */
#define NIF     15		/* if-else nesting */
#define NS      64		/* name buffer */
#define NTM     256		/* tm buffer */
#define NEV     3		/* environments */
#define EVLSZ   10		/* size of ev stack */
#define EVS     4*256		/* environment size in words */
#define NM      300		/* requests + macros */
#define DELTA   512		/* delta core bytes */
#define NHYP    10		/* max hyphens per word */
#define NHEX    128		/* byte size of exception word
				   list */
#define NTAB    35		/* tab stops */
#define NSO     5		/* "so" depth */
#define WDSIZE  170		/* word buffer size */
#define LNSIZE  680		/* line buffer size */
#define NDI     5		/* number of diversions */

#define DBL     0100000		/* double size indicator */
#define MOT     0100000		/* motion character indicator 
				*/
#define MOTV    0160000		/* clear for motion part */
#define VMOT    0040000		/* vert motion bit */
#define NMOT    0020000		/* negative motion indicator */
#define MMASK   0100000		/* macro mask indicator */
#ifndef ASA
#define CMASK   0100377
#else
#define CMASK   0100777
#endif

#ifndef ASA
#define ZBIT    0400		/* zero width char */
#else
#define ZBIT    01000           /* zero width char */
#endif
#define BMASK   0377
#define BYTE    8
#define IMP     004		/* impossible char */
#define FILLER  037
#define PRESC   026
#ifdef ASA
#define HX      022             /* High-order part of xlss */
#define LX      021             /* low-order part of xlss */
#else
#define HX      0376		/* High-order part of xlss */
#define LX      0375		/* low-order part of xlss */
#endif  ASA
#define CONT    025
#define COLON   013
#define XPAR    030
#define ESC     033
#define FLSS    031
#define RPT     014
#ifndef ASA
#define JREG    0374
#else
#define JREG    021
#endif  ASA

#define NTRAP   20		/* number of traps */
#define NPN     20		/* numbers in "-o" */

#define T_PAD   0101		/* cat padding */
#define T_INIT  0100
#define T_IESC  16		/* initial offset */
#define T_STOP  0111

#define NPP     10		/* pads per field */
#define FBUFSZ  256		/* field buf size words */
#define OBUFSZ  512		/* bytes */
#define IBUFSZ  512		/* bytes */
#define NC      256		/* cbuf size words */
#define NOV     10		/* number of overstrike chars 
				*/
#define ZONE    (-3)		/* MSK time zone */

#define TDELIM  032
#define LEFT    035
#define RIGHT   036
#define LEADER  001
#define TAB     011

#define TMASK   037777
#define RTAB    0100000
#define CTAB    0040000
#define OHC     024

#define PAIR(A,B) (A|(B<<BYTE))

#define BLK     128		/* alloc block words */

#ifdef BIG

typedef long    filep;
# define NBLIST  BIG		/* allocation , BIG = 256 per
				   65k */
# define BLKBITS 7		/* for BLK=128 */

#else

typedef unsigned        filep;
# define NBLIST  TEMP		/* a llocation list, TEMP<=512 
				*/
 /* BLK*NBLIST<=65536 words, if filep=unsigned */
# define BLKBITS 0

#endif

#ifdef SOAN
# define  DEMOS			/* Операционная система ДЕМОС  
				   */
# define  RUSMAC		/* В макросах можно исп.
				   русские буквы  */
#endif SOAN

#ifdef  ASA
extern  int trtab[];
#endif



#
/*
**  LOCAL.H -- локальные параметры зависящие от типа ЭВМ
**
**
**      Версия:
**              $Header$ @(#)common.h    7.1     2/5/81
*/

 
/*
**      Машинно-зависимые определения
**
**      Возможны пока: VAX, pdp11, MC68000
*/

#ifndef _LOCAL
#define _LOCAL

#define FAR /* */
#ifdef mc68
#define MC68000
# define        DEMOS
#define FCNTL
#define SYSTEM_V
#define NOSOCKET
/* # define        EDIAG        */
#endif

#ifdef vax
#define VAX
#ifndef ultrix
# define        DEMOS
# define        EDIAG
#else
#define FCNTL
#define NOSOCKET
#endif
#endif

#ifdef i386
#define s_386
#define SYSTEM_V
#define FCNTL
#define NOSOCKET
#endif

#if defined(M_XENIX) && defined(M_I386)
#define s_386
#define SYSTEM_V
#define FCNTL
#define NOSOCKET
#else
#if defined(M_XENIX) && defined(M_I86SM)
#define s_xenix
#define SYSTEM_V
#define FCNTL
#endif
#if defined(M_XENIX) && defined(M_I86MM)
#define s_xenix
#define m_xenix
#define SYSTEM_V
#define FCNTL
#endif
#endif

#ifdef pdp11
/* pdp11 itself !!! */
# define        DEMOS
# define        EDIAG
#endif

/*
 * Определяем
 * WORDSIZE   - размер слова
 * LOG2WORDSIZE степень двойки для размера слова
 * ORDER      - порядок байтов в слове b0,b1,b2,b3
 *                                     b0 - старший
 *                                     b3 - младший
 * NOSOCKET  - не использовать механизм связи при работе с семафором
 * MINI, MIDDLE, BIG    - размер таблиц и системы
 * ROUND - признак выравнивания целых и вещественных на границу слова
 * и длина слова для выравнивания
 * Внимание - некоторые зависимости есть в tree.h !!!
 * NOCYRFILES   - запрет на образование файлов с русскими именами
 */

# ifdef pdp11
# define        POINTSIZE       16
# define	WORDSIZE	16	/* number of bits in word */
# define	LOG2WORDSIZE	4	/* log base 2 of WORDSIZE */
# define        ORDER(b0,b1,b2,b3)      b1,b0,b3,b2
# define        NOSOCKET
# define        FCNTL
# define        MINI
# define        ROUND 2
/* # define        MICRO        */
# endif

# ifdef s_xenix
# define        POINTSIZE       16
# define	WORDSIZE	16	/* number of bits in word */
# define	LOG2WORDSIZE	4	/* log base 2 of WORDSIZE */
# define        ORDER(b0,b1,b2,b3)      b3,b2,b1,b0
# define        NOSOCKET
# define        MINI
#       ifdef m_xenix
#       define  FPOINTSIZE      32
#       else
#       define  FPOINTSIZE      16
#       endif
# endif

# ifdef s_386
# define        POINTSIZE       32
# define	WORDSIZE	32	/* number of bits in word */
# define	LOG2WORDSIZE	5	/* log base 2 of WORDSIZE */
# define        ORDER(b0,b1,b2,b3)      b3,b2,b1,b0
# define        BIG
# endif

# ifdef VAX
# define        POINTSIZE       32
# define	WORDSIZE	32	/* number of bits in word */
# define	LOG2WORDSIZE	5	/* log base 2 of WORDSIZE */
# define        ORDER(b0,b1,b2,b3)      b3,b2,b1,b0
# define        BIG
# define        NOCYRFILES
# endif

# ifdef MC68000
# define        POINTSIZE       32
# define	WORDSIZE	32	/* number of bits in word */
# define	LOG2WORDSIZE	5	/* log base 2 of WORDSIZE */
# define        ORDER(b0,b1,b2,b3)      b0,b1,b2,b3
# define        NOSOCKET
# define        BIG
# define        ROUND 2
# define        NOCYRFILES
# endif

# ifdef EDIAG
#include <ediag.h>
# else
# define ediag(l,r) (l)
# endif

#ifdef SYSTEM_V
#define index strchr
#define rindex strrchr
#endif
#endif

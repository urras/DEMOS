#ifndef _sys_koverlay_h_
#       define  _sys_koverlay_h_

/*
 *      Definitions relating to kernel text overlays.  The kernel and
 *      stand/bootstrap/boot.c must be recompiled if these are changed.
 *      In practice, it is very difficult to change these numbers since
 *      many changes will be necessary in mch.s.  This file is provided
 *      mainly to specify how kernel overlays are laid out, not for tuning.
 *
 *
 *      Primary variables:
 *              # ovly regs     the maximum size of a kernel
 *                              overlay segment will be
 *                              8k bytes * # ovly regs.
 *                              Currently, must be 1.
 *
 *              OVLY_TABLE_BASE the address in l.s where the
 *                              kernel stores the prototype
 *                              PARs and PDRs.  The only one
 *                              of the constants in this file
 *                              which may readily be changed.
 *
 *              NUM_TEXT_REGS   the maximum size of the kernel
 *                              base text segment will be
 *                              8k bytes * NUM_TEXT_REGS.
 */

/*
 * Т.к. сделано быстрое переключение оверейных сегментов,
 * то все C-модули вынесены из базового сегмента в оверлейные.
 * При этом базовый сегмент уменьшился до одного листа (стал < 8 Кб).
 * В связи с этим изменились некоторые значения в этом файле.
 * Для обращения к старым значениям (что использует, например boot)
 * надо обращаться с именами начинающимися на 'O' (old).
 */

#define OVLY_TABLE_BASE         01000

#define I_NUM_TEXT_REGS         7       /* 8 - # ovly regs */
#define N_NUM_TEXT_REGS         1
#define ON_NUM_TEXT_REGS        2       /* for 16Kb base segment */
#ifndef KERN_NONSEP
# define NUM_TEXT_REGS          I_NUM_TEXT_REGS
#else
# define NUM_TEXT_REGS          N_NUM_TEXT_REGS
#endif  KERN_NONSEP

#define I_MAX_DATA_REGS         6
#define N_MAX_DATA_REGS         4       /* 6 - NUM_TEXT_REGS - # ovly regs */
#define ON_MAX_DATA_REGS        3       /* for 16Kb base segment */
#ifndef KERN_NONSEP
# define MAX_DATA_REGS          I_MAX_DATA_REGS
#else
# define MAX_DATA_REGS          N_MAX_DATA_REGS
#endif  KERN_NONSEP

#define I_DATA_PAR_BASE         KDSA0
#define N_DATA_PAR_BASE         KISA2   /* &KISA[NUM_TEXT_REGS + # ovly regs] */
#define ON_DATA_PAR_BASE        KISA3   /* for 16Kb base segment */
#ifndef KERN_NONSEP
# define DATA_PAR_BASE          I_DATA_PAR_BASE
#else
# define DATA_PAR_BASE          N_DATA_PAR_BASE
#endif  KERN_NONSEP

#define I_DATA_PDR_BASE         KDSD0
#define N_DATA_PDR_BASE         KISD2   /* &KISD[NUM_TEXT_REGS + # ovly regs] */
#define ON_DATA_PDR_BASE        KISD3   /* for 16Kb base segment */
#ifndef KERN_NONSEP
# define DATA_PDR_BASE          I_DATA_PDR_BASE
#else
# define DATA_PDR_BASE          N_DATA_PDR_BASE
#endif  KERN_NONSEP

#define TEXT_PAR_BASE           KISA0
#define TEXT_PDR_BASE           KISD0
#ifdef  KERN_NONSEP
# define OVLY_PAR               KISA1   /* &KISA[NUM_TEXT_REGS] */
# define OOVLY_PAR              KISA2   /* for 16Kb base segment */
# define OVLY_PDR               KISD1   /* &KISD[NUM_TEXT_REGS] */
# define OOVLY_PDR              KISD2   /* for 16Kb base segment */
#else
# define OVLY_PAR               KISA7   /* &KISA[NUM_TEXT_REGS] */
# define OVLY_PDR               KISD7   /* &KISD[NUM_TEXT_REGS] */
#endif

#endif  _sys_koverlay_h_

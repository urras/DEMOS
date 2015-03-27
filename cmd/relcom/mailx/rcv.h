/*
 * Mail -- a mail program
 *
 * This file is included by normal files which want both
 * globals and declarations.
 *
 */

#ifdef  pdp11
#include <whoami.h>
#endif

/* serg  #define ediag(e,r) (_ediag?(e):(r)) */
#define ediag(e,r) ((e))
extern _ediag;
#define EDIAG_R 0
#define EDIAG_E 1

#include "def.h"
#ifndef MAIN_INIT
#define EXTERN extern
#else
#define EXTERN
#endif
#include "glob.h"

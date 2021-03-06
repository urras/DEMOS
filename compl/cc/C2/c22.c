#ifndef lint
/*NOXSTR*/
static char rcsid[] = "$Header: c22.c,v 2.10 88/06/10 20:32:15 dvolodin Stab $" ;
/*YESXSTR*/
#endif  lint

#include "c2.h"

struct optab optab[] = {
	"jbr",  JBR,
	"jeq",  CBR | JEQ<<8,
	"jne",  CBR | JNE<<8,
	"jle",  CBR | JLE<<8,
	"jge",  CBR | JGE<<8,
	"jlt",  CBR | JLT<<8,
	"jgt",  CBR | JGT<<8,
	"jlo",  CBR | JLO<<8,
	"jhi",  CBR | JHI<<8,
	"jlos", CBR | JLOS<<8,
	"jhis", CBR | JHIS<<8,
	"jmp",  JMP,
	".globl",EROU,
	"mov",  MOV,
	"clr",  CLR,
	"com",  COM,
	"inc",  INC,
	"dec",  DEC,
	"neg",  NEG,
	"tst",  TST,
	"asr",  ASR,
	"asl",  ASL,
	"sxt",  SXT,
	"cmp",  CMP,
	"add",  ADD,
	"sub",  SUB,
	"bit",  BIT,
	"bic",  BIC,
	"bis",  BIS,
	"mul",  MUL,
	"ash",  ASH,
	"xor",  XOR,
	".text",TEXT,
	".data",DATA,
	".bss", BSS,
	".even",EVEN,
	"movf", MOVF,
	"movof",MOVOF,
	"movfo",MOVFO,
	"addf", ADDF,
	"subf", SUBF,
	"divf", DIVF,
	"mulf", MULF,
	"clrf", CLRF,
	"cmpf", CMPF,
	"negf", NEGF,
	"tstf", TSTF,
	"cfcc", CFCC,
	"sob",  SOB,
	"jsr",  JSR,
	".end", END,
	0,      0};

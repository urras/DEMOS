#ifndef _sys_pcreg_h_
#       define _sys_pcreg_h_
/*
 * Регистры перфоленточной станции СМ 6204
 */

#define PC_RDRENB  01
#define PC_IENABLE 0100
#define PC_DONE    0200
#define PC_BUS     04000
#define PC_ERROR   0100000

struct pcdevice {
	int pcrcsr;
	int pcrbuf;
	int pcpcsr;
	int pcpbuf;
};

#endif _sys_pcreg_h_

#ifndef _core_h_
#       define  _core_h_

/* machine dependent stuff for core files */
#define TXTRNDSIZ       8192L
#define stacktop(siz)   (0x10000L)
#define stackbas(siz)   (0x10000L-(siz))

#endif  _core_h_

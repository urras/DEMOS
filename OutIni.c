/*
 * LSI-11 Linker
 * Запись/чтение выходного файла
 * Метод работы: прочесть-настроить-записать.
 */
#include <SysStrings>                   /* MVS() */
#include <FileSystem>
#define LFNAME  (sizeof(Fname))
#include <FileIO>
#define READL   2
#define WRITEL  3
#include <SysErrors>
#include "LinkerDefs"

static char outname[ LFNAME ];

static unsigned block;  static int index;  /* Указатель ввода/вывода */
#define BUFLENG (SectorSize*4)
static union {
    char fbuff[ BUFLENG ];
    int buff_padder;
} ub;
static long totleng;          /* Длина файла, записанного на диск */
static int secsize;           /* Размер сектора */
static char flab;

/*
 * Открыть временный файл
 */
void OutIni( fname )
    char *fname;
{
    int rc;
    FileItem ipar;
    int i;

    for( i = 0; i < LFNAME; i++ ){
        outname[ i ] = (*fname != '\0') ? *fname++ : ' ';
    }
    MVS( outname, LFNAME, ipar.name.c );    ipar.type = '-';
    ipar.access = ipar.protection = 0;    ipar.p.pwd = 0;
    if( (rc = DoUniqName( &ipar, WorkCatal )) < 0 ) Error(11);
    flab = (char) rc;
    secsize = GetSectorSize( flab );

    block = 0;    index = 0;    totleng = 0l;
}

/*
 * Прочесть 2 байтa; Указатель в/в НЕ СМЕЩАЕТСЯ!
 */
unsigned OutWGet( Offset )
    unsigned Offset;
{
    unsigned AskBlock;
    IOl_block iopar;
    int rc;
    long leng;

    if( Offset & 1 ) Error(14);
    if( (AskBlock = Offset/secsize) < block ||
        AskBlock >= block+BUFLENG/secsize )
    {
        if( index != 0 ){
            iopar.label = flab;    iopar.dir = WRITEL;
            iopar.bufadr = ub.fbuff;
            leng = ((long)block)*secsize + (iopar.buflen = index);
            if( leng >= totleng ){      /* Длина возросла ? */
                totleng = leng;
            }else{                      /* В буфере больше, чем index*/
                iopar.buflen = BUFLENG; /* Чтобы не обрезать по index*/
            }
            iopar.sector = (long) block;
            if( (rc = _io_( &iopar )) >> 8 == (~0) ){
                if( rc != ErrDiskFull ) Error(12);
                Error(13);              /* Диск переполнен */
            }
        }
        iopar.dir = READL;
        iopar.buflen = BUFLENG;
        iopar.sector = (long) (block = AskBlock);
        if( (rc = _io_( &iopar )) >> 8 == (~0) ){
            if( rc != ErrFileAddr ) Error(14);
        }

        index = Offset % secsize;
    }else{
        index = Offset - block*secsize;
    }

    return (ub.fbuff[ index+1 ] << 8) + (ub.fbuff[ index ] & 0xFF);
}

/*
 * Записать в файл; Указатель в/в смещается обычным порядком
 */
void OutWrite( Buff, Len )
    register char *Buff;
    int Len;
{
    IOl_block iopar;
    int rc;
    long leng;

    for( ; --Len >= 0; ){
        ub.fbuff[ index ] = *Buff++;
        if( ++index >= BUFLENG ){
            iopar.label = flab;    iopar.dir = WRITEL;
            iopar.bufadr = ub.fbuff;
            iopar.buflen = BUFLENG;
            iopar.sector = (long) block;
            if( (rc = _io_( &iopar )) >> 8 == (~0) ){
                if( rc != ErrDiskFull ) Error(12);
                Error(13);              /* Диск переполнен */
            }
            block += BUFLENG/secsize;
            index = 0;
        }
    }
    if( (leng = block*secsize + index) >= totleng ) totleng = leng;
}

void OutClose()
{
    static char out_proc[8] = {
        0x02,0x1F,0xBE,0xA0,0x0C,0xD0,0x00,0x00
    };

    int rc;

    {
      static IOl_block iopar;
      long leng;

      if( index != 0 ){
          iopar.label = flab;    iopar.dir = WRITEL;
          iopar.bufadr = ub.fbuff;
          iopar.buflen = BUFLENG;
          leng = ((long)block)*secsize + index;
          if( leng >= totleng ){
              totleng = leng;
              iopar.buflen = index;
          }
          iopar.sector = (long) block;
          if( (rc = _io_( &iopar )) >> 8 == (~0) ){
              if( rc != ErrDiskFull ) Error(12);
              Error(13);              /* Диск переполнен */
          }
      }
      SetEOF( flab, (totleng + (secsize-1)) / secsize );
    }

    {
      static FileItem fpar;
      char *cp;
      Statistics fib;     /* Буфер информации о файле */

      fpar.type = 'B';
      MVS( outname, LFNAME, fpar.name.c );
      DeleteName( &fpar, WorkCatal );/* Стираем, может дать EFILNU */

      fpar.protection = 0;
      fpar.access = 2;
      {  register FileItem *ip;
          fpar.p.pwd = ((ip=GetFileItem(OwnCatal)) == 0)? 0: ip->p.pwd;
      }

      Tget( &fib );                  /* Получаем размеры программы */
      cp = (char*) &fpar.info;
       /* Размер кода */
      *cp++ = (char) fib.Stlen;  *cp++ = (char) (fib.Stlen >> 8);
       /* Размер данных */
      *cp++ = (char) fib.Sdlen;  *cp++ = (char) (fib.Sdlen >> 8);
       /* Размер буферов */
      *cp++ = (char) fib.Sblen;  *cp++ = (char) (fib.Sblen >> 8);
       /* Зарезервировано */
      *cp++ = *cp++ = 0;

      MVS( out_proc, 8, (char*) &fpar.processor );
        /*Переименовываем*/
      if( RenameFile( &fpar, flab, WorkCatal ) < 0 ){
          Error(15);
      }
    CloseFile( flab );
    }
}

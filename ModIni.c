/*
 * LSI-11 linker
 * Функции доступа к об'ектным модулям
 * Используется a.out-подобный формат, но заголок находится в статье.
 */
#include <FileStruct>
#include <FileSystem>
#define LFNAME  sizeof(Fname)
#include <FileIO>
#define READL   2
#include <SysErrors>
#include <SysStrings>         /* MVS() */
#include "LinkerDefs"
#include "CatalDefs"

static Module  modvect[ MAXMOD ];    static int Nmods;
static Library libvect[ MAXLIB ];    static int Nlibs;

#define ReadLeng (SectorSize*4)
static struct {
    int bounder;
    char buffer[ ReadLeng ];
} bu;

 /* Описатель текущего буфера */
static int savactual;
static unsigned savblock;
static unsigned savleng;

 /* Описатель текущего модуля */
static unsigned blbase; /* База модуля в файле (сектор и индекс)... */
static int ibase;       /* .. для модуля в отдельном файле == 0,0   */
static unsigned block;  /* Указатель в файл (секторы). Для библиотеки
                         * можно непосредственно использовать в io() */
static unsigned fstop;  /* Длина в секторах, для библиотечного -
                         * адрес блока следующего файла */
static int lastfill;    /* Количество байт в последнем секторе */
static int begind;      /* Смещение относительно block */
static int secsize;     /* Размер сектора в байтах */
static Module *curmod;  /* Текущий модуль */
static char flab;       /* Метка файла (если не библиотечный) */

#define SECTOR secsize

static int modnext;

/*
 * Инициация в начале всей работы
 */
void ModIni()
{
    FileItem *ip;
    int rc;

    /* Открываем все библиотеки */
    Nlibs = 0;
    CatIni( WorkCatal );
    while( (ip = CatNext()) != NULL ){
        if( (ip->type & 0x7F) == 'U' ){
            if( (rc = FindFile( ip, WorkCatal )) < 0 ){
                Error(3);        /* Таковая не найдена */
            }
            libvect[ Nlibs ].Lparent = WorkCatal;
            libvect[ Nlibs ].Llabel = (char) rc;
            MVS( ip->name.c, LFNAME, libvect[ Nlibs ].Lname );
            Nlibs++;
        }
    }

    Nmods = 0;                   /* Bss у нас не чистится */
    modnext = 0;                 /* для второго прохода */
}

 /* Выделение и заполнение элемента таблицы модулей */
 /* Кое-что заполняется потом: Mlibflag и Mxxxbase */
static Module *EnterModule( FileItem *ip );
static Module *EnterModule( ip )
    FileItem *ip;
{
    Module *mp;
    char *pci;

    if( Nmods == MAXMOD ) Error(5);
    mp = & modvect[ Nmods++ ];
    MVS( ip->name.c, LFNAME, mp->Mfilnam );    SetDiag( ip->name.c );
    mp->Mldbase = 0;    mp->Mbssoff = 0;     /* Это заполняет Tcopy() */
     /* Единственно переносимый способ чтения статьи файла */
    pci = (char*) &ip->info;
    mp->Mtextsize = (pci[ 0 ] << 8) + pci[ 1 ];
    mp->Mdatasize = (pci[ 2 ] << 8) + pci[ 3 ];
    mp->Mbsssize  = (pci[ 4 ] << 8) + pci[ 5 ];
    mp->Msymsize  = (pci[ 6 ] << 8) + pci[ 7 ];

    mp->Mbegsym = NULL;

    return mp;
}

/*
 * Открываем файл и заполняем статическую информацию (2 варианта)
 */
static void OpenLib( FileItem *, Module * );
static void OpenLib( ip, mp )
    FileItem *ip;
    Module *mp;
{
    SECTOR = GetSectorSize( mp->Mvar.Mlib.Mlibptr->Llabel );
    if( SECTOR > ReadLeng ) Error(15);
    blbase = block = ip->ident;
    ibase = begind = ip->p.shift;
    fstop = (ip+1)->ident;
    lastfill = (ip+1)->p.shift;
    if( lastfill != 0 ) fstop++;  else lastfill = SECTOR;
    flab = 0;
}

static void OpenObj( char );
static void OpenObj( fl )
    char fl;
{

    SECTOR = GetSectorSize( fl );
    if( SECTOR > ReadLeng ) Error(15);
    block = blbase = 0;
    fstop = (int) GetFileLeng( fl );
    lastfill = SECTOR;    begind = ibase = 0;
    flab = fl;
}

/*
 * Открыть модуль по имени (т.е. поиск)
 * Из последнего открытого модуля возможно чтение.
 */
Module *ModOpen( name )
    char *name;
{
    static FileItem ipar;

    Module *mp;
    FileItem *ip;

    { register int i;    register char *ns = name;
        for( i = 0; i < LFNAME; i++ ){
            ipar.name.c[ i ] = (*ns != '\0') ? *ns++ : ' ';
        }
        ipar.type = 'O';
    }

    savactual = FALSE;              /* Теряем буфер (другого файла) */

    /*
     * Ищем в рабочем каталоге
     */
    {  register rc;
        rc = FindFile( &ipar, WorkCatal );
        if( rc >= 0 ){              /* Файл модуля найден */
            ip = GetFileItem( (char) rc );
            mp = EnterModule( ip ); /* Резервируем описатель */
            mp->Mlibflag = Mobjfile;
            OpenObj( (char)rc );    /* Задание статики для чтения */
            return curmod = mp;
        }else if( rc != ErrFile ){  /* Какая-то ошибка файл. системы */
            Error(4);
        }
    }

    /*
     * Ищем в библиотеках
     */
    {  int lct;                     /* Libraries CounTer */
        for( lct = 0; lct < Nlibs; lct++ ){
            CatIni( libvect[ lct ].Llabel );
            while( (ip = CatNext()) != NULL ){
                if( (ip->type & 0x7F) == 'O' &&
                    _cmps( ip->name.c, ipar.name.c, LFNAME ) == 0 )
                {
                    mp = EnterModule( ip );   /* Получаем описатель */
                    mp->Mlibflag = Mlibrary;  /* Заносим вариант */
                    mp->Mvar.Mlib.Mlibptr = & libvect[ lct ];
                    OpenLib( ip, mp );
                    return curmod = mp;
                }
            }
        }
    }

    Diagnose( 1, name );                    /* Сообщаем, что не нашли*/
    return curmod = NULL;                   /* Фига с маслом */
}

/*
 * Чтение из модуля (возможно, библиотечного)
 */
void ModRead( ppbuff, pleng )
    char **ppbuff;
    int *pleng;
{
    IOl_block iopar;

    /*
     * Проверяем на попадание в буфер
     */
    if( savactual ){          /* В буфере что-нибудь есть ? */
        if( block >= savblock && block < savblock + savleng/SECTOR ){
             /* Попадание в буфер */
            *ppbuff = bu.buffer + (block - savblock)*SECTOR + begind;
            *pleng = savleng - (*ppbuff - bu.buffer);
            if( savblock + savleng/SECTOR >= fstop ){
                *pleng -= (SECTOR-lastfill);
            }
            return;
        }
    }

    /*
     * Заполняем и отдаем буфер
     */
    if( curmod->Mlibflag != Mobjfile ){   /* Библиотечный модуль ? */
        iopar.label = curmod->Mvar.Mlib.Mlibptr->Llabel;
    }else{
        iopar.label = flab;
    }
    iopar.dir = READL;
    iopar.bufadr = bu.buffer;
    if( (iopar.buflen = ReadLeng)/SECTOR >= fstop-block ){
        iopar.buflen = (fstop-block)*SECTOR;
    }
    iopar.sector = (long) block;
    if( (int) _io_( &iopar ) >> 8 == (~0) ) Error(6);
    savblock = block;  savleng = iopar.buflen+(SECTOR-1) & ~(SECTOR-1);
    savactual = TRUE;           /* savxxxx теперь актуальны */
    block += iopar.buflen/SECTOR;

    *ppbuff = bu.buffer + begind;
    *pleng = iopar.buflen - begind;
    if( block >= fstop ) *pleng -= (SECTOR-lastfill);
    begind = 0;
}

/*
 * Смещение указателя чтения
 */
void ModSeek( offset )
    unsigned offset;          /* Смещение задается в байтах */
{
    block = blbase + (ibase + offset) / SECTOR;
    begind =         (ibase + offset) % SECTOR;
}

/*
 * Освобождение метки файла
 */
void ModClose()
{
    if( curmod->Mlibflag == Mobjfile ) CloseFile( flab );
    savactual = FALSE;
}

/*
 * Открытие модуля на втором проходе, т.е. без поиска
 */
Module *ModNext()
{
    Module *mp;

    if( modnext == Nmods ) return NULL;
    mp = & modvect[ modnext++ ];
    if( mp->Mlibflag != Mobjfile ){     /* Библиотека */
        FileItem *ip;

        CatIni( mp->Mvar.Mlib.Mlibptr->Llabel );
        loop{
            if( (ip = CatNext()) == NULL ) Error(7);
            if( (ip->type & 0x7F) == 'O' &&
                _cmps( ip->name.c, mp->Mfilnam, LFNAME ) == 0 ) break;
        }
        OpenLib( ip, mp );
    }else{
        int rc;
        FileItem csv_par;

        csv_par.type = 'O';
        MVS( mp->Mfilnam, LFNAME, csv_par.name.c );
        if( (rc = FindFile( &csv_par, WorkCatal )) < 0 ) Error(4);
        OpenObj( (char)rc );
    }

    SetDiag( mp->Mfilnam );
    return curmod = mp;
}

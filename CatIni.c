/*
 * LSI-11 Linker
 * Чтение каталогов и заголовков библиотек
 * (которые, к счастью, имеют одинаковый формат)
 */
#include <FileStruct>
#include <FileIO>
#include <SysErrors>
#include "CatalDefs"          /* Проверка своих прототипов */
#include "LinkerDefs"         /* Error() */

#define READLEN  (SectorSize*4)
static union {
    FileItem ibuff[ READLEN/sizeof(FileItem) ];
    char cbuff[ READLEN ];
} icu;
static IOl_block siop = { 0, 2, icu.cbuff, READLEN, 0l };

static FileItem *fip;

static void FillBuff( void );
static void FillBuff()
{
    int rc;
    int i;

    for( i = 0; i < READLEN/SectorSize; i += SectorSize ){
        icu.cbuff[ i ] = 0;
    }
    if( (rc = _io_( &siop ))>>8 == (~0) ){
        if( rc != ErrFileAddr ) Error(7);
    }
    siop.sector += siop.buflen/SectorSize;
}

void CatIni( clab )
    char clab;
{
    siop.label = clab;
    siop.sector = 0l;
    FillBuff();
    fip = icu.ibuff;
}

FileItem *CatNext()
{
    if( fip >= & icu.ibuff[ READLEN/sizeof(FileItem) ] ){
        FillBuff();
        fip = icu.ibuff;
    }
    if( fip->type == '\0' ) return NULL;
    return fip++;
}

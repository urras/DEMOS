#INCLUDE <UPPERCASE>        /*πεςελμΰώαεν λονπιμρτος ξα βομψϋιε βυλχω*/

#INCLUDE <DISPLAY>          /*DPC, DPQ  ETC.*/
#INCLUDE <SYSCALL>          /*σιστενξοε οπισαξιε UNIT'A*/

#DEFINE INTSIZE SIZEOF(INT) /*χοοβύε-το ϊδεσψ ότο ξε οδξο ι το φε*/
#DEFINE SECTOR  256

#DEFINE QBOOTS  10          /*λομιώεστχο ϊαηςυϊώιλοχ (θαςδχεςξωθ)*/
#DEFINE BOOTLEN 512
#DEFINE MAXTRY  5
#DEFINE MAXUNIT 4           /*λομ-χο υστςοκστχ χ λοξζιηυςαγιι ναϋιξω*/

#DEFINE WRITE     0X01
#DEFINE BL_BACK   0X05
#DEFINE WRITE_TM  0X08
#DEFINE WRITE_GAP 0X09

STRUCT UNITCB \(            /*υπς. βμολ UNIT'A*/
    CHAR *UBUFF;
    INT   ULENG;
    INT   UAUX[3];
\);
STRUCT UNITRET \(           /*ςεηιστςω, χοϊχςαύαενωε UNIT'OM*/
    INT SYSNUM;
    INT RETCODE;
\) UNIT();

#DEFINE INFOLN 512          /*δμιξα ιξζοςναγιοξξοκ ώαστι βμολα*/
STRUCT MTBLOCK \(           /*στςυλτυςα βμολα ξα μεξτε*/
    CHAR     BLMISS[4];         /*ξαδπισψ 'MISS'*/
    UNSIGNED BLADDR;            /*αδςεσα ϊαηςυϊλι βμολα ιμι ϊαπυσλα*/
    INT      BLENDMRK;          /*ναςλες λοξγεχοηο βμολα*/
    CHAR     BLBUFF[INFOLN];
\);
#DEFINE SEC_PER_BL (INFOLN/256)  /*λομιώεστχο MISS-σελτοςοχ χ βμολε*/

#PRAGMA STARTER(NULL);
MAIN()
\(
#PRAGMA USE(FIL:STRUCT);
    /*#DEFINE LBLIBR 0XF4*/
#PRAGMA USE(LIB:PARAMS);

% EQUALS I11END=&06,I11LBU=&08,I11BEG=&0A,I11RUN=&0C
#PRAGMA USE(UFI:STRUCT);

    INT *PITEM, *RRESET();       /*υλαϊατεμψ ξα στατψΰ ζακμα*/
    INT PROGLEN;                 /*δμιξα πςοηςαννω χ MISS-σελτοςαθ*/

    STATIC STRUCT UNITRET LOCKRET, WRET;

    STATIC STRUCT MTBLOCK MTBUFF;
    STATIC STRUCT UNITCB
        WR_UCB = \(
            (CHAR*) &MTBUFF,
            SIZEOF(STRUCT MTBLOCK),
            1,0,0             /*δμιξα ϊαπισι - 1 βμολ*/
        \),
        BO_UCB = \(
            MTBUFF.BLBUFF,
            BOOTLEN,
            1,0,0
        \)
    ;

    EXTERN CHAR *AZC;         /*MISS' οβύαρ ϊοξα*/
    STATIC CHAR
        TXBOOT[] = "I\0\0\1\T512 BYTES FOR BOOTSTRAP",
        TXPROG[] = "I\0\0\1\TSTAND-ALONE PROGRAM TO SAVE",
        TXUNIT[] = "\037\N\2\6\BUNIT:X\F*MIUS TAPE GENERATOR*",
        TXCOPY[] = "\R\25 COPIES:X"
    ;
%%  INT AZCOFF;
    CHAR BOOTLAB,PROGLAB;

    INT INPSY;
    INT QCOPIES;
    INT XS;
    INT NTRY;
    INT PROG_ADDR;

    /*
     * οτλςωχαεν χθοδξωε ζακμω
     */
%%  AZCOFF = AZCGEN( 0,0,TXBOOT ); /*δεμαεν AZC σ πςοχεςλοκ ζακμ-πςογ*/
%%  MVS( TXBOOT,SIZEOF(TXBOOT),AZC+AZCOFF );
    MVS( TXBOOT,SIZEOF(TXBOOT),AZC );
    IF( START( LIBASK,0,0X100+LBLIBR ) < 1 ) EXIT(-1);
    BOOTLAB = AZC[1];
%%  AZCOFF = AZCGEN( BOOTLAB,0,TXPROG );      /*νετλα BOOTL ισλμΰώεξα*/
    MVS( TXPROG,SIZEOF(TXPROG),AZC );
    IF( START( LIBASK,0,0X100+LBLIBR ) < 1 ) EXIT(-1);
    PROGLAB = AZC[1];

    /*
     * ϊαθχατωχαεν υστςοκστχο
     */
    DO\(
        DPC( "\037TAPE UNIT:" );
        DO\(
            IF( (INPSY = DPQ('0')) == FINISY ) EXIT(-1);
        \)WHILE( INPSY < '0' \!\! INPSY >= '0'+MAXUNIT ) ;
        LOCKRET = UNIT( 0, ('T'<<8)+(INPSY-'0'), 0X80 );
    \)WHILE( LOCKRET.RETCODE != 0 );
    TXUNIT[10] = INPSY;
    DPC( TXUNIT );

    /* ϊαπςοσ λομιώεστχα λοπικ */
    DPC( "\N NUMBER OF COPIES:" );
    DO\(
        INPSY = DPQ('0');
    \)WHILE( INPSY < '1' \!\! INPSY >= '9' );
    TXCOPY[10] = INPSY;
    DPC( TXCOPY );
    QCOPIES = INPSY-'0';

    /*
     * πιϋεν ϊαηςυϊώιλ (βεϊ ποχτοςοχ -> ϊαηςυϊώιλ δομφεξ βωτψ θοςοϋιν)
     */
    DPC( "\N BOOTSTRAP WRITING" );
    RRESET( BOOTLAB, &PROGLEN );    RBLOCK( BO_UCB.UBUFF, BOOTLEN );
    FOR( XS=QBOOTS; --XS>=0; )\(
        WRET = UNIT( &BO_UCB, LOCKRET.SYSNUM, WRITE );
        IF( WRET.RETCODE < 0 ) EXIT( WRET.RETCODE );
    \)

    /*
     * πιϋεν πςοηςαννυ
     */
    DPC( "\N PROGRAM WRITING" );
    FOR( XS=0; XS<QCOPIES; XS++ )\(
        PITEM = RRESET( PROGLAB, &PROGLEN );
        PROG_ADDR = PITEM[ I11BEG/INTSIZE ];
        FOR( ; PROGLEN > 0; PROGLEN -= INFOLN/SECTOR )\(
            RBLOCK( MTBUFF.BLBUFF, INFOLN );
            MVS( "MISS",4,MTBUFF.BLMISS );
            MTBUFF.BLADDR = SWAB( PROG_ADDR );
            MTBUFF.BLENDMRK = 0;
            NTRY = MAXTRY;
            FOR(;;)\(
                WRET = UNIT( &WR_UCB, LOCKRET.SYSNUM, WRITE );
                IF( WRET.RETCODE >= 0 )BREAK;
                IF( --NTRY<0 ) EXIT( WRET.RETCODE );
                UNIT( &WR_UCB, LOCKRET.SYSNUM, BL_BACK );
            \)
            PROG_ADDR += INFOLN;
        \)
    \)

    MTBUFF.BLADDR = SWAB( PITEM[ I11RUN/INTSIZE ] );
    MTBUFF.BLENDMRK = 1;
    FOR( XS=0; XS<QCOPIES; XS++ )\(
        UNIT( &WR_UCB, LOCKRET.SYSNUM, WRITE );
    \)

    UNIT( &WR_UCB, LOCKRET.SYSNUM, WRITE_TM );  /*WRTM -> ξυφεξ βυζες*/
    UNIT( &WR_UCB, LOCKRET.SYSNUM, WRITE_TM );  /*α ιξαώε EBUFAD*/

  /*UNIT( 0, LOCKRET.SYSNUM, 0X82 );*/
    EXIT(0);
\)

      SUBROUTINE OREGON
C
C ����� : �����=9���04���, user=4.03, sys=2.04
C ����� 900 ������ �� tmp ��� ����������
C
      REAL*8 RNDM
      IMPLICIT INTEGER*2 (A-Z)
      COMMON/ORE1/ B1,D9,P,T,B3,B,RAIDE(30)
C
       INTEGER A,G,    B,     C,     M1,   T,      Z
C          ��K� E�A �ATPOH� O�E��A PA�HOE �EH��� �O�OTO
C
      INTEGER*(2) D9,  M
C     K�.CTPE���� M���
C
      INTEGER*2 X,X1,T1,WG,WP,E,P,S5,D(15),S4,G1,G2,D3
      CHARACTER*6 DT(38)
      INTEGER*2 B1,B3,RAIDE,TIME
      CHARACTER*6 VARD(3)
      CHARACTER*15 VADA(9)
      INTEGER*(2) D1,KORT(18),MDOL(20),MSK(20),TMAS(2)
      CHARACTER*12 WEEK(7)
C
      DATA D/6,11,13,15,17,22,32,35,
     *      37,42,44,54,64,69,95/
      DATA VARD/'�AYH�','A�A��','C��KC�'/
      DATA VADA/'��H�A��YK','�O���O� HO�','KO�AH���Y�OK',
     *'�O�OTO� K��K','KP�BO� HOC','�E�EH��  �EC',
     *'��A� A�MA�','CKA���OPE�','�Y�E�Y�'/
      DATA WEEK/'�OHE�E��H�K','BTOPH�K','CPE�A  ',
     *'�ETBEP�','��TH��A','CY��OTA','BOCKPECEH�E'/
      DATA DT/' 12 A�','PE��',' 26 A�','PE��',' 10 MA','�',' 24 MA','�',
     *' 7 ��H','�',' 21 ��','H�',' 5 ���','�',' 19 ��','��',' 2 AB�',
     *'YCTA',' 16 AB','�YCTA',' 31 AB','�YCTA',' 13 CE','HT��P�',
     *' 27 CE','HT��P�',' 11 OK','T��P�',' 25 OK','T��P�',' 6 HO�','�P�'
     *,' 22 HO','��P�',' 6 �EK','A�P�',' 20 �E','KA�P�'/
      I9=TIME(0)
      I9=I9/5000-I9/3000
      N=RNDM(-1)*I9+1
      ITIKR=1-(-1)**N
C-----------------------------------------------
1     WRITE(6,"(' �Y�ETE ��PAT� (1),HET (0)')")
      Z=0
      READ(5,860)I1
      IF(I1.NE.1)RETURN
      I1=TIME(0)
      WRITE(6,"(' BAM HY�H� �PAB��A ? (1- �A,0- HET)')")
      READ(5,860)N
      IF(N.NE.1)GO TO 701
      WRITE(6,"(//,' �YTE�ECTB�E �O OPE�OHCKO� TPO�E')")
      WRITE(6,"(' �� �TATA M�CCYP� B OPE�OH B 1847 �O�Y')")
      WRITE(6,"(' B� �O��H� �PEO�O�ET� 2040 M���')")
      WRITE(6,"(' �A 5-6 MEC��EB - EC�� OCTAHETEC� B ��B�X',/)")
      WRITE(6,"('Y BAC ECT� 900 �O��APOB, � B� TO��KO �TO')")
      WRITE(6,"(' �A��AT��� �A �YP�OH 200 �O�.')")
      WRITE(6,"(' BAM HY�HO �OTPAT�T� OCTA��H�E �EH���')")
      WRITE(6,"(' HA C�E�Y��E BE��:')")
      WRITE(6,"(' ��K�- B� MO�ETE �OTPAT�T� HA H�X 200-300 �.')")
      WRITE(6,"(' �EM �O���E B� �A��AT�TE,TEM ��CTPEE �Y�ETE EXAT�')")
      WRITE(6,"(' E�A- �EM �O���E Y BAC EE �Y�ET,TEM MEH��E')")
      WRITE(6,"(' BEPO�THOCT� �A�O�ET�')")
      WRITE(6,"(' OPY��E - �A 1 �O�.MO�HO KY��T� 50 �ATPOHOB')")
      WRITE(6,"(' OH� HY�H� ��� �A��T�,OXOT� � T.�.')")
      WRITE(6,"(' O�E��A - OCO�EHHO BA�HO B XO�O�HY� �O�O�Y')")
      WRITE(6,"(' PA�H�E �P��AC�- �EKAPCTBA � �PO��E BE��')")
      WRITE(6,"(' HEO�XO��M� �P� �O�E�H� � HE�PE�B��EHH�X C�Y�A�X')")
      WRITE(6,"( ' B� MO�ETE �OTPAT�T� BCE �EH��� �O HA�A�A ���')")
      WRITE(6,"(' OCTAB�T� B KACCE HA �YTEB�E PACXO�� �P� OCTAHOBKAX')")
      WRITE(6,"('B CE�EH��X')")
      WRITE(6,"(' O�HAKO,TAM BCE CTO�T �OPO�E.MO�HO TAK�E')")
      WRITE(6,"(' E�Y �O��BAT� �YTEM OXOT�-HA�O HA�E�ATAT� ',
     *'KOPOTKOE C�OBO')")
      WRITE(6,"(' �EM ��CTPEE HA�E�ATAETE TEM �Y��E ',
     *'�Y�ET PE�Y��TAT',//)")
      WRITE(6,"(' �E�AEM YC�EXA !!!')")
C-----------------------------------------------
 701  WRITE(6,
     *"(/' HACKO��KO XOPO�O B� CTPE��ETE?',/
     *' (1) OT���HO,(2) XOPO�O,(3) �O�EE MEHEE,')")
      WRITE(6,
     *"(' (4) HE O�EH�,(5) ��OXO'//
     *' �EM �Y��E B� CE�� �APEKOMEH�YETE,TEM CKOPEE')")
      WRITE(6,"(' BAM �P��ETC� �P��E�AT� K OPY���')")
      READ(5,860)D9
      I2=TIME(0)
      I1=(I2-I1)
      DO 180 I=1,I1
 180  I2=RNDM(-1)
C-----------------------------------------------
      IF(D9.NE.7)GO TO 30
      D9=5
      GOTO 811
30    IF(D9.LE.3)GO TO 811
      IF(D9.GT.5)D9=5
      IF(ITIKR.GE.1)GO TO 811
      ITIKR=0
      IF(RNDM(-1).GE.0.8)ITIKR=1
      WRITE(6
     *,"(' A TE�EP� �POBEP�M BA�� BO�MO�HOCT�',/
     *' HA�O C�E�AT� 3 B�CTPE�A - �Y���E �BA �� H�X',/
     *' �Y�YT �A�ETH�M� B BA�E� KBA����KA���')")
      IB3=0
      IKAT=0
      DO 203 I=1,3
      CALL D61
      IF(B1.GT.1)GO TO 200
      IKAT=IKAT+3
      IB3=IB3+B3
      GOTO 203
200   IF(B1.GE.4)GO TO 202
      IKAT=IKAT+2
      IB3=IB3+B3
      GOTO 203
202   IKAT=IKAT+1
      IB3=IB3+B3
203   CONTINUE
      IF(IKAT-5)205,206,207
205   WRITE(6,"(' O�EH� C�A�O')")
      IX=-2+IB3/3
      WRITE(6,"(' B� CTPE���� HA',I5,' KATE�.,A �APEKOMEH�OBA��C� HA',I5,/
     *' TAK�M HA OPE�OHCK�X TPO�AX HE MECTO')")IX,D9
220   WRITE(6,"(' �O�OB�TE C�E�Y��E�O �O�POBO���A',//)")
      GOTO 1
C-----------------------------------------------
206   WRITE(6,"(' �OC�E�H�� �AHC BAM - CTPE���� HE C���HO')")
      IX=IB3/3-1/2
      WRITE(6,"(' BA�A KATE�.',I2,'. CTPE���TE �OC�E�H�� PA�')")IX
      CALL D61
      IF(B1.GE.4)GO TO 208
207   WRITE(6,"(' B� C�PAB���C� C KBA����KA��E�')")
      IX=IB3/3
      IF(D9-IX)211,212,213
211   WRITE(6,
     *"(' O�HAKO B� XBACTYH: BA�A KATE�.',I2,' A CKA�A�� �TO',I2,/
     *' A XBACTYHAM TO�E HE MECTO HA TPO�AX')")IX,D9
      GOTO 220
212   WRITE(6,"(' C�ACT��BO�O �YT�')")
      WRITE(6,216)D9
216   FORMAT(' BA�A KATE�.',I1)
      GOTO 811
213   WRITE(6,"(' B� HACTO���� �Y�Y��� ��OHEP',/
     *' EC�� �OBE�ET, TO �OTOM �OE�EM B K�OH�A�K')")
      WRITE(6,216)D9
      GOTO 811
208   WRITE(6,"(' O��T� C�A�OBATO - H��E�O HE �O�E�AE��',/
     *' �P��ETC� �O�T� B T�P �OTPEH�POBAT�C�')")
      GOTO 220
C-----------------------------------------------
811   X1=-1
      K8=0
      S4=0
      G1=0
      G2=0
      M=0
      M9=0
      D3=0
850   WRITE(6,"(/' CKO��KO B� XOT�TE �A��AT�T� �A ��KOB ?')")
      READ(5,860)A
860   FORMAT(I5)
      IF(A.GE.200)GO TO 901
      WRITE(6,"(' MA�O')")
      GOTO 850
901   IF(A.LE.300)GO TO 9930
      WRITE(6,"(' C���KOM MHO�O')")
      GOTO 850
C
9930  WRITE(6,"(' CKO��KO B� XOT�TE �OTPAT�T� HA E�Y ?')")
      READ(5,860)G
      IF(G.GE.0)GO TO 980
      WRITE(6,"(' �TO HEBO�MO�HO')")
      GOTO 9930
C
980   WRITE(6,"(' CKO��KO B� XOT�TE �OTPAT�T� HA �ATPOH� ?')")
      READ(5,860)B
      IF(B.GE.0)GO TO 1030
      WRITE(6,"(' �TO HEBO�MO�HO')")
      GOTO 980
C
1030  WRITE(6,"(' CKO��KO B� XOT�TE �OTPAT�T� HA O�E��Y ?')")
      READ(5,860)C
      IF(C.GE.0)GO TO 1080
      WRITE(6,"(' �TO HEBO�MO�HO')")
      GOTO 1030
C
1080  WRITE(6,"(' CKO��KO B� XOT�TE �OTPAT�T�',/
     *' HA PA�H�E �P��AC� ?')")
      READ(5,860)M1
      IF(M1.GE.0)GO TO 1130
      WRITE(6,"(' �TO HEBO�MO�HO')")
      GOTO 1080
C
1130  T=700-A-G-B-C-M1
      IF(T.GE.0)GO TO 1170
      WRITE(6,11150)
11150 FORMAT(' Y BAC HE XBAT�T �EHE� - �ABA�TE CHA�A�A')
      GOTO 850
C-----------------------------------------------
1170  B=50*B
      WRITE(6,
     *"(' �OC�E BCEX �ATPAT Y BAC OCTA�OC� ',I6,' �O��APOB')")T
      WRITE(6,"(/,' �OHE�E��H�K, 29 MAPTA 1847 �.')")
      GOTO 1750
C-----------------------------------------------
1230  IF(M.GE.2040)GO TO 5430
      D3=D3+1
      IF(D3.NE.20)GOTO 18401
      WRITE(6,"(' B� �YTE�ECTBOBA�� C���KOM �O��O',/
     *' � �O����� OT ��MH�X XO�O�OB')")
      GOTO 5170
18401 IQ=(D3-1)*2+1
      WRITE(6,
     *"(/'�OHE�E��H�K'/A6,A6/'1847 �.'/)")DT(IQ),DT(IQ+1)
C---  Q=D3+64 -----Q HE �C�O���YETC�
C-----------------------------------------------
1750  IF(G.LT.0)  G=0
      IF(B.LT.0)  B=0
      IF(C.LT.0)  C=0
      IF(M1.LT.0) M1=0
      IF(G.LT.13)
     * WRITE(6,"(' BAM HY�HO �OCTAT� E��, � KAK MO�HO CKOPEE')")
      M2=M
      IF(S4.EQ.1)GO TO 1950
      IF(K8.NE.1)GO TO 1990
1950  T=T-20
      IF(T.LT.0)GO TO 5080
      WRITE(6,"(' ��ATA �OKTOPY 20 �O�.')")
      K8=0
      S4=0
C-----------------------------------------------
1990  IF(M9.EQ.1)GO TO 2020
      WRITE(6,
     *"(' �PO��EHO ',I6,' M���')")M
      GOTO 2040
2020  WRITE(6,"(' �PO��EHO 950 M���')")
      M9=0
2040  WRITE(6,12040) G,B,C,M1,T,Z
12040 FORMAT(/'   E�A �ATPOH� O�E��A PA�HOE  KACCA �O�OTO',/
     *I6,I6,I7,I7,I8,I6)
      IF(X1.EQ.-1)GO TO 2170
      X1=X1*(-1)
2080  WRITE(6,"(' XOT�TE (1)���AHOB�T�C� B CE�EH��',/
     *' (2) �OOXOT�T�C� ��� (3) �PO�O��AT� �YT�?')")
      READ(5,860)X
      IF(X.GT.2)GO TO 2150
      IF(X.GE.1)GO TO 2270
2150  X=3
      GO TO 2270
2170  WRITE(6,"(' XOT�TE (1) �OOXOT�T�C� ��� (2) �PO�O��AT� �YT�?')")
      READ(5,860)X
      IF(X.NE.1)X=2
      X=X+1
      IF(X.GE.3)GO TO 2260
      IF(B.GT.39)GO TO 2260
      WRITE(6,"(' Y BAC C���KOM MA�O �ATPOHOB ��� OXOT�')")
      GOTO 2170
C-----------------------------------------------
2260  X1=-X1
2270  IF(X-2)2290,2540,2720
2290  IZ=RNDM(-1)*10+1
      WRITE(6,
     *"('MO�ETE �PO�AT� �O�OTO ! �A YH��� ',I6,' �O�.')")IZ
      READ(5,860)IZD
      IF(Z.LE.0)GO TO 2390
      IF(IZD.LE.0)GO TO 2301
      IF(IZD.LE.Z)GO TO 2293
      WRITE(6,"(' CTO��KO HE �MEETE - BO��MEM CKO��KO ECT� ')")
      T=T+IZ*Z
      Z=0
      GOTO 2301
2293  WRITE(6,"(' �PO�A��...')")
      Z=Z-IZD
      T=T+IZ*IZD
      GOTO 2301
2390  IF(IZD.LE.0)GO TO 2301
      WRITE(6,"(' C�EPBA HA�O HA�T� �O�OTO...')")
2301  WRITE(6,"(' CKO��KO B� XOT�TE �OTPAT�T�:',/,' HA E�Y?')")
      CALL D24
      G=G+(20*P/30)*P
      WRITE(6,"(' HA �ATPOH�?')")
      CALL D24
      B=B+(2*P*50/3)
      WRITE(6,"(' HA O�E��Y?')")
      CALL D24
      C=C+(2*P/3)
      WRITE(6,"(' HA PA�H�E �P��AC�?')")
      CALL D24
      M1=M1+(2*P/3)
      M=M-55
      GOTO 2720
2540  IF(B.GT.39)GO TO 2570
      WRITE(6,"(' Y BAC C���KOM MA�O �ATPOHOB ��� OXOT�')")
      GOTO 2080
2570  M=M-55
      CALL D61
      IF(B1.LE.1)GO TO 2660
      IF(100*RNDM(-1).LT.13*B1)GO TO 2710
      G=G+48-2*B1
      WRITE(6,"(' HE��OXO� B�CTPE� - �P��THO�O A��ET�TA !')")
      B=B-10-3*B1
      GOTO 2720
2660  WRITE(6,"(' �P�MO ME��Y ��A� - OT���HO !!!',/
     *' KPY�HA� �O���A !')")
      G=G+52+6*RNDM(-1)
      B=B-10-4*RNDM(-1)
      GOTO 2720
2710  WRITE(6,"(' B� �POMAXHY��C� � BA� O�E� Y�E�A�...')")
2720  IF(G.LT.13)GO TO 5060
2750  WRITE(6,
     *"(' KAK B� XOT�TE ��TAT�C�:(1) �KOHOMHO,',/
     *' (2) YMEPEHHO ��� (3) XOPO�O?')")
      READ(5,860)E
      IF(E.GT.3)GO TO 2750
      IF(E.LT.1)GO TO 2750
      G=G-8-5*E
      IF(G.GE.0)GO TO 2860
      G=G+8+5*E
      WRITE(6,
     *"(' CTO�� XOPO�O B� ��TAT�C� HE MO�ETE')")
      GOTO 2750
2860  M=M+200+(A-220)/5.+10*RNDM(-1)
      L1=0
      C1=0
      W1=((M/100.-4)**2+12)
      IF(10.*RNDM(-1).GT.((M/100.-4.)**2+72.)/W1+1)GO TO 3550
2901  WRITE(6,"(' B�EPE�� BCA�H�K�. OH�')")
      S5=0
      IF(RNDM(-1).LT.0.8)GO TO 2950
      WRITE(6,"(' HE')")
      S5=1
2950  WRITE(6,"(' �OXO�� HA BPA�OB')")
      WRITE(6,"(' BA�A TAKT�KA? ')")
2970  WRITE(6,
     *"(' (1) Y�E�AT�,(2) ATAKOBAT�,(3) �PO�O��AT� �YT�,',/
     *' (4) �A���AT�C�?')")
      IF(RNDM(-1).LE.0.2)S5=1-S5
      READ(5,860)T1
      IF(T1.LT.1)GO TO 2970
      IF(T1.GT.4)GO TO 2970
      IF(S5.EQ.1)GO TO 3330
      IF(T1.GT.1)GO TO 3110
      M=M+20
      M1=M1-15
      B=B-150
      A=A-40
      GOTO 3470
3110  IF(T1.GT.2)GO TO 3240
      CALL D61
      B=B-B1*40-80
3140  IF(B1.GT.1)GO TO 3170
      WRITE(6,"(' HE��OXO� B�CTPE� - B� �X �PO�HA��')")
      GOTO 3470
3170  IF(B1.LE.4)GO TO 3220
      WRITE(6,"(' ��OXO CTPE��ETE...� �O�Y���� Y�AP HO�OM...')")
      K8=1
      WRITE(6,"(' BAM HEO�XO��M �OKTOP')")
      GOTO 3470
3220  WRITE(6,"(' HE��OXO CTPE��ETE...')")
      GOTO 3470
3240  IF(T1.GT.3)GO TO 3290
      IF(RNDM(-1).GT.0.8)GO TO 3450
      B=B-160
      M1=M1-15
      GOTO 3470
3290  CALL D61
      B=B-B1*30-80
      M=M-50
      GOTO 3140
3330  IF(T1.GT.1)GO TO 3370
      M=M+15
      A=A-10
      GOTO 3470
3370  IF(T1.GT.2)GO TO 3410
      M=M-15
      B=B-100
      GOTO 3470
3410  IF(T1.LT.3)GO TO 3470
      M=M-40
3450  WRITE(6,"(' OH� HA BAC HE HA�A��')")
      GOTO 3550
3470  IF(S5.EQ.0)GO TO 3501
      WRITE(6,"(' BCA�H�K� ���� M�PH�M�')")
      GOTO 3550
3501  WRITE(6,"(' BCA�H�K� ���� BPA�AM� - �POBEP��TE BA�� �OTEP�')")
      IF(B.GE.0)GO TO 3550
      WRITE(6,"(' Y BAC KOH����C� �ATPOH� - OH� BAC �P�KOH����')")
      GOTO 5170
3550  D1=0
      N=100*RNDM(-1)
3580  D1=D1+1
      IF(D1.GE.16)GO TO 4670
      IF(N.GT.D(D1))GO TO 3580
      GO TO(3660,3701,3740,3790,3820,
     *      3850,3890,3960,4130,4190,
     *      4220,4290,4340,4560,4670)D1
3625  WRITE(6,"('KO�MAP')")
C------------------------------------------ D1=1
3660  WRITE(6,"(' C�OMA�C� �YP�OH - �OTEP�HO BPEM�')")
      M=M-25-5*RNDM(-1)
      M1=M1-8
      GOTO 4710
C------------------------------------------ D1=2
3701  WRITE(6,"(' ��K �OBPE��� HO�Y - YMEH����AC� CKOPOCT�')")
      M=M-35
      A=A-20
      GOTO 4710
C------------------------------------------ D1=3
3740  WRITE(6,"(' B� C�OMA�� PYKY - HY�HO HA�O��T� ��HY')")
      M=M-15-4*RNDM(-1)
      M1=M1-2-3*RNDM(-1)
      GOTO 4710
C------------------------------------------ D1=4
3790  WRITE(6,"(' Y BAC Y�E� ��K.B� �OTEP��� BPEM� HA �O�CK�')")
      M=M-37
      GOTO 4710
C------------------------------------------ D1=5
3820  WRITE(6,"(' B� �OTEP��� �OPO�Y � �O��H� EE �CKA��')")
      M=M-30
      GOTO 4710
C------------------------------------------ D1=6
3850  WRITE(6,"(' B� �OTPAT��� BPEM� HA �O�CK� BO��')")
      M=M-30*RNDM(-1)-2
      GOTO 4710
C------------------------------------------ D1=7
3890  WRITE(6,"(' C���H�� ��BEH� - �OTEP�HO BPEM� � �ACT� �A�ACOB')")
      G=G-10
      B=B-500
      M1=M1-15
      M=M-30*RNDM(-1)-5
      GOTO 4710
C------------------------------------------ D1=8
3960  WRITE(6,"(' HA�A�EH�E �AH��TOB')")
      CALL D61
      B=B-20*B1
      IF(B.GT.0)GO TO 4030
      WRITE(6,"(' Y BAC KOH����C� �ATPOH� - Y BAC �A�PA�� �EH���')")
      T=T/3.
      GOTO 4040
4030  IF(B1.LE.1)GO TO 4101
4040  WRITE(6,"(' BAM �POCTPE���� HO�Y � �A�PA�� O�HO�O ��KA')")
      K8=1
      WRITE(6,"(' �Y��E, EC�� �OKTOP �OCMOTP�T BA�Y PAHY')")
      M1=M1-5
      A=A-20
      GOTO 4710
4101  WRITE(6,"(' B� OT���HO CTPE��ETE � BCEX PA�O�HA��',/
     *' BCE B �OP��KE')")
      GOTO 4710
C------------------------------------------ D1=9
4130  WRITE(6,"(' B BA�EM �YP�OHE ��� �O�AP',
     *' - �PO�A�A �ACT� �MY�ECTBA')")
      G=G-40
      B=B-400
      M1=M1-8*RNDM(-1)-3
      M=M-35
      GOTO 4710
C----------------------------------------- D1=10
4190  WRITE(6,"(' B� �OTEP��� �OPO�Y B TYMAHE - �OTEP�HO BPEM�')")
      M=M-30-5*RNDM(-1)
      GOTO 4710
C----------------------------------------- D1=11
4220  WRITE(6,"(' BAC YKYC��A ��OB�TA� �ME�')")
      B=B-10
      M1=M1-5
      IF(M1.GE.0)GOTO 4710
      WRITE(6,"(' Y BAC HET �EKAPCTB - B� YM�PAETE')")
      GOTO 5170
C----------------------------------------- D1=12
4290  WRITE(6,"(' �YP�OH O�POK�HY�C� �P� �EPE�PABE �EPE� PEKY')")
      G=G-30
      C=C-20
      M=M-40-20*RNDM(-1)
      GOTO 4710
C----------------------------------------- D1=13
4340  WRITE(6,"(' HA BAC HA�A�� ��K�E �BEP�')")
      CALL D61
      IF(B.GT.39)GO TO 4410
      WRITE(6,"(' Y BAC ���O C���KOM MA�O �ATPOHOB',/
     *' OT BAC OCTA��C� PO�K� �A HO�K�')")
      K8=1
      GOTO 5120
4410  IF(B1.GT.2)GO TO 4440
      WRITE(6,"(' XOPO�E� CTPE���O� B� �X PA�O�HA��')")
      GOTO 4450
4440  WRITE(6,"(' OH� BCE-TAK� �O�PA��C� �O BA��X �A�ACOB')")
4450  B=B-20*B1
      C=C-B1*4
      G=G-B1*8
      GOTO 4710
4490  WRITE(6,"(' XO�O�HA� �O�O�A - �P-P-P. Y BAC')")
      IF(C-22.GT.4*RNDM(-1))GO TO 4530
      WRITE(6,"(' HE')")
      C1=1
4530  WRITE(6,"(' �OCTATO�HO TE��A� O�E��A')")
      IF(C1.EQ.0)GO TO 4710
      GOTO 6301
4560  WRITE(6,"(' C���HA� �YP� - �PO�A�A �ACT� �A�ACOB')")
      M=M-25-10*RNDM(-1)
      B=B-200
      M1=M1-4-3*RNDM(-1)
      GOTO 4710
C----------------------------------------- D1=15
4610  IF(E.EQ.1)GO TO 6301
      IF(E.EQ.3)GO TO 4650
      IF(RNDM(-1)-0.25)4710,4710,6301
C----------------------------------------- D1=14
4650  IF(RNDM(-1)-0.5)6301,4710,4710
C----------------------------------------- D1=16
4670  WRITE(6,"(' �H�E��� �OKA�A�� BAM,��E �O��T� E��')")
      G=G+14
4710  IF(RNDM(-1).GT.0.55)GO TO 404
      IP9=RNDM(-1)*11.+1
      GOTO(3960,3625,3740,3820,4220,4340,4490,2901,4670,4130,3850)IP9
C ********* �OP�  *************
4714  IF(M.LE.950)GO TO 1230
      W=9-((M/100.-15.)**2+72.)/((M/100.-15.)**2+12.)
      IF(10*RNDM(-1)-W)4730,4730,4860
4730  WRITE(6,"(' B� HAXO��TEC� B �OPAX')")
      IF(RNDM(-1)-0.1)4750,4750,4780
4750  WRITE(6,"(' B� C����C� C �OPO�� � �OTEP��� BPEM� HA EE �O�CK')")
      M=M-100
      GOTO 4860
4780  IF(RNDM(-1)-0.11)4790,4790,4840
4790  WRITE(6,"(' �YP�OH �OBPE��EH')")
      M1=M1-5
      B=B-200
      M=M-40-30*RNDM(-1)
      GOTO 4860
4840  WRITE(6,"(' YMEH����AC� CKOPOCT� �EPE�B��EH��')")
      M=M-65-(RNDM(-1))/0.02
4860  IF(G1.EQ.1)GO TO 4901
      G1=1
      IF(RNDM(-1)-0.8)4970,4890,4890
4890  WRITE(6,"(' BAM Y�A�HO Y�A�OC� �PEO�O�ET� �OP�')")
4901  IF(M.LT.1700)GO TO 4940
      IF(G2.EQ.1)GO TO 4940
      G2=1
      IF(RNDM(-1).LT.0.7)GO TO 4970
4940  IF(M.GT.950)GO TO 1230
      M9=1
      GOTO 1230
4970  WRITE(6,"(' CHE�HA� �YP� B �OPAX ',
     *' - �OTEP�HO BPEM� � �ACT� �A�ACOB')")
      L1=1
      G=G-25
      M1=M1-10
      B=B-300
      M=M-40-40*RNDM(-1)
      IF(C-18-2*RNDM(-1))6301,4490,4490
C ********** CMEPT� **********
5060  WRITE(6,"(' Y BAC KOH���AC� E�A - B� YMEP�� OT �O�O�A')")
      GOTO 5170
5080  T=0
      WRITE(6,"(' BAM HE�EM �A��AT�T� �OKTOPY')")
      GOTO 5120
C
5110  WRITE(6,"(' Y BAC KOH����C� �EKAPCTBA')")
5120  WRITE(6,"(' B� YMEP�� OT')")
      IF(K8.EQ.1)GO TO 5160
      WRITE(6,"(' BOC�A�EH�� �E�K�X'/)")
      GOTO 5170
5160  WRITE(6,"(' PAH'/)")
5170  WRITE(6,"(/,' BB��Y TAKO� C�TYA���',/
     *' ECT� HECKO��KO HE�O����X �OPMA��HOCTE�')")
      WRITE(6,"(*' XOT�TE �� B� KPAC�B�� �PO�?')")
      READ(5,860)WG
      WRITE(6,"(' XOT�TE �� B� ���H�E �OXOPOH�?')")
      READ(5,860)WP
      WRITE(6,"(' COO���T� �� BA��M PO�CTBEHH�KAM?')")
      READ(5,860)N
      IF(N.GE.1)GO TO 5310
      WRITE(6,"(' OH� �Y�YT �EC�OKO�T�C� O BAC',/)")
      GOTO 5320
5310  WRITE(6,"(' �A TE�E�PAMMY C BAC 4.5 �O�.'/)")
5320  IF(WG.NE.0)GO TO 5324
      WRITE(6,"(' BAC �O�BEP��� KPEMA���',/)")
      GOTO 5325
5324  WRITE(6,"(' BAC �O�O���� B ��HKOB�� �PO�',/)")
5325  IF(WP.NE.0)GO TO 5328
      IF(WG.NE.0)GO TO 5329
      WRITE(6,"(' BA� �E�E� PA�BE��� �O BETPY',/)")
      GOTO 5330
5328  WRITE(6,"(' COCTO���C� ���H�E �OXOPOH�',/)")
      GOTO 5330
5329  WRITE(6,"(' �PO� B�CTAB�EH HA ',
     *' �EHTPA��HO� ��O�A�� B �TATE OPE�OH')")
5330  WRITE(6,"(' �O���OE C�AC��O �A �H�OPMA���')"      )
      WRITE(6,"(' MO�ET ��T� BAM �O���E �OBE�ET B C�E�Y��� PA�',//
     *' C YBA�EH�EM',/' B�ACT� �TATA OPE�OH',/)")
      GOTO 1
C-----------------------------------------------
5430  G9=(2040.-M2)/(M-M2)
      G=G+(1-G9)*(8+5*E)
      WRITE(6,"(/,' B� HAKOHE� �P����� B OPE�OH',/
     *' �PEO�O�EB 2040 M���',//' B� HACTO���� ��OHEP',//)")
      G9=G9*14
      D3=D3*14+G9
      G9=G9+1
      IF(G9.GE.8)G9=G9-7
      WRITE(6,"(/XA12)")WEEK(G9)
      IF(D3.GT.124)GO TO 5740
      D3=D3-93
      WRITE(6,
     *"(3X,I5,' ���� 1847 �.')")D3
      GOTO 5920
5740  IF(D3.GT.155)GO TO 5780
      D3=D3-124
      WRITE(6,
     *"(3X,I5,' AB�YCTA 1847 �.')")D3
      GOTO 5920
5780  IF(D3.GT.185)GO TO 5820
      D3=D3-155
      WRITE(6,
     *"(3X,I5,' CEHT��P� 1847 �.')")D3
      GOTO 5920
5820  IF(D3.GT.216)GO TO 5860
      D3=D3-185
      WRITE(6,
     *"(3X,I5,' OKT��P� 1847 �.')")D3
      GOTO 5920
5860  IF(D3.GT.246)GO TO 5901
      D3=D3-216
      WRITE(6,
     *"(3X,I5,' HO��P� 1847 �.')")D3
      GOTO 5920
5901  D3=D3-246
      WRITE(6,
     *"(3X,I5,' �EKA�P� 1847 �.')")D3
5920  IF(B.LE.0)   B=0
      IF(C.LE.0)   C=0
      IF(M1.LE.0) M1=0
      IF(T.LE.0)   T=0
      IF(G.LE.0)   G=0
      WRITE(6,12040)G,B,C,M1,T,Z
      WRITE(6,"(' �PE���EHT �EPE�AET BAM CBOE �O��PAB�EH�E',/
     *' � �E�AET BAM YC�EXA HA HOBOM MECTE')")
      GOTO 1
C-----------------------------------------------
6301  IF(100*RNDM(-1)-10.LT.35*(E-1))GO TO 6370
      IF(100.*RNDM(-1).LT.100+(40./(4.**(E-1))))GO TO 6410
      WRITE(6,"(' B� CEP�E�HO �A�O�E��',/
     *' HY�HO OCTAHOB�T�C� � �O�E��T�C�')")
      M1=M1-10
      S4=1
      GOTO 6440
6370  WRITE(6,"(' Y BAC �E�KOE HE�OMO�AH�E')")
      M=M-25
      M1=M1-2
      GOTO 6440
6410  WRITE(6,"(' B� ��OXO CE�� �YBCTBYETE')")
      M=M-25
      M1=M1-5
6440  IF(M1.LT.0)GO TO 5110
      IF(L1.EQ.1)GO TO 4940
      GOTO 4710
C ***** �EPEBH� ******
404   IF(RNDM(-1).LE.0.8)GO TO 181
      IV1=RNDM(-1)*3+1
      IV2=VARD(IV1)
      I1=(RNDM(-1)*(27-IV1)+1)/4.
      I2=(RNDM(-1)*(27-IV1)+1)/4.
      I3=(RNDM(-1)*(27-IV1)+1)/4.
      WRITE(6,
     *"(/,' B� BO��� B �A�EP� �H�E��OB ��EMEH� ',A6,/
     *' �Y��TE OCO�EHHO BH�MATE��H�!')")IV2
      WRITE(6,"(' �OXO�E �TO �H�E��� HACTPOEH�')")
      IS5=0
      IF(RNDM(-1).LT.0.6)GO TO 102
      WRITE(6,"(' HE')")
      IS5=1
102   WRITE(6,"(' �PY�E���HO')")
      IV0=RNDM(-1)*9+1
      WRITE(6,
     *"(//,' BAC B��E� �O�P�BETCTBOBAT� BO��� ��EMEH�',/
     *10X,' *** ',A12,' *** ',/)")VADA(IV0)
      IV1=IV1*IV0
      IF(RNDM(-1).LT.0.8)GO TO 110
      WRITE(6,"(' K BAM �O�O�E� �AMAH.',/
     *' XOT�TE �A 10 �O�. Y�HAT� CBOE �Y�Y�EE ?')")
      READ(5,860)I
      IF(I.EQ.1)GO TO 109
      WRITE(6,"(' KAK Y�O�HO...�EPE��C�')")
      GOTO 110
109   T=T-10
      IF(IV1.LE.9)GO TO 113
      IF(IV1-12)115,115,116
113   WRITE(6,"(' B� �O�A�� B M�PHOE ��EM�.�A 10 �ATPOHOB',/
     *' MO�HO B�MEH�T�:')")
      WRITE(6,119)I1,I2,I3
119   FORMAT(' O�E��� �A',I6,' �O�.',/
     *' �EKAPCTB �A ',I6,' �O�.',/
     *' E�� �A ',I6,' �O�.')
      GOTO 110
115   WRITE(6,"(' B� HAXO��TEC� B ��EMEH�, �AKOH��B��M BO�HY.',/
     *' �A 10 �ATPOHOB MO�HO B�MEH�T�:')")
      WRITE(6,119)I1,I2,I3
      GOTO 110
116   WRITE(6,"(' B�...BO�...�...��� B BO�HCTBEHHOE ��EM�.',/
     *' �Y��E HE MEH�T�C� � Y�E�AT�')")
110   WRITE(6,
     *"(' BA�A TAKT�KA:(1) Y�E�AT�,(2) ATAKOBAT�,',/
     *' (3) �A���AT�C�,(4) PA��OBAP�BAT�?')")
      READ(5,860)I
      IF(I-2)122,123,124
124   IF(I-4)125,126,126
C--- Y�E�AT�
122   IF(RNDM(-1).LT.0.8)GO TO 127
      WRITE(6,"(' B� C HEKOTOP�M� �OTEP�M� C�AC�� CBO� CKA���')")
      B=B-20*RNDM(-1)
      GOTO 181
127   WRITE(6,"(' �H�E��� CXBAT��� BAC.�P��ETC� �AT� B�KY�')")
      G=G-20
      B=B-40
      C=C-10
      M1=M1-5
      M=M-25
      GOTO 181
C---  ATAKA
123   CALL D61
      B=B-10*RNDM(-1)
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.GT.1)GO TO 141
      WRITE(6,"(' BO��� Y��T.�AH�KA B ��EMEH�')")
      GOTO 132
141   WRITE(6,"(' �H�EE� HAT���BAET �YK')")
      CALL D61
      B=B-10*RNDM(-1)-2
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.LE.1)GO TO 132
133   WRITE(6,"(' �POMAX...B� PAHEH�...CTPE�A �PO��A HACKBO��')")
      B=B-10*RNDM(-1)
      M1=M1-5
      WRITE(6,"(' ��-�A �EPEB�EB �OKA�A�AC� PYKA C KO��EM')")
      CALL D61
      B=B-10*RNDM(-1)
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.LT.1)GO TO 132
      WRITE(6,"(' HA CBO� BEK �OCTPE���� - �OPA � �OM�PAT�')")
      GOTO 5170
132   WRITE(6,"(' HE��OXO CTPE��E�� - �YT� CBO�O�EH...�E��...')")
      M=M+10
      GOTO 181
C---  �A��TA
125   IF(IS5.NE.1)GO TO 142
      IF(RNDM(-1).GT.0.8)GO TO 145
144   CALL D61
      B=B-15*RNDM(-1)-2
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.LE.1)GO TO 132
      GOTO 133
145   WRITE(6,"(' �H�E��� ���� M�PH�E')")
      WRITE(6,
     *"(' BA�A TAKT�KA:(1) Y�E�AT�,(2) ATAKOBAT�,',/
     *' (3) �A���AT�C�,(4) PA��OBAP�BAT�?')")
      READ(5,860)I
      IF(I-1)178,178,126
142   IF(RNDM(-1)-0.8)145,145,144
C     PA��OBAP�BAT�
126   IF(IS5.EQ.0)GOTO 293
      IF(RNDM(-1)-0.8)290,290,294
290   WRITE(6,"(' �TO-TO �H�E��AM HE XO�ETC� PA��OBAP�BAT� !')")
      GOTO 123
293   IF(RNDM(-1).LE.0.2)GO TO 290
294   WRITE(6,"(' �EM �O���E CYMEETE CKA�AT� �H�E�CK�X C�OB,TEM',/
     *' �Y���� PA�MEH MO�HO �PO��BECT�')")
      K2=0
      K1=0
      DO 150 I=1,5
      CALL D61
      IF(B1.GT.1)GO TO 9152
      K1=K1+1
      WRITE(6,"(' CKA�AHO C�OB=',I6)")K1
      GOTO 150
9152  K2=K2+1
      IF(K2-2)150,152,152
C     IF(K1.GE.4)GO TO 156
152   WRITE(6,"(' C�A�O PA��OBAP�BAE��...�O�Y��C�',/
     *' TPY�HO �Y�ET MEH�T�C�...')")
      GOTO 155
150   CONTINUE
155   IF(K1.LT.4)GO TO 157
156   WRITE(6,"(' XOPO�O �OBOP��� - �H�E��� �PE��A�A�T',/
     *' MEH�T� �O�OTO HA �ATPOH� (10 �ATPOHOB-1 YH���)')")
      WRITE(6,"(' HA CKO��KO MEH�E��?')")
      CALL DT24
      Z=Z+P/10.
157   I1=I1+K1/2.
      I2=I2+K1/2.
      I3=I3+K1/2.
      IP=0
      WRITE(6,
     *"(' CKO��KO �ATPOHOB B� XOT�TE �OMEH�T�?',/
     *' �A 10 �ATP. - E�� HA ',I6,' �O�.')")I1
      CALL DT24
      IF(P.EQ.0)GOTO 173
      IP=1
173   G=G+I1*P/10.
      WRITE(6,
     *"(' �A 10 �ATP. - O�E��� HA ',I6,' �O�.')")I2
      CALL DT24
      IF(P.EQ.0)GOTO 174
      IP=1
174   C=C+I2*P/10.
      WRITE(6,
     *"(' �A 10 �ATP. - PA�HOE HA ',I6,' �O�.')")I3
      CALL DT24
      IF(P.EQ.0)GOTO 175
      IP=1
175   M1=M1+I3*P/10.
      IF(IV1.LE.12)GO TO 176
      IF(IP.NE.1)GOTO 178
179   WRITE(6,"(' B� �X PA�O�����')")
      GOTO 123
176   IF(IP.EQ.0)GOTO 179
178   WRITE(6,"(' B� CKYP��� TPY�KY M�PA,',/
     *' �H�E��� KA�ETC� BAM�')")
      IS6=0
      IF(IS5.NE.1)GO TO 163
      IF(RNDM(-1).GT.0.8)GO TO 166
165   WRITE(6,"(' HE')")
      IS6=1
      GOTO 166
163   IF(RNDM(-1).GT.0.8)GO TO 165
166   WRITE(6,"(' �OBO��H�')")
      IF(IS6.NE.1)GO TO 169
      WRITE(6,171)
171   FORMAT(' �P��ETC� PA��OBAP�BAT� "�PY��M ���KOM"')
      GOTO 123
169   WRITE(6,1691)
1691  FORMAT(' BAC �POBO�AET BCE ��EM�',/
     *' BO��� HA �OPO�Y �O�AP��',/
     *' OC��HHOE YXO HA C�ACT�E',/)
      WRITE(6,"(' �H�E�CKA� �EPEBH� �O�A��...',//)")
181   IF(RNDM(-1)-0.3)499,499,183
183   I=RNDM(-1)*6+1
      GOTO(184,186,188,9212,9220,236)I
184   WRITE(6,"(' �OP�T �PEP��...B� CBEPHY�� C �YT�...')")
      M=M-45
      GOTO 499
186   WRITE(6,"(' ��OB�TA� PEKA...��T� HE����...B� OC�A���.')")
      M=M-30
      M1=M1-10
      GOTO 499
188   WRITE(6,
     *"(' �PO�ACT�...�TO �E�AT�...',/
     *' (1) �P��AT�,(2) B O�XO�,(3) �EPE� Y�K�� MOCT?')")
      READ(5,860)I
      IF(I-2)9201,9202,9203
9201  IF(RNDM(-1).GT.0.5)GO TO 9205
      WRITE(6,"(' BAM Y�A�OC�...HO BCE �MY�ECTBO HE �EPE�POC���')")
      B=B-20
      G=G-5
      C=C-5
      M=M+15
      GOTO 499
9205  WRITE(6,"(' �X...HEY�A�A...HO BCE �E CXBAT���C� �A BETKY',/
     *' ...�OTEP�HO �ACT� �MY�ECTBA')")
      B=B-40
      G=G-10
      C=C-10
      M=M+5
      GOTO 499
9202  WRITE(6,"(' XO� HE CME���,HO HA�E�H�� - �OTEP�HO MHO�O BPEMEH�')")
      M=M-150
      GOTO 499
9203  IF(RNDM(-1).LT.0.6)GO TO 9208
      WRITE(6,"(' C�ACT��BO �PO���...')")
      M=M+15
      GOTO 499
9208  WRITE(6,"(' KA�ETC� MOCT�K HE B��EP�A�',
     *' ...�O���A� �OTEP� �MY�ECTBA')")
      B=B-60
      G=G-10
      C=C-10
      M1=M1-5
      M=M+5
      GOTO 499
9212  WRITE(6,"(' ...PE�KA...HA�O �EPE���T�.')")
      WRITE(6,"(' HA�A�EH�E A����ATOPOB...')")
      CALL D61
      B=B-25*RNDM(-1)
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.GT.1)GO TO 9216
      WRITE(6,"(' ...XOPO�O...�O�A��...A����ATOP� O�E�A�T Y��T�M')")
      M=M+10
      GOTO 499
9216  WRITE(6,"(' �POMAX...A����ATOP KOE-�TO OTKYC��...',////)")
      WRITE(6,"(' �� �A�ACOB KOHE�HO...A B� �TO �O�YMA��?..',//)")
      G=G-5
      C=C-5
      M1=M1-3
      M=M+5
      GOTO 499
9220  WRITE(6,"(' O�EPO ��POKOE - �Y��E �� �EPE���T�')")
      WRITE(6,"(' BA�� �E�CTB��:(1) ���T�,(2) B O�XO�?')")
      READ(5,860)I
      IF(I.GE.2)GO TO 9202
      IF(RNDM(-1).GE.0.6)GO TO 225
      WRITE(6,"(' HEY�A�OC�...TOH�TE...�ACT� �A�ACOB YTOHY�A',/
     *' ...�H�E��� C ��PO�O� B�TA���� BAC')")
      B=B-30
      G=G-5
      C=C-10
      M1=M1-2
      M=M+5
      WRITE(6,"(/,' CKO��KO �ATPOHOB �O�AP�TE �H�E��AM �A C�ACEH�E?')")
      READ(5,860)I
      B=B-I
      I1=25*RNDM(-1)+10
      IF(I.GT.I1)GO TO 233
      WRITE(6,"(' O���AE�� HA�A��H�K...�EPE��C�')")
      CALL D61
      B=B-10*RNDM(-1)-5
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.GT.1)GO TO 833
      WRITE(6,"(' B� �X Y����...')")
      GOTO 499
833   WRITE(6,"(' �POMAX..B� PAHEH�..�H�E��� �A�PA�� �ACT� �A�ACOB')")
      B=B-50
      G=G-6
      C=C-5
      M1=M1-5
      M=M+5
      GOTO 499
233   WRITE(6,"(' OH� �P�H��� BA� �AP')")
      GOTO 499
225   WRITE(6,"(' B� C�ACT��BO �EPE�PAB���C�',
     *' - HO �C�OPT��� �ACT� E��')")
      G=G-5
      M=M+25
      GOTO 499
236   WRITE(6,"(' Y�KA� �OPHA� TPO�A...�ACT� �MY�ECTBA COPBA�AC�...')")
      B=B-25
      G=G-5
      C=C-5
      M1=M1-3
      M=M+5
      IF(RNDM(-1)-0.8)239,239,499
239   WRITE(6,"(' �P��A�T ��YAP�...')")
      CALL D61
      B=B-10
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.GT.1)GO TO 242
      WRITE(6,"(' OT���HO...MO�ETE �O�AP�T� �KYPK� PO�H�M')")
      GOTO 499
242   WRITE(6,"(' ��OXO CTPE��ETE...��K �APE�AH')")
      M=M-100
      B=B-10*RNDM(-1)-2
C ******* �OPO�OK ��O ********
499   IF(RNDM(-1).LE.0.6)GO TO 4714
      WRITE(6,"(' B� BO��� B �OPO�OK � �A��� B PECTOPAH��K',/
     *'          �E�EHHO�O ��O ')")
      WRITE(6,"(' XOT�TE C��PAT�:(1) B KAPT�,(2) B PY�ETKY,',/
     *' (3) �OCTPE��T� B T�PE,(4) B��T� � �PO�O��AT� �YT�?')")
      READ(5,860)I
      IF(I.EQ.0.OR.I.GT.4)I=4
      GO TO (501,502,503,560)I
  501 CALL POKER
      GO TO 560
C..............................................
C        P Y � E T K A
C..............................................
502   WRITE(6,"(' �PAB��A HY�H�? (1-�A,0-HET)')")
      READ(5,860)I
      IF(I.NE.1)GO TO 508
      WRITE(6,5021)
5021  FORMAT(' CTAB�T� X �O�. HA ��C�O C 0-36',/
     *' O - �EPET BEC� �AHK � 36 PA� YBE����BAET CTABKY',/
     *' 1-36  - 36 PA� YBE����BAET B���P��')
508   K=0
911   WRITE(6,"(' CTABKA B �O�.')")
      CALL D24
      IF(P.LT.0)GO TO 508
      IF(K.GE.20)GO TO 912
      K=K+1
514   WRITE(6,"(' HA KAKOE ��C�O?')")
      READ(5,860)ISK
      IF(ISK.GT.36)GO TO 514
      MDOL(K)=P
      MSK(K)=ISK
      WRITE(6,"(' CTABK� �AKOH�EH�? (1-�A,0-HET)')")
      READ(5,860)I
      IF(I.NE.1)GO TO 911
912   I2=RNDM(-1)*37
      WRITE(6,"(' �AHK - B���PA�O ��C�O ',I4/)")I2
      I3=0
      DO 523 I=1,K
      IF(MSK(I).NE.I2)GO TO 523
      I3=MDOL(K)*36
      IF(I2.EQ.0)I3=I3*10
      T=T+I3
      WRITE(6,"(' �O��PAB��� B� COPBA�� �AHK ',I6,' �O�.')")I3
      GOTO 527
523   CONTINUE
      WRITE(6,"(' �A�� HE B���PA�� '/
     *' XOT�TE E�E? (1),HET (0)')")
      READ(5,860)I
      IF(I.EQ.1)GO TO 508
C----------------------------------------------
560   IF(RNDM(-1).GT.0.8)GO TO 535
527   WRITE(6,5271)
5271  FORMAT(' ��O �PE��A�AET BAM �OBECE��T�C�'/
     *' BA�A TAKT�KA:(1) B���T�,(2) �O�T� K �EBY�KAM,',/
     *' (3) B��T� � �PO�O��AT� �YT�?')
      READ(5,860)I
      IF(I-2)931,532,533
931   WRITE(6,"(' �TO �Y�EM ��T�:(1) MAPT�H�-2 �O�.,(2) �P�H��-3 �O�.,',/
     *' (3) B�CK�-5 �O�. ?')")
      READ(5,860)I
      IF(I-2)834,835,836
834   T=T-2
      WRITE(6,"(' C�A�O ��E��!!!')")
      GOTO 838
835   T=T-3
      GOTO 838
836   T=T-5
838   WRITE(6,8381)
8381  FORMAT(' B� �O�T� ���H� ,HO PA�BE HA��E�C�',/
     *' KAK CB�H�� �A TAK�E �EH���!')
      IF(RNDM(-1).GE.0.6)GO TO 527
535   WRITE(6,"(' B� �PO�O��AETE �YT�',/)")
      GOTO 4714
532   WRITE(6,5321)
5321  FORMAT(' KAK�E BAM HPAB�TC�:(1) TO�CT�E,(2) XY��E',/
     *' (3) CPE�H�E?')
      READ(5,860)IST
      WRITE(6,
     *"(' KAK�E BO�OC�:(1) CBET��E,(2) �EPH�E,(3) KOP��HEB�E?')")
      READ(5,860)ISP
      IST1=RNDM(-1)*3+1
      ISP1=RNDM(-1)*3+1
      IM=0
      IF(IST1.EQ.IST) IM=1
      IF(ISP1.EQ.ISP) IM=IM+1
      IF(IM-1)545,544,543
545   T=T-25
      WRITE(6,"(' �TO CTO��O BAM 25 �O�.')")
      GOTO 535
544   WRITE(6,"(' �TO BE�� �EHA ��O...�EPE��C�..')")
556   WRITE(6,"(' XO���H B�H�MAET KO��T..')")
      CALL D61
      B=B-10
      CALL PATRON
      IF(P.NE.0)GOTO 5170
      IF(B1.LE.1)GO TO 548
      WRITE(6,"(' ��OXO CTPE��E��...�O�Y��� 2 CB�H�A B PYKY,',/
     *' ��ATA �OKTOPY 20 �O�.')")
      T=T-20
      GOTO 535
548   WRITE(6,"(' XOPO�O CTPE��E�� - PAH�� XO���HA...� Y�E�AE��')")
      GOTO 535
543   WRITE(6,"(' �TO �E B� �E�AETE? - �TO BE�� TO�HA� KO���',/
     *' BA�E� �EH� - ��AT�T� HE HA�O !',///)")
      GOTO 535
533   IF(RNDM(-1).GT.0.8)GO TO 535
      WRITE(6,"(' B� O���E�� XO���HA...')")
      GOTO 556
503   WRITE(6,"(' B�CTPE� - 1 �O�.,�P�� �A 3 �O�A�AH�� -10 �O�.')")
      I1=0
C
      DO 559 I=1,3
      CALL D61
      B=B-10
      T=T-1
      IF(B1.LE.1)I1=I1+1
559   CONTINUE
      IF(I1.NE.3)GOTO 535
      WRITE(6,"(' B� B��PA�� �P�� !')")
      T=T+10
      GOTO 560
      END
      SUBROUTINE D24
      REAL*8 RNDM
      IMPLICIT INTEGER*2 (A-Z)
      COMMON/ORE1/ B1,D9,P,T,B3,B,RAIDE(30)
2401  READ(5,11860)P
11860 FORMAT(I5)
      IF(P.LE.0)GO TO 2405
      T=T-P
      IF(T.GE.0)GO TO 2405
      WRITE(6,12430)
12430 FORMAT(' Y BAC HE XBATAET �EHE�')
      T=T+P
      P=0
      GOTO 2401
2405  CONTINUE
      RETURN
      END
      SUBROUTINE DT24
      REAL*8 RNDM
      IMPLICIT INTEGER*2 (A-Z)
      COMMON/ORE1/ B1,D9,P,T,B3,B,RAIDE(30)
2401  READ(5,11860)P
11860 FORMAT(I5)
      IF(P.LE.0)GO TO 2405
      B=B-P
      IF(B.GE.0)GO TO 2405
      WRITE(6,12430)
12430 FORMAT(' Y BAC HE XBATAET �ATPOHOB')
      B=B+P
      P=0
      GOTO 2401
2405  CONTINUE
      RETURN
      END
      SUBROUTINE PATRON
      REAL*8 RNDM
      IMPLICIT INTEGER*2 (A-Z)
      COMMON/ORE1/ B1,D9,P,T,B3,B,RAIDE
      P=0
      IF(B.GE.0)GO TO 301
      WRITE(6,302)
302   FORMAT(' Y BAC KOH����C� �ATPOH� - OH� BAC �P�KOH����')
      P=5170
301   CONTINUE
      RETURN
      END
      SUBROUTINE D61
      REAL*8 RNDM
      IMPLICIT INTEGER (A-Z)
      COMMON/ORE1/ B1,D9,P,T,B3,B,RAIDE
      INTEGER*(2) D9,B1,B3,SY(4),CY(4),RAIDE(30)
      DATA RAIDE/'A','�','B','�','�','E','�','�','�','X','�',
     *'�','K','�','M','H','O','�','P','C','T','Y','�',
     *'�','�','�','�','�','�','�'/
6101  WRITE(6,16101)
16101 FORMAT(' �E�ATA�TE')
      DO 16102 I=1,4
      IRAID=INT(30*RNDM(-1)+1)
      SY(I)=RAIDE(IRAID)
16102 CONTINUE
      WRITE(6,16110)(SY(I),I=1,4)
6108  WRITE(6,16108)
16108 FORMAT(/)
      B3=TIME(0)
6110  READ(5,16110)(CY(I),I=1,4)
16110 FORMAT(4A1)
      B1=TIME(0)
      WRITE(6,16108)
      IBK=(B1-B3)/35.
      B1=IBK-(D9-1.)
      B3=IBK
      IF(B1.LT.0)B1=0
      DO 16116 I=1,4
      IF(CY(I).NE.SY(I))B1=9
16116 CONTINUE
      RETURN
      END

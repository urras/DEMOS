
+                  Standalone � Unix (����� ���������)

     -- Makefile:

 LIBSA = libsa

 CFLAGS = -o -DSTANDALONE $[INCLUDES]

 libsa.a: $[LIBSA] saio.h
         cd libsa
         make INCLUDES="$[INCLUDES]" COFLAGS="$[COFLAGS]" ../libsa

 mkfs: libsa.a srt0.o mkfs.c c.o
         cc -c $[CFLAGS] mkfs.c
         ld -o mkfs  srt0.o mkfs.o c.o libsa.a -lc

     -- ���������� � ���������� ��������� (����������������):

 libsa.a         ������ SYS.c �������� ������� read,open etc.,
                 � ����� trap - ���������� ���������.
 saio.h          ����������� ��� ������� ��������� devsw;
                 ���������� iob[] - �.�. ��������� ��������� �����.
                 Saio.h ���������� � c.c, � �� � main.
 c.c,    c.o     ������� devXXX(iob*); ������ ����. ����. devsw[].
 srt0.s, srt0.o  �������. ���������� ������� CSW, video; ������ bss.
 libc.a (-lc)    ����������� ������� ���� strcpy

     -- ������������������ ���������� �������� �/�:

     ��������� ������ open("xx",MODE) ��� read(fd,buff,len) (�� SYS.o).
     �������  open  &  read  ������  �����  ����� �/��� �����-�� ���-��
 ����� � ��������� iob.
     ���������� devopen(*iob) ��� devread(*iob).
 devopen(*iob) �������� xxopen(*iob), � devread ��������
 xxstrategy(*iob, OPCODE), ��� OPCODE - READ,WRITE (SEEK ?).
     Xxopen � xxstrategy - ������� ����������� ��������.

     -- ��� standalone-�����:

     tm(0,5)  ���  rk(0,0)conf-rk  ���  rk(1,0)/src/includes/inode.h
     !  ! !        !  ! ! !
     !  ! !        !  ! ! `------- ��� ����� ��� SYS'find()
     !  ! !        !  ! !
     !  ! `--------!--!-+--------- �.�. ��������; �������� ��� devopen.
     !  !          !  !            �� ����� - ����� ����� ����� TM.
     !  !          !  !
     !  `----------!--+----------- ����� ����������.
     !             !
     `-------------+-------------- ��� ���������� (������� �� devsw).

     -- ��������� �����:

 /* Mitra-225 */
 #if     UCB_NKB == 0
 %- #define CLSIZE  0            /* number of blocks / cluster */
 #define BSIZE   0x100           /* size of secondary block (bytes) */
 #define NINDIR  (BSIZE/sizeof(daddr_t))
 #define BMASK    0xFF           /* BSIZE-1 */
 #define BSHIFT      8           /* LOG2(BSIZE) */
 #define NMASK    0x3F           /* NINDIR-1 */
 #define NSHIFT      6           /* LOG2(NINDIR) */
 #endif
 #define INOPB       4
 #define NADDR      10

     ���� Unix'� ������� ��:
 0-� ���������� ���� - ��������� (� ����� ���� ?)
 1-�            ���� - ��������� (�������������� ����� �����)
  ����� �� s_isize   - ����� � i-������.
  ����� �� s_fsize   - �������� ������������.

     ���  stand-alone  ������  ����� � ����� (i_boff) �������� ��������
 ������� i-����� � ���. ������ �� ������ ���. ����.

     �� ������� ������, ��� ����� ���� �����.
     �������� ������� ����� ����� i-���� 2.



                           -- 11 --


                     ���� ����� 2.15.3

                        ��� �����-32


            ��������� �� ���������� � ���������





������������ ��������������

     ������ ������ ���� ����� 2.15.3 ��� �����-32 ���������-
����  ���  ������ �� ����������� � VAX/11 ��� � ������������
������� �����-32 ��� Unis 4.3  BSD.  �������  �������  �����
2.2 �� ������ �� ������.  � ������� ������� ������������:

1)   ��������� �� ����������  �  ���������  (��������  ����-
     ����);

2)   ����� �� ��������� 1600 bps, ���������� ��� �����:
     - ������������ �� �������;

     - ������� Rubin 2.15.3.

3)   ������������� ����� ����  ����������  ������  ���������
     ��������� ��� ������ � ����� �������� (���������� ����-
     ���  �������),  �  ������������  ������������  ��  ����
     �����.

     � ���� � �������� �  �������  TAR   ��������  ���������
�����������:


./bin
     ����������� ����� ������� RUBIN;

./lib/libq.a
     ���������� ����������� ��� ������ Equel;

./files
     ����������� ����� ��� Rubin.

./data/base/.Keep
     ���� ����  ������  ������  ���  ����������  �����������
     ./data/base,  � ������� ����������� (�� ���������) ����
     ������.

./source/h
     ��������� include-����� ��� �������� �� Equel.

     ���� � ������������� (������ ���� �� �����) ����� ����-
��� ���������� tar � �������� ��������� �����������:









                           -- 22 --


./doc/LP
     ���������������� ��������� � ���� ���-8 ���  ������  ��
     ������� ��������;

./doc/{quel,demos,files,error,other,...)
     ������������������ ���������� ������������;

     � ������������ ������  ���������  ���������  (���������
������ �������� ������, � ������� ��� ��������):


Notes32
     - ��������� �� ������� � �� ���������;

Intro
     - �������� � ������������;

Ref* - ���������� �����������;

Tutorial
     - ��������������� ����������� �� Quel;

Equel
     - ����������� �� ����� Equel;

maintain
     - ����������� �� ������������� �������;

     �������������� ����� ����� ��������:

./lib/{gutil,iutil,ctlmod}
     ������� ����������;

./source/h
     ����� ��������;

./source/iutil/*.[ch]
     �������� ������ ���������� ������� �������;

./source/support/{Makefile,printr.c}
     �������� ������ ��������� "printr", ������� ������ ���-
     ����� � ��������� �������� � ������ �������� � ����.

�������������������� �� �������������� ����������--3322

     ��� ������ "rubin" ������� ������  �����  "�����  ����-
�����"  �  ���� ���������� ����� � �������� ������ (��������
FILES), I-����� (INODES), � � ������� SWAP-�������. �������-
������  ��������  �������� �� 20 ������, 10 I-����� � �� 500
�� �� SWAP-������� �� ������� ������������ ����������� ����-
�������� "rubin" (���� �� ����� ������ 2-� - 3-�).












                           -- 33 --


�������������������� ��������������

     ���������� ������������ �� ��������� ������:

1)   ������� ������������ "rubin" � �������, ��������� ����-
     ��������  ����������  ��;  �  �������� ���������� �����
     ������������� ��������� C-shell;

2)   ����� � ������������ "rubin".

3)   ���� ������� �������� �� �������, ������� ������  ����-
     �������, �� ����������� ����� files/users � �����������
     � ������ data/base/*.


4)   ������� ������� � �����, ������� (��� ����� ����� ����-
     ������ �� /dev/nrmt8):

      tar xvf /dev/nrmt8

     ��� ������� ����������� ���  �������  �����  ��  �����,
     ������ ���  �������� � ���������� ����� ������ �������:

      mt -t /dev/nrmt8 fsf


5)   ����� � �����������������:

      su


6)   ��������� �������:

      make idemos


7)   ����� �� ����������������� �, ���� �������  ��������  �
     ������ ���, �������:


      ./bin/usersetup


     ����� ��������������� ����� .cshrc � .login.   �������-
���  �  ����������  "path"  ~rubin/bin  ���������� ��������.
����  ��  �����������  ��  ��������  "/bin/red"  �  ����  ��
"/bin/csh",  �� �������������� ���� "files/startup", �������
��� /bin/re � /bin/csh �� ��, ��� �� �����������.

     ���  ������ ���� ���������� ����������� ������ �������-
���  rubin_lock.  �� �������������� ��� ���������� ������� �
/etc/rubin_lock, � �������  ���  ���  �������  ����������  �
/etc/rc.  ���  ������  ����������  �����  ���� ��������� ���
�������:









                           -- 44 --


 /etc/rubin_lock

���� ����������� ������� � ���������  �  �������  �  �������
�������� rubin_lock.

���������������� ��������������

     ������ ���� � ��������  ������  �����������  ����������
�������� ���������������� ���� ������ "demodb":

 demodb ���_����

� ��������� ����� ����� �������� �� ���������� ����������.

     �����    ���    Equel    ����������    �    �����������
doc/Examples/*.q.   ����  ���  ����������� ���������, ������
��������� �������:

 make clean
 make

� ������ �������� ���������� ���������.

��������. ����� ��������� ������� ���������  �������  ������
�������      "/usr/include/rubin/*.h"      �      ����������
/usr/lib/libq.a,  �  �  ����  ������  ��������  ������������
(���������� "path") ������ ������� ���������� "~rubin/bin".

������������������ ���� �������������������� RRUUBBIINN 22..1155..33

     ��� ������ ���� �� ��������� ��-1700 ������������� ���-
������:

1)   ������������ ����� "-b" - ���������� ���������� ������-
     ���,  �  "-rheap"  -  ���������  ������� ��������� ����
     "heap" - ������, �����  ���  ��������.   ���  ������  �
     "Equel" ����� �������� � ��������� ������ "rubin".

2)   �����  ���������  �����  ���������  �  ����   ���������
     ������� "sysmod".

3)   ����� ������ ���� ���������  "restore" � "purge -f".

4)   ��������� �������������, ����������� �  �����������  ��
     Equel.

     ���������� ���� ���������� ��������� ������������������
� 2 - 3 ����.















                           -- 55 --


���������������� ��������������  ����  ������������  ������������  ((22..1144  ������  ����--11442200  ��
��..��..))

     - ����� ���� ������: d4 - ���� � t4 - �����, �  �������
       ��� ������ � ����.

     - ����� ������ - ����������  ("Serial")  ���  ���������
       ���������� ������� ��� ��������� ������ � ����;

     - ����� ��� ����������� ������ - SHASH , � ����� �����-
       ���� ����������� ��������� - "tuples" � ������;

     - ����� ����� ���������� ��  ������  ����������  ������
       "FCNTL",   ���  ��  �������  ������������  ����������
       "/dev/lock".



������������������ ������������

1)   ���� ������� ��������� � ������� ���������� ������ ���-
     ������� "Retrieve" � ������ ��������� (�� ���� � �����-
     ������ ����� "cheapsort"), � ����� ��������� ��� ������
     ���� ���������� "Replace", ����� ���������� ������ ���-
     ����. ����� ���������� - �������������� � ����� �������
     ������  "-rheap" ������ ��� ������� ��������� - �� ���-
     �������� (��.  "rubin(DEMOS)" � "modify(QUEL)" �  ����-
     ������ �����������.

�������������� ������������ 22..1155..33 ���� 22..1155..22

2)   ������� ���������� Equel ��� ������ ��  �����  ��������
     ������.

3)   ������� ���������������� ����� �� �������� ����
      ����="�����*" ��� ����="[�-�]*"

     � ������, ���� "����" �������� ������, ������� ��������
     � ISAM ��� CISAM.

4)   ���������� ������ � �������� ISAM ���  �������  (������
     32000  ���������� �������� � 60000 ���� ��������) ����-
     �����.

5)   �������  �����������  ��������  �����  ������,  �������
     ����������  ����������  ���������  ���������� (ksort7).
     ���� ������ ���������� KSORTMEM, �� ��� ����������  ���
     �����  �����������  ������  � ����������, ������� ����-
     �����  ���������  ksort.  ���  ���������  ��  ������  �
     �������, ��������, 16 ��, ���������

      setenv KSORTMEM 6000

     �������� � ��������� ��� ����������  �������  ���������









                           -- 66 --


     (���������  ����������  ������  6-��  �������� ��� ����
     ����������� ������ � ������) �, ��������������,  �����-
     �����  ����� ��������, ������� �������� ������� ������-
     ����� ���������.

6)   ������� ������ ������� ���:
      Xenix-286
      Xenix-386
      Unix 5.3/IX
      Unix V.3/Besta
      Demos/32

      ��������� ������ ��� Demos 2.2/�� � ����� 2.2/��1425.

������������������ �� ����������������������

     ������ � ���� ���������� ������� �������� �������������
�������  � ���������� "DEMOS/*", ������� �.�., ��� ��������-
���  ������ �� alex@kiae.su (���� RelCom, InterNet, Euunet �
��������� � ����).










































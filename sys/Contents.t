
     ����� ���� Unix

 sysent.c       - ������� �������� �������
 main.c         - ��������� �������
 boot.s         - ?
 mch1.s         - ����� ��� ����� �� ��������� �����, ������������,
                  ��������� ������� (sysent), emt, ������ ����,
                  �������� ����������, � ����� ������ �������� ��
                  ��������� ���������� backup.
                : ��� mitra ��� ���� ��������� mch1m
 mch2.s         - ��������� ���� fuword, t.e. �������������� ��������
                  �����������. ������, �������� �������� u(ser) � ovno.
                : ��� mitra ��� ���� ���������
                  fetchstore, save. ���-��� ����� ������� � dummies.
 trap.c         - ������� trap & syscall (����� � ����).
 sys1.c         - ������ ���������� ����������
 sys2.c         - ������ �������� �������, ����� stat.
 sys3.c         - ������ stat, fstat.
 nami.c         - namei(), uchar(), schar();
  namei -> * Convert a pathname into a pointer to a locked inode,
           * with side effects usable in creating and removing files.
           * This is a very central and rather complicated routine.
 prf.c          - printf, panic etc. ���������� putchar �����������
                  ���������.
 machdep.c      - ������������ ����� ������ ���� ������������� �����-
                  ����������� ������� �� "c" (dorti, mapx etc.)
                : ��� mitra ��� ���� ��������� machdep.
 slp.c          - ������������� � ������������.
                  ������: qswtch, swtch.
 sched          - ������ sched � ������� swapper.
                : �� mitra swapper ������� � ��������� ����.

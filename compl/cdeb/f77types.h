#ifdef CDEBUG
/* ���� ftypes */
/* variable types
 * numeric assumptions:
 *      int < reals < complexes
 *      TYDREAL-TYREAL = TYDCOMPLEX-TYCOMPLEX
 */

#define TYUNKNOWN 0
#define TYADDR 1
#define TYSHORT 2
#define TYLONG 3
#define TYREAL 4
#define TYDREAL 5
#define TYCOMPLEX 6
#define TYDCOMPLEX 7
#define TYLOGICAL 8
#define TYCHAR 9
#define TYSUBR 10
#define TYERROR 11
#define NTYPES (TYERROR+1)
#endif
union cdebdesc {
        unsigned cdebword;
        struct {
unsigned cdbtype: 4;    /* ���    */
unsigned cdbfarr: 1;    /* ������ */
unsigned cdbfarg: 1;    /* ���������� �������� */
unsigned cdbleng:10;    /* ����� ���������� - ��� character */
        };
};
#define CDBFORM "~l~%06o\t=%o\n"  /* ������, �������� - ����� ������� */
#define CDBOFFS "~~%s=%o\n"  /* ������ ������� ������: common,�����   */
#define FRMCOMMON 0100000
/* ������ ������ ��� cdebug:
   ��� ��������� - �� ������ common:
   ��� = ��������
   l~<�����> = ���������
   ����� "���" ����� ��������� ������ v.*, ������� ����� ����������.
   ��� ������ common:
   ��� = FRMCOMMON
   ~~����� = �����
   l~<�����> = ���������
*/

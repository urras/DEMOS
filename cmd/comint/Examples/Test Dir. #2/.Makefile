SRC = .test.c
DST = .test
CMP = cc
COMPILE = ${CMP} -g -o ${DST} ${SRC}

start: ${DST}
	-@echo "������ ��������� ...";echo "";echo ""
	-@${DST}

debug: ${DST}
	-@echo "������� ��������� ...";echo "";echo ""
	-@sdb ${DST}

${DST}: ${SRC}
	-@echo "��������� ���������������� !"
	-@until ( echo "����������� ... ";  ${COMPILE} ); do \
		echo ""; echo "�������� ... "; red ${SRC}; (sync;exit 0);\
	  done
	-@echo "                 ... all right !"

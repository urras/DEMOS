#	pubindex csh script
if ($#argv == 0) then
	echo 'Usage: pubindex bibliofile [...]'
	echo '	first argument is the basename for indexes'
	echo '	indexes will be called bibliofile.{ia,ib,ic}'
	exit
endif
/usr/lib/refer/mkey $* | /usr/lib/refer/inv X$1
mv X$1.ia $1.ia
mv X$1.ib $1.ib
mv X$1.ic $1.ic

rubin -s demo << END
\!set -x;grep "^[A-Z]" /etc/perms/* | sed -n "s/[A-Z]..$/&/p" > perms
destroy perms
{remove;perm}\p\g
create perms(fp=c20,PC=c10,perm=c5,dir=c10,type=i1,file=c32,SRC=c5)
\p\g
copy perms(fp="c0:",PC=c0,perm=c0,dir=c0,d=d0,type=c0,file=c0,SRC=c0nl)
from "{pathname}/demo/perms"
\p\g
\!rm perms
modify perms to cisam on pc
\v\g
\!set -x;grep "^[A-Z]" /etc/pevms/* | sed -n "s/[A-Z]..$/&/p" > pevms
destroy pevms
{remove;perm}
\p\g
create pevms(fp=c20,PC=c10,perm=c5,dir=c10,type=i1,file=c32,SRC=c5)
\p\g
copy pevms(fp="c0:",PC=c0,perm=c0,dir=c0,d=d0,type=c0,file=c0,SRC=c0nl)
from "{pathname}/demo/pevms"
\p\g
\!rm pevms
modify pevms to cisam on file\p\g
index on pevms is _pei(pc)\p\g
modify _pei to cisam\p\g
destroy pc\p\g
retrieve pc(pc=perms.pc)\p\g
modify pc to isam
append to pc(pc=pevs.pc)\p\g
printr pc\p\g
destroy ot\p\g
retrieve ot(pc=pc.pc,
perms=count(perms.pc by pc.pc where perms.pc = pc.pc),
pevms=count(pevms.pc by pc.pc where pevms.pc = pc.pc))\p\g
print ot\p\g
END

indir   = 0.
exit    = 1.
fork    = 2.
read    = 3.
write   = 4.
open    = 5.
close   = 6.
wait    = 7.
creat   = 8.
link    = 9.
unlink  = 10.
exec    = 11.
chdir   = 12.
time    = 13.
mknod   = 14.
chmod   = 15.
chown   = 16.
break   = 17.
stat    = 18.
lseek   = 19.
getpid  = 20.
mount   = 21.
umount  = 22.
setuid  = 23.
getuid  = 24.
stime   = 25.
ptrace  = 26.
alarm   = 27.
fstat   = 28.
pause   = 29.
utime   = 30.
smdate  = 30.
stty    = 31.
gtty    = 32.
access  = 33.
nice    = 34.
sleep   = 35.
ftime   = 35.
sync    = 36.
kill    = 37.
csw     = 38.
setpgrp = 39.
dup     = 41.
pipe    = 42.
times   = 43.
profil  = 44.
setblev = 45.
setgid  = 46.
getgid  = 47.
signal  = 48.
rtp     = 49.
setgrp  = 50.
acct    = 51.
phys    = 52.
lock    = 53.
ioctl   = 54.
reboot  = 55.
mpx     = 56.
vfork   = 57.
setinf  = 59.
exece   = 59.
local   = 58.           / local system call indirect
umask   = 60.
getinf  = 60.
chroot  = 61.

			/ local system calls

login   =  1.           / mark login process
lstat   =  2.           / like stat, but don't follow symbolic links
submit  =  3.           / submit
nostk   =  4.           / release stack segment
killbkg =  5.           / kill background processes
killpg  =  6.           / kill process group
renice  =  7.           / change a process's nice
fetchi  =  8.           / fetch from user I space
ucall   =  9.           / call a kernel subroutine
quota   = 10.           / set quota
qfstat  = 11.           / long fstat (for quotas)
qstat   = 12.           / long stat (for quotas)
gldav   = 14.           / get load average
fperr   = 15.           / read floating point error registers
vhangup = 16.           / virtually hang up a control terminal
symlink = 29.           / create symbolic link
readlink        = 30.           / read symbolic link

/       UCB_NET system calls
/       not present if network not installed

select  = 18.           / select active fd
gethost = 19.           / get host name
sethost = 20.           / set host name
socket  = 21.           / get socket fd
connect = 22.           / connect socket
accept  = 23.           / accept socket connection
send    = 24.           / send datagram
receive = 25.           / receive datagram
socketa = 26.           / get socket address
setreuid        = 27.   / set real user id
setregid        = 28.   / set real group id
gethstid        = 31.   / get host id
sethstid        = 32.   / set host id

/       IPK local system calls

truncate        = 33.   / truncate file (pathname)
ftruncate       = 34.   / truncate file (fd)
flock           = 35.   / file lock (fd,cmd)

mkdir           = 37.   / make directory ( path, mode )
rmdir           = 38.   / remove directory ( path )
semsys          = 39.   / IPC semaphore control
msgsys          = 40.   / IPC message control
ustat           = 41.   / FS usage statistics

set history=40
set savehist=40
set nonomatch
set path = ( $path /usr/local/bin ~/bin )
set shell=/bin/sh
set tty=`tty | sed 's/\/dev\///'`
set prompt = 'ru (\!) '
set shistory=20
setenv USER "$home:t"
setenv SHELL /bin/sh
alias   a       alias
a       bak     rm -f \*.b .\*.b  core
a       bk      rm -f \*.b .\*.b  core
a       c       cat
a       e       setenv
a       exit    logout
a       h       history
a       f       jobs -l
a       l       /bin/ls -asFC
a       ls      /bin/ls -CF
a       ll      /bin/ls -al
a       m       more -c
a       md      mkdir

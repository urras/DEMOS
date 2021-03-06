








                     SSeettttiinngg uupp tthhee NNeett


                        Tom Truscott

                      Duke University
        (now at the Research Triangle Institute, NC)
                        January 1980
          (revised 1983 to better reflect reality)





_I_n_s_t_a_l_l_i_n_g _f_i_l_e_s _a_n_d _P_r_o_g_r_a_m_s.

     This paper describes how to get the system named  "xyz"
on  the  uucp net.  If you are on the net, stop reading!  In
chosing your system name, keep it  short  (no  more  than  7
characters  long)  and make it specific.  That is, 'physics'
is a poor uucp name!  The University of California at Berke-
ley  names  their  machines ucbvax, ucbcad, ucbmone, etc.  A
standard Seventh Edition UNIX*  system  is  assumed.   [Yup,
this  is ancient!  I have deleted useless paragraphs -- trt]
The v7 C compiler MUST be used to recompile uucp; the Photo-
typesetter  version  fails.   It is necessary to compile and
install the mail, uucp, and news programs.  In what follows,
"/bin/..." can be replaced by "/usr/bin/...".

_M_A_I_L

     Ignore this section if you machine already has  a  net-
work  mail  program  in  place.   First,  you must tell your
machine   its   name.    Modern   versions   of   UNIX   use
gethostname(II)  or uname(II).  Ancient versions use include
files.    If   you   are   in   the    dark    ages,    edit
/usr/include/whoami.h and change the system name to xyz.  Do
the same to /usr/include/ident.h (not currently used, by the
way).  Then recompile mail:

        cd /usr/src/cmd
        cmake mail

and copy it to /bin.  It should be setuid to root.  The net-
work  uses  a  restricted version of mail, which is actually
mail but invoked with a different name:

        ln /bin/mail /bin/rmail
__________________________
*UNIX is a Trademark of Bell Laboratories.




                     September 11, 1990





                           - 2 -


Mail should now be ready for the net.  A  newer  version  of
mail,  called  Mail,  should  be  used  if  you have it.  It
invokes a mail delivery program called  delivermail  or  the
newer one called sendmail.

_U_U_C_P

     Read the documents on uucp and uucp  implementation  in
the  _U_N_I_X  _P_r_o_g_r_a_m_m_e_r'_s  _M_a_n_u_a_l.  Also read the manual pages
for uucp and uux.

_1.  _G_e_t_t_i_n_g _r_e_a_d_y _t_o _i_n_s_t_a_l_l _u_u_c_p

     Edit /etc/passwd to add the user "uucp" to your system.
Uucp  should  have unique user and group ids (for security).
Duke's uucp and daemon have the same group id, so  they  can
both  access  /dev/dn?  which is not accessible by "others."
Uucp will own the directories and other files  of  the  uucp
system.  Actually, it is nice to also add the user "UUCP" as
a synonym for "uucp", and put "UUCP" earlier in /etc/passwd.
That  way  uucp  will  identified  as UUCP in mail messages,
etc., which  highlights  the  fact  that  the  messages  are
machine generated.  Here are typical lines:

UUCP:iPkI/ZUX3YX2Y:4:19::/usr/spool/uucppublic:/usr/lib/uucp/uucico
uucp:iPkI/ZUX3YX2Y:4:19::/usr/spool/uucppublic:/usr/lib/uucp/uucico


_2.  _I_n_s_t_a_l_l_i_n_g _u_u_c_p _p_r_o_g_r_a_m_s

     To install uucp, read the  INSTALL  file  in  the  uucp
directory  and follow the instructions.  It references other
documentation, which should also be read.

_3.  _F_i_l_e_s _i_n /_u_s_r/_l_i_b/_u_u_c_p

     All of these files should have the uid and gid of uucp,
and  should  not  be  readable (or writable) by others.  The
files "L-devices" and "L-dialcodes" are needed only  if  xyz
calls  other systems.  The file formats are described in the
documentation and are straightforward.   The  USERFILE  file
should have one line for each system which calls xyz:

        Uduke,duke  /usr/spool/uucppublic
        ,xyz /

The first line of the sample  USERFILE  indicates  that  the
duke  system uses login name Uduke and can access the public
directory.  Here is a typical "Uduke" line in /etc/passwd:

Uduke:PSB8EZ5w2tQ4I:4:19::/usr/spool/uucppublic:/usr/lib/uucp/uucico

This line differs from that for uucp only in having  a  dif-
ferent  login  name and password.  The last line of USERFILE



                     September 11, 1990





                           - 3 -


should be ",xyz /", which permits local users but not remote
systems to access anything on xyz.  [Actually, USERFILE is a
real botch.  If you want security, never given  anyone  per-
mission from '/'.]

     The file L.sys has a line for each  system  with  which
xyz  communicates.  If xyz DOES NOT place calls to Duke then
the L.sys line for Duke might be:

        duke None ACU 300 123-4567 ogin Uxyz ssword fooha

This line indicates that  xyz  should  never  ("None")  call
Duke.  If xyz DOES place calls to Duke then the line must be
altered to so indicate.   The  documentation  explains  that
case fairly well.

_4.  _G_e_t_t_i_n_g _i_t _r_u_n_n_i_n_g

     Uucp is now ready for execution.  If Duke  is  to  call
xyz,  the xyz system can request that Duke send mail to test
its operation.  /usr/spool/uucp/LOGFILE, SYSLOG, and  ERRLOG
are  useful  logs  of uucp activity.  If uucp does not work,
and the problems cannot be resolved,  xyz  could  give  Duke
another login on xyz so someone here can poke around and see
what the problem might be.  The login should have  the  same
uid and gid as uucp but without a uucico "shell".

     If xyz can place calls to another system, a  good  test
is  to  try to place the call.  Suppose xyz has both a call-
out and a call-in line.  Then an entry for the mythical sys-
tem  "foo"  can  be put in the L.sys and USERFILE files, and
xyz can call itself as follows:

        echo "test 1" | mail foo!root
        echo "test 2" | mail foo!xyz!root
        echo "test 3" | mail foo!foo!root

Mail sent from system xyz to itself causes a  local  invoca-
tion  of mail.  Mail sent to the mythical system "foo" will,
however, place a call to send the mail.  Uucp will  complain
about  locking,  and  shell metacharacters may not work, but
the communication should still succeed.   Even  if  xyz  has
only  a  call-in or call-out unit it may be possible to fake
the other one by putting a null modem between the two lines.
[Unfortunately,  the  newest  version  of  uucp botches loop
testing fairly badly.  But you  can  observe  it  successful
connect.]

     Also helpful is use of the "-x" debugging option.   The
debugging information produced by this blatant security hole
is truly a wonder to behold.  When  it  is  in  effect,  the
/usr/spool/uucp/AUDIT  file at the remote end will also col-
lect debugging information.




                     September 11, 1990





                           - 4 -


_5.  _C_l_e_a_n_i_n_g _u_p _a_f_t_e_r _u_u_c_p

     Uucp will run okay without any  cleanup;  however,  the
uucp  files  can  become  immense.   Duke runs several shell
files via cron(8).  These are in uuaids/uu.*.  Systems which
call Duke should run the following script occasionally:

        /usr/lib/uucp/uucico -r1 -sduke

Systems which call-out can request uucp to transmit  waiting
files:

        /usr/lib/uucp/uucico -r1

The standard version of cron runs with  uid  "daemon"  which
may  not  be able to run the cleanup or uucico scripts.  The
scripts could be run via at(1),  or  you  could  remove  the
"setuid(1);"   at  the  start  of  cron.   Then  cron  gives
superuser privileges to its children.  As a  precaution,  we
run  a  program  which does a setuid to uucp before invoking
the scripts.

_6.  _O_b_s_c_u_r_e _p_r_o_b_l_e_m_s _t_h_a_t _c_a_n _h_a_p_p_e_n

     When uucp logs in, it is greeted (in standard  V7  sys-
tems)  with the message of the day, "you have mail.", and so
forth.  Sometimes these messages cause uucp  to  fail.   One
cure is for login to suppress the messages when the user has
a non-standard shell (e.g. uucico).

     If you have implemented your own tty handler,  be  sure
it  supports 8 bit RAW input and output.  You should compare
your own handler with that of the standard tty.c,  and  make
sure the RAW mode execution is the same.  [This version does
not yet support 7 bit datapath.]

_U_S_I_N_G _T_H_E _N_E_T

     The simplest and least error-prone use of  the  net  is
that  of sending mail from one system to another.  A user on
xyz can send a letter to Duke as follows:

        echo "xyz lives" | mail duke!root

A letter can be sent to unc as follows:

        echo "xyz lives on" | mail duke!unc!root

Here are some local network enthusiasts: [I only  kept  this
around for historical interest.]

duke!swd          Steve Daniel                C version of news
duke!jte          duke system administrator
duke!trt          Tom Truscott                network enthusiast



                     September 11, 1990





                           - 5 -


duke!unc!smb      Steve Bellovin              network news architecture
duke!phs!dennis   Dennis Rockwell             V6/PWB UNIX enthusiast


     Uucp  provides  a  convenient  way  to  transfer  files
between  systems.  Files may be transferred by mailing them,
but that can get annoying.  Here are some  hints  for  using
uucp.

1.   By convention, the name "~uucp" may be used  to  access
     the public directory on any system.  Thus,
          uucp -m -d x.c duke!~uucp/xyz/
     copies the file x.c to the subdirectory xyz  of  Duke's
     public  directory  (/usr/spool/uucppublic).   The  "-m"
     option requests that mail be sent when x.c is transmit-
     ted.   The  "-d" option requests that the remote system
     create any directories that are needed (in  this  case,
     the  subdirectory  xyz).   The  final "/" in ".../xyz/"
     informs uucp that xyz is a directory and not  an  ordi-
     nary file.

2.   uucp -d -m x.c duke!unc!~uucp/xyz/
     will not work since  uucp  cannot  handle  "duke!unc!".
     Uucp and uux work only between two systems; they do not
     handle a system-pathname sequence as does  mail.   [See
     uusend for a uucp-like multi-hop copy program.]

_N_E_W_S

     You are not running the  distributed  bulletin  board?!
Poor innocent.  Don't start.  You can never go back.


























                     September 11, 1990



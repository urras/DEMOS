#ifndef _ediag_h_
#       define  _ediag_h_

#	ifdef sparc

 /* Use of braces over 'e' causes warnings from pcc when e is a string */
# define ediag(e,r)  e
# define _setediag() /**/

#	else

#       	ifndef ediag

  extern short _ediag;
# define ediag(e,r) (_ediag?(e):(r))
# define _setediag() /* */

# define EDIAG_E        1       /* English */
# define EDIAG_R        0       /* Russian */

#       	endif  ediag

#	endif  sparc

#endif  _ediag_h_

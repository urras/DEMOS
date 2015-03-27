#ifndef _ediag_h_
#       define  _ediag_h_

#       ifndef ediag

  extern short _ediag;
# define ediag(e,r) (_ediag?(e):(r))
# define _setediag() /* */

# define EDIAG_E        1       /* English */
# define EDIAG_R        0       /* Russian */

#       endif  ediag

#endif  _ediag_h_

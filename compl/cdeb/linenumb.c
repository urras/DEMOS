/* ÷÷ïä îïíåòá óôòïëé ğï õëáúáôåìà 'pcmnd'

×ÏÚ×ÒÁÔ: =0 - ÏÛÉÂËÁ, !=0 - ÎÏÍÅÒ ÓÔÒÏËÉ
*/

#include "cdeb.h"

int line_number()
{
    extern char *pcmn;
    register char *p;
    register int line;

    p = pcmn;
    if( *p == '.' ) {
        line = act_line ;  ++pcmn;
    } else {
        line = convi10();
        if( p == pcmn )  line = 0;
    }
    return( line );
}

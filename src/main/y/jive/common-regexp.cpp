
#include "y/jive/common-regexp.hpp"

namespace upsylon {

    namespace Jive
    {

#define Y_JIVE_RX_SIGN    "[-+]?"
#define Y_JIVE_RX_UPART   "[:digit:]+"
#define Y_JIVE_RX_IPART   Y_JIVE_RX_SIGN Y_JIVE_RX_UPART
#define Y_JIVE_RX_FPART   "([.][:digit:]*)?"
#define Y_JIVE_RX_EPART   "(" "e&" Y_JIVE_RX_SIGN Y_JIVE_RX_UPART ")?"

#define Y_JIVE_RX_DBL Y_JIVE_RX_IPART Y_JIVE_RX_FPART Y_JIVE_RX_EPART

        const char * const RegExpFor::Identifier = "[_[:alpha:]][:word:]*";
        const char * const RegExpFor::Unsigned   = Y_JIVE_RX_UPART;
        const char * const RegExpFor::Integer    = Y_JIVE_RX_IPART;
        const char * const RegExpFor::Double     = Y_JIVE_RX_DBL;
        const char * const RegExpFor::Float      = Y_JIVE_RX_DBL "f";


    }

}

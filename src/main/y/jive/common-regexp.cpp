
#include "y/jive/common-regexp.hpp"

namespace upsylon {

    namespace Jive
    {

        const char * const RegExpFor::Identifier = "[_[:alpha:]][:word:]*";
        const char * const RegExpFor::Unsigned   = "[:digit:]+";
        const char * const RegExpFor::Integer    = "[-+]?[:digit:]+";

    }

}

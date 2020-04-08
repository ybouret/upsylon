#include "y/jargon/pattern/regexp.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        const char * const RegularExpression:: Integer  =
        "[-+]?"         // optional sign
        "[:digit:]+";   // content
        
        const char * const RegularExpression:: Real    =
        "[-+]?"                    // optional sign
        "[:digit:]+"               // ip
        "([.][:digit:]*)?"         // fp
        "([eE][-+]?[:digit:]+)?"   // exp
        "f?";                      // optional float
        
        const char * const RegularExpression:: Identifier =
        "[_[:lower:][:upper:]]"
        "[:word:]*"
        ;
    }
    
}

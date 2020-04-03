#include "y/jargon/pattern/regexp.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        const char RegularExpression:: Integer[]  =
        "[-+]?"         // optional sign
        "[:digit:]+";   // content
        
        const char RegularExpression:: Real[]     =
        "[-+]?"                    // optional sign
        "[:digit:]+"               // ip
        "([.][:digit:]*)?"         // fp
        "([eE][-+]?[:digit:]+)?"   // exp
        "f?";                      // optional float
        
        const char RegularExpression:: Identifier[] =
        "[_[:lower:][:upper:]]"
        "[:word:]*"
        ;
    }
    
}

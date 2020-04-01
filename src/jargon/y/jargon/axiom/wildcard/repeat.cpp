
#include "y/jargon/axiom/wildcard/repeat.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Repeat:: ~Repeat() throw()
        {
        }
        
        Repeat:: Repeat(Axiom         &axiom,
                        const size_t   nmin,
                        const uint32_t user,
                        const char     mark) :
        Wildcard( MakeTag(mark,*axiom.label), user, axiom ),
        minimalCount(nmin)
        {
        }
    }
    
}

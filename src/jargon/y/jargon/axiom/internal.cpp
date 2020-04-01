
#include "y/jargon/axiom/internal.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Internal:: ~Internal() throw()
        {
        }
        
        Internal_:: ~Internal_() throw()
        {
        }
        
        bool Internal_:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            (void) xtree;
            (void) lexer;
            (void) source;
            (void) xcache;
            return false;
        }
        
        
    }
    
}

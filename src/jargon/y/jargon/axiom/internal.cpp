
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
        
        ios::ostream & Internal_:: display(ios::ostream &fp) const
        {
            return fp("|_[Internal_<%s>]\n", **label);
        }
    }
    
}

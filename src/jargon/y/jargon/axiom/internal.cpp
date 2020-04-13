
#include "y/jargon/axiom/internal.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Internal:: ~Internal() throw()
        {
        }
        
        Internal_:: ~Internal_() throw()
        {
        }
        
        bool Internal_:: accept(Y_JARGON_AXIOM_ARGS_) const
        {
            (void) xtree;
            (void) lexer;
            (void) source;
            return false;
        }
        
        ios::ostream & Internal_:: display(ios::ostream &fp) const
        {
            return fp("|_[Internal_<%s>]\n", **label);
        }
        
        const char * Internal:: vizual() const throw() { return "shape=oval,style=solid";  }
    }
    
}

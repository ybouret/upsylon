
#include "y/jargon/axiom/inactive.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Inactive:: ~Inactive() throw()
        {
        }

        bool Inactive::accept(Y_JARGON_AXIOM_ARGS) const
        {
            (void) lexer;
            (void) source;
            (void) xtree;
            (void) xcache;
            (void) accepted;
            throw exception("Jargon::Inactive cannot accept!!!");
            //return false;
        }

        
    }
    
}


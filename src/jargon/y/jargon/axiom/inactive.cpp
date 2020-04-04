
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
            throw exception("Jargon::Inactive cannot accept!!!");
        }

        ios::ostream & Inactive:: display(ios::ostream &fp) const
        {
            return fp("|_[Inactive<%s>]\n", **label);
        }

        const char * Inactive:: vizual() const throw()
        {
            return "shape=oval,style=dotted";
        }
    }
    
}


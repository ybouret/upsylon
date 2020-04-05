
#include "y/jargon/axiom/all.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {


        bool Axiom:: isTerminal() const throw()
        {
            return Terminal::UUID == uuid;
        }
        
    }
    
}


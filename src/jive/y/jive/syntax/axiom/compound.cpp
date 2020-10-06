

#include "y/jive/syntax/axiom/compound.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Compound:: ~Compound() throw()
            {
            }
            
            Compound & Compound:: operator<<(const Axiom &axiom)
            {
                push_back( new Axiom::Reference(axiom) );
                return *this;
            }

            
        }
        
    }
    
}


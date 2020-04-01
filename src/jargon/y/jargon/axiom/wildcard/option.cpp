
#include "y/jargon/axiom/wildcard/option.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Option:: ~Option() throw()
        {
        }
                
        Option:: Option( Axiom &axiom ) :
        Wildcard( MakeTag(Mark,*axiom.label), UUID, axiom )
        {
        }
        
        bool Option:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            return false;
        }
        
    }
    
}


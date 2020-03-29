
#include "y/jargon/axiom.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
       
        
        size_t XNode:: serialize(ios::ostream &fp) const
        {
            switch(genre)
            {
                case IsInactive:
                    break;
                    
                case IsTerminal:
                    break;
                    
                case IsInternal:
                    break;
            }
            return 0;
        }
        
        const char * XNode:: className() const throw()
        {
            return "XNode";
        }
        
    }
}
    

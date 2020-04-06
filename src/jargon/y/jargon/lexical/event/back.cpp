

#include "y/jargon/lexical/event/back.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            OnBack:: ~OnBack() throw()
            {
            }
            
            OnBack:: OnBack(const Action &a, const Tag &id) :
            ControlEvent(a,id)
            {
            }
                        
        }
        
        
    }
    
}



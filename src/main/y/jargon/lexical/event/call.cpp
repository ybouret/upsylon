
#include "y/jargon/lexical/event/call.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
        
            OnCall:: ~OnCall() throw()
            {
            }
            
            OnCall:: OnCall(const Action &a, const Tag &tag) :
            ControlEvent(Call,a,tag)
            {
            }
         
            
            
        }
        
        
    }
    
}



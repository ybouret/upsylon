
#include "y/jargon/lexical/event/call.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
        
            OnCall:: ~OnCall() throw()
            {
            }
            
            OnCall:: OnCall(const Action &a, const string &id) :
            ControlEvent(Call,a,id)
            {}
         
            
            
        }
        
        
    }
    
}



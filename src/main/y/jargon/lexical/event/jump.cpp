

#include "y/jargon/lexical/event/jump.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            OnJump:: ~OnJump() throw()
            {
            }
            
            OnJump:: OnJump(const Action &a, const string &id) :
            ControlEvent(Jump,a,id)
            {
            }
            
            
            
        }
        
        
    }
    
}



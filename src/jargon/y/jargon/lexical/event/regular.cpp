#include "y/jargon/lexical/event/regular.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            
            RegularEvent:: ~RegularEvent() throw()
            {
            }
            
            RegularEvent:: RegularEvent(const Type    t,
                                        const Action &a) :
            Event(Regular,a), type(t)
            {
                Y_PATTERN_SELF(RegularEvent);
            }
            
        }
        
    }
    
}

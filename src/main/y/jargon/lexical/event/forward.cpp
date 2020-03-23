
#include "y/jargon/lexical/event/forward.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {

            
            OnForward:: ~OnForward() throw()
            {
            }
            
            OnForward:: OnForward( const Action &a ) :
            RegularEvent(Forward,a)
            {
            }
            
        }
    }
    
}


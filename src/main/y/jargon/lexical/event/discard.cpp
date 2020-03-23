#include "y/jargon/lexical/event/discard.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            
            OnDiscard:: ~OnDiscard() throw()
            {
            }
            
            OnDiscard:: OnDiscard( const Action &a ) :
            RegularEvent(Discard,a)
            {
            }
            
        }
    }
    
}


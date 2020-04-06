
#include "y/jargon/lexical/event.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            

            Event:: ~Event() throw()
            {
            }
            
            Event:: Event(const Kind    k,
                          const Action &a) :
            kind(k),
            action(a),
            self(0)
            {
            }
            
            
        }
        
    }
}

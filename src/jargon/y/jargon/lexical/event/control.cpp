
#include "y/jargon/lexical/event/control.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            ControlEvent:: ~ControlEvent() throw()
            {
            }
            
            
            ControlEvent:: ControlEvent(const Type    t,
                                        const Action &a,
                                        const Tag    &id) :
            Event(Control,a),
            type(t),
            label(id)
            {
                assert(Back!=type);
                Y_PATTERN_SELF(ControlEvent);
            }
            
            ControlEvent:: ControlEvent(const Action &a, const Tag &id) :
            Event(Control,a),
            type(Back),
            label(id)
            {
                Y_PATTERN_SELF(ControlEvent);
            }
            
            
        }
        
    }
    
}


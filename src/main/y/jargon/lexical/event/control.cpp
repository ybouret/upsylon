
#include "y/jargon/lexical/event/control.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            ControlEvent:: ~ControlEvent() throw()
            {
            }
            
            
            ControlEvent:: ControlEvent(const Type   t,
                                        const Action &a,
                                        const string &id) :
            Event(Control,a),
            type(t),
            name(id)
            {
                assert(Back!=type);
            }
            
            ControlEvent:: ControlEvent(const Action &a) :
            Event(Control,a),
            type(Back),
            name()
            {
            }
            
            
        }
        
    }
    
}


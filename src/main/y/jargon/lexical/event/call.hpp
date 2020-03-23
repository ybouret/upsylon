
//! \file

#ifndef Y_JARGON_LEXICAL_EVENT_CONTROL_CALL_INCLUDED
#define Y_JARGON_LEXICAL_EVENT_CONTROL_CALL_INCLUDED 1

#include "y/jargon/lexical/event/control.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
 
            class OnCall : public ControlEvent
            {
            public:
                virtual ~OnCall() throw();
                explicit OnCall(const Action &, const string &);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnCall);
                
            };
            
        }
        
    }
    
}

#endif


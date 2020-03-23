

//! \file

#ifndef Y_JARGON_LEXICAL_EVENT_CONTROL_JUMP_INCLUDED
#define Y_JARGON_LEXICAL_EVENT_CONTROL_JUMP_INCLUDED 1

#include "y/jargon/lexical/event/control.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            class OnJump : public ControlEvent
            {
            public:
                virtual ~OnJump() throw();
                explicit OnJump(const Action &, const string &);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnJump);
                
            };
            
        }
        
    }
    
}

#endif


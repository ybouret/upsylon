

//! \file

#ifndef Y_JARGON_LEXICAL_EVENT_CONTROL_BACK_INCLUDED
#define Y_JARGON_LEXICAL_EVENT_CONTROL_BACK_INCLUDED 1

#include "y/jargon/lexical/event/control.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            class OnBack : public ControlEvent
            {
            public:
                virtual ~OnBack() throw();
                explicit OnBack(const Action &);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnBack);
                
            };
            
        }
        
    }
    
}

#endif


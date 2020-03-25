
//! \file

#ifndef Y_JARGON_LEXICAL_EVENT_CONTROL_CALL_INCLUDED
#define Y_JARGON_LEXICAL_EVENT_CONTROL_CALL_INCLUDED 1

#include "y/jargon/lexical/event/control.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
 
            //! create envent to call a named sub-scanner
            class OnCall : public ControlEvent
            {
            public:
                //! cleanup
                virtual ~OnCall() throw();
                
                //! setup action to take befor call sub-scanner
                explicit OnCall(const Action &, const Tag &);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnCall);
                
            };
            
        }
        
    }
    
}

#endif


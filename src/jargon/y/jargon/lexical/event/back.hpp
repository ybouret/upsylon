

//! \file

#ifndef Y_JARGON_LEXICAL_EVENT_CONTROL_BACK_INCLUDED
#define Y_JARGON_LEXICAL_EVENT_CONTROL_BACK_INCLUDED 1

#include "y/jargon/lexical/event/control.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            //------------------------------------------------------------------
            //
            //! creating an event to come back from a sub-scanner
            //
            //------------------------------------------------------------------
            class OnBack : public ControlEvent
            {
            public:
                //! cleanup
                virtual ~OnBack() throw();
                
                //! setup action to be taken before coming back
                explicit OnBack(const Action &,const Tag &id);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnBack);
                
            };
            
        }
        
    }
    
}

#endif


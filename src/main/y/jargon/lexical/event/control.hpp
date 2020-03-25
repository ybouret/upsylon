//! \file

#ifndef Y_JARGON_LEXICAL_EVENT_CONTROL_INCLUDED
#define Y_JARGON_LEXICAL_EVENT_CONTROL_INCLUDED 1

#include "y/jargon/lexical/event.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
             
            //------------------------------------------------------------------
            //
            //! base class for control events
            /**
             a control event will change a translator's state
             */
            //------------------------------------------------------------------
            class ControlEvent : public Event
            {
            public:
                
                //! category of control event
                enum Type
                {
                    Call, //!< call a sub scanner
                    Back, //!< back from a sub scanner
                    Jump  //!< jump to a new scanner
                };
                const Type      type;    //!< category of control event
                const Tag       label;   //!< name to jump/call to, info for back
                virtual ~ControlEvent() throw();
                                
            protected:
                //! initialize call/jump
                explicit  ControlEvent(const Type,const Action &,const Tag &);
                
                //! initialize back
                explicit  ControlEvent(const Action &a, const Tag &);

                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ControlEvent);
            };
            
            
            
            
        }
        
    }
    
}

#endif




//! \file

#ifndef Y_JARGON_LEXICAL_EVENT_DISCARD_INCLUDED
#define Y_JARGON_LEXICAL_EVENT_DISCARD_INCLUDED 1

#include "y/jargon/lexical/event/regular.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            //------------------------------------------------------------------
            //
            //! forwarding event
            /**
             take action an discard lexeme
             */
            //------------------------------------------------------------------
            class OnDiscard : public RegularEvent
            {
            public:
                explicit OnDiscard(const Action &); //!< setup
                virtual ~OnDiscard() throw();       //!< cleanup
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnDiscard);
            };
            
            
        }
    }
}

#endif


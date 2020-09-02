

//! \file

#ifndef Y_JIVE_LEXICAL_EVENT_FORWARD_INCLUDED
#define Y_JIVE_LEXICAL_EVENT_FORWARD_INCLUDED 1

#include "y/jive/lexical/event/regular.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            //------------------------------------------------------------------
            //
            //! forwarding event
            /**
             take action an produce lexeme
             */
            //------------------------------------------------------------------
            class OnForward : public RegularEvent
            {
            public:
                explicit OnForward(const Action &); //!< setup
                virtual ~OnForward() throw();       //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnForward);
            };


        }
    }
}

#endif


//! \file

#ifndef Y_JIVE_LEXICAL_EVENT_CONTROL_JUMP_INCLUDED
#define Y_JIVE_LEXICAL_EVENT_CONTROL_JUMP_INCLUDED 1

#include "y/jive/lexical/event/control.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            //! create event to jump to another scanner
            class OnJump : public ControlEvent
            {
            public:
                //! ccleanup
                virtual ~OnJump() throw();

                //! setup action to take before jumping to a named sub-scanner
                explicit OnJump(const Action &, const Tag &);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnJump);

            };

        }

    }

}

#endif


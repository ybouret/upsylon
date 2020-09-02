#include "y/jive/lexical/event/jump.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            OnJump:: ~OnJump() throw()
            {
            }

            OnJump:: OnJump(const Action &a, const Tag &id) :
            ControlEvent(Jump,a,id)
            {
            }

        }

    }

}



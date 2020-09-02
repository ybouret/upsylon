

#include "y/jive/lexical/event/call.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            OnCall:: ~OnCall() throw()
            {
            }

            OnCall:: OnCall(const Action &a, const Tag &tag) :
            ControlEvent(Call,a,tag)
            {
            }



        }


    }

}



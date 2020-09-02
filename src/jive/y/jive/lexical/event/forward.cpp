#include "y/jive/lexical/event/forward.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {


            OnForward:: ~OnForward() throw()
            {
            }

            OnForward:: OnForward( const Action &a ) :
            RegularEvent(Forward,a)
            {
            }

        }
    }

}


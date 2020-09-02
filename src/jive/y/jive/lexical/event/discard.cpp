#include "y/jive/lexical/event/discard.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {


            OnDiscard:: ~OnDiscard() throw()
            {
            }

            OnDiscard:: OnDiscard( const Action &a ) :
            RegularEvent(Discard,a)
            {
            }

        }
    }

}


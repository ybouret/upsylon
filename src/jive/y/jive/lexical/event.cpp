

#include "y/jive/lexical/event.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            
            Event:: ~Event() throw()
            {
            }

            Event:: Event(const Kind    k,
                          const Action &a) :
            kind(k),
            action(a)
            {
            }


        }

    }
}

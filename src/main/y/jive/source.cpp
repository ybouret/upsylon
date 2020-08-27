
#include "y/jive/source.hpp"

namespace upsylon {

    namespace Jive
    {
        Source:: ~Source() throw()
        {
        }


        Source:: Source(Module *m) throw() :
        handle(m),
        cache()
        {

        }
    }

}


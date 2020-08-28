
#include "y/jive/pattern/logic/seq.hpp"

namespace upsylon {

    namespace Jive
    {

        Sequential:: ~Sequential() throw()
        {
        }

        Sequential:: Sequential(const uint32_t t) throw() :
        Logical(t)
        {
        }

        Sequential:: Sequential(const Sequential &other) :
        Logical(other)
        {
        }

        
    }

}


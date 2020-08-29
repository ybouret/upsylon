

#include "y/jive/pattern/logic/rev.hpp"

namespace upsylon {

    namespace Jive
    {

        Reworkable:: ~Reworkable() throw()
        {
        }

        Reworkable:: Reworkable(const uint32_t t) throw() :
        Logical(t)
        {
        }

        Reworkable:: Reworkable(const Reworkable &other) :
        Logical(other)
        {
        }

    }

}



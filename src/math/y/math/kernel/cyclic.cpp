
#include "y/math/kernel/cyclic.hpp"

namespace upsylon
{
    namespace math
    {
        cyclic_info:: cyclic_info(const size_t internal, const size_t avail) throw():
        cyclic_offset(internal),
        cyclic_extraneous(avail)
        {

        }

        cyclic_info:: ~cyclic_info() throw()
        {

        }
    }
}


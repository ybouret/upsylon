
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

        size_t cyclic_info:: check_size(const size_t n)
        {
            if(n<3) throw libc::exception(EDOM,"not enough point for cyclic system");
            return n;
        }

    }
}


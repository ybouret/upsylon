#include "y/core/isqrt.hpp"

namespace upsylon {

    namespace mkl
    {
        size_t sqrt_of(const size_t n) throw()
        {
            return isqrt::of<size_t>(n);
        }
    }
    
}


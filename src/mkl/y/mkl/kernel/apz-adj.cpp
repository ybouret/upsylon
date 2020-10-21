#include "y/mkl/kernel/apz-adj.hpp"
#include "y/mkl/simplify.hpp"

namespace upsylon {
    
    namespace mkl
    {
        apz apz_rescale( matrix<apz> &m, apz &d )
        {
            return simplify<apz>::on(m,d);
        }
    }
    
}

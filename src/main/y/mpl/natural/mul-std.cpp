#include "y/mpl/natural.hpp"

namespace upsylon {

    namespace mpl {

        

        natural natural:: lmul(const natural &lhs, const natural &rhs)
        {
            return __lmul(lhs.byte,lhs.bytes,rhs.byte,rhs.bytes);
        }

    }

}


#include "y/type/standard.hpp"

namespace upsylon {

    namespace core
    {
        const standard<int>      :: div_proc standard<int>      ::div_call = div;
        const standard<long>     :: div_proc standard<long>     ::div_call = ldiv;
        const standard<long long>:: div_proc standard<long long>::div_call = lldiv;
    }
}

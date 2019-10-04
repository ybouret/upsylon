
#include "y/type/aliasing.hpp"

namespace upsylon {

    void *       aliasing:: __(void       *p) throw()
    {
        return p;
    }

    const void * aliasing:: __(const void *p) throw()
    {
        return p;
    }

}

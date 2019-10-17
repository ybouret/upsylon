
#include "y/type/aliasing.hpp"

namespace upsylon {

    void * aliasing:: anonymous(void       *p) throw()
    {
        return p;
    }

    void * aliasing:: anonymous(void *p, const ptrdiff_t shift) throw()
    {
        uint8_t *q = static_cast<uint8_t*>(p);
        return q+shift;
    }


    const void * aliasing:: anonymous(const void *p) throw()
    {
        return p;
    }

    const void * aliasing:: anonymous(const void *p, const ptrdiff_t shift) throw()
    {
        const uint8_t *q = static_cast<const uint8_t*>(p);
        return q+shift;
    }


}

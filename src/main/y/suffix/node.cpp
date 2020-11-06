#include "y/suffix/node.hpp"
#include "y/object.hpp"

namespace upsylon
{
    void *suffix:: in_use() throw()
    {
        static const union {
            flag_t f;
            void  *p;
        } alias = { unsigned_int<sizeof(void*)>::maximum };
        return alias.p;
    }
}

#include "y/counting/counting.hpp"
#include "y/memory/allocator/pooled.hpp"

namespace upsylon
{
    memory::allocator & counting:: instance()
    {
        return memory::pooled::instance();
    }

    memory::allocator & counting:: location() throw()
    {
        return memory::pooled::location();
    }
}

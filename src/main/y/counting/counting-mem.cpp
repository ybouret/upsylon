#include "y/counting/counting.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon
{
    memory::allocator & counting:: mem_instance()
    {
        return memory::pooled::instance();
    }

    memory::allocator & counting:: mem_location() throw()
    {
        return memory::pooled::location();
    }
}

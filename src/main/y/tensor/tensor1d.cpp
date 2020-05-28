
#include "y/tensor/tensor1d.hpp"
#include "y/memory/global.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace core
    {
        memory::allocator & tensor:: instance()
        {
            static memory::allocator &mgr = memory::global::instance();
            return mgr;
        }
        
        memory::allocator & tensor:: location() throw()
        {
            static memory::allocator &mgr = memory::global::location();
            return mgr;
        }
        
        
    }
}


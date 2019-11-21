#include "y/graphic/types.hpp"

namespace upsylon {

    namespace Graphic {

        void *Memory:: Acquire( size_t &n )
        {
            static memory::allocator &mgr = memory::global::instance();
            return mgr.acquire(n);
        }

        void  Memory:: Release( void * &p, size_t &n ) throw()
        {
            static memory::allocator &mgr = memory::global::location();
            return mgr.release(p, n);
        }
    }
}


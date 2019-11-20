
#include "y/graphic/metrics.hpp"
#include "y/exception.hpp"


namespace upsylon {

    namespace Graphic {

        void * Metrics:: Acquire(size_t &n)
        {
            static memory::allocator &mgr = memory::global::instance();
            return mgr.acquire(n);
        }

        void  Metrics:: Release(void * &p, size_t &n ) throw()
        {
            static memory::allocator &mgr = memory::global::location();
            mgr.release(p,n);
        }

    }

}





#include "y/tensor/tensor1d.hpp"
#include "y/memory/global.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace core
    {
        void * tensor:: acquireMemory(size_t &n)
        {
            static memory::allocator &mgr = memory::global::instance();

            return mgr.acquire(n);
        }

        void   tensor:: releaseMemory(void *&p, size_t &n) throw()
        {
            static memory::allocator &mgr = memory::global::location();
            mgr.release(p,n);
        }
    }
}


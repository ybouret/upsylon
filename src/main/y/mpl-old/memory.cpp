
#include "y/mpl/memory.hpp"

namespace upsylon {

    Y_SINGLETON_IMPL(mpl::dispatcher);

    namespace mpl {

        dispatcher:: ~dispatcher() throw()
        {
        }

        dispatcher:: dispatcher() throw() :
        singleton<dispatcher>(),
        memory::tight::quarry(),
        memory::tight::quarry_allocator(access,*this)
        {
        }
        
    }

}

#include "y/mpl/natural.hpp"

namespace upsylon {

    namespace mpl {

        uint8_t * natural:: __acquire(size_t &n, size_t &s)
        {
            static dispatcher &mgr = dispatcher::instance();
            return mgr.acquire_bytes(n,s);
        }

        void natural:: __release(uint8_t *&p, size_t &n, size_t &s) throw()
        {
            assert(p);
            assert(dispatcher::exists());
            static dispatcher &mgr = dispatcher::location();
            if(size_t(1)<<s!=n)
            {
                std::cerr << "[s=" << s << ",n=" << n << "]" << std::endl;
            }
            assert( size_t(1)<<s==n);
            mgr.release_bytes(p,n,s);
        }

    }

}





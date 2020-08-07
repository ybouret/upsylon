
#include "y/mpl/memory.hpp"

namespace upsylon {

    namespace mpl {

        dispatcher:: ~dispatcher() throw()
        {
        }

        dispatcher:: dispatcher() throw() :
        singleton<dispatcher>(),
        memory::small::quarry(),
        memory::small::quarry_allocator(access,*this)
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
            mgr.release_bytes(p,n,s);
        }

    }

}


#include "y/memory/vein.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon {
    
    namespace mpl {
        
        namespace
        {
            static uint64_t ___IO[ Y_U64_FOR_ITEM(memory::vein) ];
        }
        
        manager:: manager()
        {
            new ( aliasing::anonymous(___IO) ) memory::vein();
        }
        
        manager:: ~manager() throw()
        {
            self_destruct( *aliasing::as<memory::vein>(___IO) );
            Y_BZSET_STATIC(___IO);
        }
        
        void * manager:: acquire( size_t &n )
        {
            static memory::allocator &IO = *aliasing::as<memory::vein>(___IO);
            Y_LOCK(access);
            return IO.acquire(n);
        }
        
        void   manager:: release(void * &p, size_t &n ) throw()
        {
            static memory::allocator &IO = *aliasing::as<memory::vein>(___IO);
            Y_LOCK(access);
            IO.release(p,n);
        }
        
        uint8_t * manager:: __acquire(size_t &n)
        {
            static memory::allocator &IO = *aliasing::as<memory::vein>(___IO);
            Y_LOCK(access);
            return static_cast<uint8_t*>(IO.acquire(n));
        }
        
        void manager:: __release(uint8_t * &p,size_t &n) throw()
        {
            static memory::allocator &IO = *aliasing::as<memory::vein>(___IO);
            Y_LOCK(access);
            IO.release((void * &)p,n);
        }
    }
    
}



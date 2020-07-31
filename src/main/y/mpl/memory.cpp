
#include "y/mpl/memory.hpp"
#include "y/memory/vein.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"

#include <cstring>

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



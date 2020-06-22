
#include "y/mpl/memory.hpp"
#include "y/memory/vein.hpp"
#include "y/type/self-destruct.hpp"
#include <cstring>

namespace upsylon {
    
    namespace mpl {
        
        namespace
        {
            static uint64_t ___IO[ Y_U64_FOR_ITEM(memory::vein) ];
            static inline
            memory::allocator * __IO() throw()
            {
                return memory::io::__force<memory::vein>(___IO);
            }
        }
        
        manager:: manager()
        {
            new ( __IO() ) memory::vein();
        }
        
        manager:: ~manager() throw()
        {
            self_destruct( *__IO() );
            memset(___IO,0,sizeof(___IO));
        }
        
        void * manager:: acquire( size_t &n )
        {
            static memory::allocator &IO = *__IO();
            Y_LOCK(access);
            return IO.acquire(n);
        }
        
        void   manager:: release(void * &p, size_t &n ) throw()
        {
            static memory::allocator &IO = *__IO();
            Y_LOCK(access);
            IO.release(p,n);
        }
        
        uint8_t * manager:: __acquire(size_t &n)
        {
            static memory::allocator &IO = *__IO();
            Y_LOCK(access);
            return static_cast<uint8_t*>(IO.acquire(n));
        }
        
        void manager:: __release(uint8_t * &p,size_t &n) throw()
        {
            static memory::allocator &IO = *__IO();
            Y_LOCK(access);
            IO.release((void * &)p,n);
        }
    }
    
}



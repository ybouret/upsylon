#include "y/memory/allocator/dyadic.hpp"
#include "y/memory/vein.hpp"
#include "y/memory/io.hpp"
#include "y/type/self-destruct.hpp"

namespace upsylon
{
    namespace memory
    {

        namespace
        {
            static uint64_t ___vein[ Y_U64_FOR_ITEM(vein) ];

            static inline vein * __vein() throw()
            {
                return memory::io::__force<vein>(___vein);
            }
        }


        dyadic:: dyadic() throw()
        {
            new (__vein()) vein();
        }
        
        dyadic:: ~dyadic() throw()
        {
            self_destruct( *__vein() );
        }

        void * dyadic:: acquire(size_t &n)
        {
            static vein & mgr = * __vein();
            Y_LOCK(access);
            return mgr.acquire(n);
        }

        void dyadic:: release(void *&p, size_t &n) throw()
        {
            static vein & mgr = * __vein();
            Y_LOCK(access);
            mgr.release(p,n);
        }

    }
}


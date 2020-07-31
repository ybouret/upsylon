#include "y/memory/allocator/dyadic.hpp"
#include "y/memory/vein.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon
{
    namespace memory
    {

        namespace
        {
            static uint64_t ___vein[ Y_U64_FOR_ITEM(vein) ];
        }


        dyadic:: dyadic() throw()
        {
            Y_BZSET_STATIC(___vein);
            new (aliasing::anonymous(___vein)) vein();
        }
        
        dyadic:: ~dyadic() throw()
        {
            self_destruct( * aliasing::as<vein>(___vein)  );
            Y_BZSET_STATIC(___vein);

        }

        void * dyadic:: acquire(size_t &n)
        {
            static vein & mgr = * aliasing::as<vein>(___vein);
            Y_LOCK(access);
            return mgr.acquire(n);
        }

        void dyadic:: release(void *&p, size_t &n) throw()
        {
            static vein & mgr = *aliasing::as<vein>(___vein);
            Y_LOCK(access);
            mgr.release(p,n);
        }

    }
}


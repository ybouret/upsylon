

#include "y/memory/allocator/dyadic.hpp"
#include "y/object.hpp"

namespace upsylon
{

    Y_SINGLETON_IMPL_WITH(object::life_time-1,memory::dyadic);

    namespace memory
    {

        dyadic:: ~dyadic() throw()
        {
        }

        dyadic:: dyadic() :
        singleton<dyadic>(),
        tight::dyadic_allocator( object::proto() )
        {
        }


        void *dyadic:: acquire(size_t &n)
        {
            Y_LOCK(access);
            return acquire_block(n);
        }

        void dyadic:: release(void * &p, size_t &n) throw()
        {
            Y_LOCK(access);
            release_block(p,n);

        }
    }

}

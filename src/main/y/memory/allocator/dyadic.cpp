

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

    }

}

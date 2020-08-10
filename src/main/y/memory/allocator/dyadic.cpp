

#include "y/memory/allocator/dyadic.hpp"
#include "y/object.hpp"

namespace upsylon {

    namespace memory {

        const at_exit::longevity dyadic::life_time = object::life_time-1;

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

#include "y/memory/allocator/dyadic.hpp"
#include "y/memory/small/object.hpp"

namespace upsylon
{
    namespace memory
    {


        dyadic:: dyadic() : impl(0)
        {
            //small::objects & objs = small::object::proto();
            
        }
        
        dyadic:: ~dyadic() throw()
        {
        }

        void * dyadic:: acquire(size_t &n)
        {

        }

        void dyadic:: release(void *&p, size_t &n) throw()
        {

        }

    }
}


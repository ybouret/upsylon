#include "y/memory/allocator/dyadic.hpp"
#include "y/object.hpp"
#include "y/code/base2.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace memory
    {


        dyadic:: dyadic() : impl(0)
        {
            small::objects & objs =  object::proto();
            impl = &objs;
        }
        
        dyadic:: ~dyadic() throw()
        {
        }

        void * dyadic:: acquire(size_t &n)
        {
            if(n>small::vein::max_size)
            {
                throw libc::exception(EDOM,"memory::dyadic::acquire(overflow %lu)",(unsigned long)n);
            }
        }

        void dyadic:: release(void *&p, size_t &n) throw()
        {
            assert(is_a_power_of_two(n));
            assert(p);

        }

    }
}


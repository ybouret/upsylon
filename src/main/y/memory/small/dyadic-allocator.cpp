
#include "y/memory/small/dyadic-allocator.hpp"
#include "y/memory/small/objects.hpp"
#include "y/code/base2.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon {

    namespace memory {

        namespace small {

            dyadic_allocator:: ~dyadic_allocator() throw()
            {
            }

            dyadic_allocator:: dyadic_allocator(objects &usr_objs) throw() :
            objs( usr_objs )
            {

            }

            static inline size_t po2(const size_t n, size_t &ibit )
            {
                size_t p = 1<<ibit;
                while(p<n)
                {
                    ++ibit;
                    p<<=1;
                }
                return p;
            }

            void *dyadic_allocator:: acquire(size_t &n)
            {
                if(n>vein::max_size) throw libc::exception(EDOM,"dyadic_acquire");
                size_t exp2 = 0;
                size_t size = po2(n,exp2);
                try {
                    Y_LOCK(objs.Access);
                    void *p = objs.dyadic_acquire(exp2);
                    n=size;
                    return p;
                }
                catch(...)
                {
                    n=0;
                    throw;
                }
            }

            void dyadic_allocator:: release(void *&p, size_t &n) throw()
            {
                assert(p);
                assert(n>0);
                assert(n<=vein::max_size);
                assert(is_a_power_of_two(n));
                Y_LOCK(objs.Access);
                objs.dyadic_release(p,integer_log2(n));
                p=NULL;
                n=0;
            }

        }

    }

}



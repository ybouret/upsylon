
#include "y/memory/tight/dyadic-allocator.hpp"
#include "y/memory/tight/objects.hpp"
#include "y/code/base2.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon {

    namespace memory {

        namespace tight {

            dyadic_allocator:: ~dyadic_allocator() throw()
            {
            }

            dyadic_allocator:: dyadic_allocator(objects &usr_objs) throw() :
            objs( usr_objs )
            {

            }

            static inline size_t po2(const size_t n, size_t &ibit )
            {
                static const size_t one = 1;
                size_t p = one<<ibit;
                while(p<n)
                {
                    ++ibit;
                    p<<=1;
                }
                return p;
            }

            void *dyadic_allocator:: acquire(size_t &n)
            {
                if(n>0)
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
                else
                {
                    return 0;
                }
            }

            void dyadic_allocator:: release(void *&p, size_t &n) throw()
            {
                if(p)
                {
                    assert(n>0);
                    assert(n<=vein::max_size);
                    assert(is_a_power_of_two(n));

                    Y_LOCK(objs.Access);
                    objs.dyadic_release(p,integer_log2(n));
                    p=NULL;
                    n=0;
                }
                else
                {
                    assert(n<=0);
                }
            }

        }

    }

}



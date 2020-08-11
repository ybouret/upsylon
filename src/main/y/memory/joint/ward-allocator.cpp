
#include "y/memory/joint/ward-allocator.hpp"

namespace upsylon {

    namespace memory {

        namespace joint {

            ward_allocator:: ~ward_allocator() throw()
            {
            }

            ward_allocator:: ward_allocator( lockable &l, ward &w ) throw() :
            L(l),
            W(w)
            {
                
            }

            void * ward_allocator:: acquire(size_t &n)
            {
                Y_LOCK(L);
                return W.acquire_block(n);
            }

            void ward_allocator:: release(void *&p, size_t &n) throw()
            {
                Y_LOCK(L);
                W.release_block(p,n);
            }
                
            
        }

    }

}


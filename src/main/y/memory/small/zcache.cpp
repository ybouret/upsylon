
#include "y/memory/small/zcache.hpp"
#include "y/type/utils.hpp"
#include "y/memory/allocator/global.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            size_t __zcache:: length_from(const size_t a, const size_t b) throw()
            {
                return next_power_of_two( max_of(a,b) );
            }

            void * __zcache:: acquire(const size_t chunk_size)
            {
                static global &mgr = global::instance();
                assert(is_a_power_of_two(chunk_size));
                return mgr.__calloc(1,chunk_size);
            }

            void   __zcache::release(void *addr, const size_t chunk_size) throw()
            {
                assert(addr!=NULL);
                assert(is_a_power_of_two(chunk_size));

                static global &mgr = global::location();
                mgr.__free(addr,chunk_size);
            }

            void * __zcache:: first(void * addr) throw()
            {
                return static_cast<char *>(addr)+header;
            }
        }

    }

}


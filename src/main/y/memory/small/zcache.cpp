
#include "y/memory/small/zcache.hpp"
#include "y/type/utils.hpp"
#include "y/code/base2.hpp"
#include <iostream>

namespace upsylon {

    namespace memory {

        namespace small {

            size_t __zcache:: chunk_size_from(const size_t a, const size_t b) throw()
            {
                return next_power_of_two( max_of(a,b,vein::min_bytes) );
            }

            void * __zcache:: first(void * addr) throw()
            {
                return static_cast<char *>(addr)+header;
            }

            void __zcache:: missing(const size_t num, const unsigned sz)  throw()
            {
                std::cerr << "[zcache<sizeof=" << sz << ">] missing #zombies=" << num << std::endl;
            }
        }

    }

}


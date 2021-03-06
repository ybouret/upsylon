
#include "y/memory/tight/zcache.hpp"
#include "y/type/utils.hpp"
#include "y/code/base2.hpp"
#include "y/os/run-time-log.hpp"
 

namespace upsylon {

    namespace memory {

        namespace tight {

            size_t __zcache:: chunk_size_from(const size_t a, const size_t b) throw()
            {
                return next_power_of_two( max_of(a,b,vein::min_size) );
            }

            void * __zcache:: first_node_of(void * addr) throw()
            {
                return static_cast<char *>(addr)+header;
            }

            void __zcache:: missing(const size_t num, const unsigned sz)  throw()
            {
                rtl(run_time_warning) << "[tight::zcache<sizeof=" << std::setw(4) << sz << ">] missing #zombies=" << num << std::endl;
            }
            

        }

    }

}


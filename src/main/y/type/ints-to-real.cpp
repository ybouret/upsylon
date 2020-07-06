
#include "y/type/ints-to-real.hpp"
#include "y/os/error.hpp"
#include <cerrno>

namespace upsylon {
    
    namespace kernel {

        void i2r_radix::check()
        {
            if( value!= 2 ) fatal_error("expecting radix=2");
        }
        
    }
}

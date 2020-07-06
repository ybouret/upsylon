
#include "y/type/ints-to-real.hpp"
#include "y/os/error.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon {
    
    namespace kernel {

        void i2r_common::check()
        {
            if( radix!= 2 ) throw libc::exception(EINVAL,"expecting radix=2");
        }

        void i2r_common:: out_of_range()
        {
            throw libc::exception(ERANGE,"integer to real");
        }
        
    }
}

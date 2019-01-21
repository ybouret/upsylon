//! \file
#ifndef Y_CRYPTO_BC_GRAY_INCLUDED
#define Y_CRYPTO_BC_GRAY_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {
    
    namespace crypto {
        
        template <typename T>
        struct gray
        {
            static T encode( T n ) throw();
            static T decode( T n ) throw();
        };
        
    }
    
}

#endif

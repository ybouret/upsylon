//! \file
#ifndef Y_CRYPTO_BC_GRAY_INCLUDED
#define Y_CRYPTO_BC_GRAY_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {
    
    namespace crypto {

        //! using gray codes
        template <typename T>
        struct gray
        {
            static T encode( T n ) throw(); //!< gray forward
            static T decode( T n ) throw(); //!< gray reverse
        };
        
    }
    
}

#endif

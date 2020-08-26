
//! \file

#ifndef Y_AP_PRIME_MILLER_INCLUDED
#define Y_AP_PRIME_MILLER_INCLUDED 1

#include "y/yap/natural.hpp"

namespace upsylon
{

    namespace yap
    {

        //! implementing Miller's test
        struct Miller
        {
            static bool is_prime(const natural     &); //!< check
            static bool is_prime(const number::utype); //!< check wrapper
        };
    }

}

#endif

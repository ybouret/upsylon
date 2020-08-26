
//! \file

#ifndef Y_AP_PRIME_MILLER_INCLUDED
#define Y_AP_PRIME_MILLER_INCLUDED 1

#include "y/yap/natural.hpp"

namespace upsylon
{

    namespace yap
    {

        struct Miller
        {
            static bool is_prime(const natural &);
        };
    }

}

#endif

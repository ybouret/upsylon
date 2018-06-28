#ifndef Y_ALEA_INCLUDED
#define Y_ALEA_INCLUDED 1

#include "y/random/bits.hpp"

namespace upsylon
{
    extern random::bits &alea;
    void alea_init() throw();
}

#endif


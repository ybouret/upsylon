//! \file
#ifndef Y_ALEA_INCLUDED
#define Y_ALEA_INCLUDED 1

#include "y/random/bits.hpp"

namespace upsylon
{
    extern random::bits &alea; //!< internal alea generator
    void alea_init() throw();  //!< init alea to a hardware bases value
}

#endif


//! \file
#ifndef Y_HR_INTS_INCLUDED
#define Y_HR_INTS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    class human_readable
    {
    public:
        const double value; //!< +/-[0:1024]
        const char   radix; //!< b,k,M,G,P,T,E

        human_readable( int64_t i ) throw();
        human_readable( const human_readable & ) throw();
        ~human_readable() throw();
        human_readable & operator=( const human_readable & ) throw();
    };

}

#endif


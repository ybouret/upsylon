//! \file
#ifndef Y_HR_INTS_INCLUDED
#define Y_HR_INTS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    //! human readable integer with multiplier
    class human_readable
    {
    public:
        const double value; //!< +/-[0:1024]
        const char   radix; //!< b,k,M,G,P,T,E

        human_readable( int64_t i ) throw();                           //!< setup
        human_readable( const human_readable & ) throw();              //!< copy
        ~human_readable() throw();                                     //!< cleanup
        human_readable & operator=( const human_readable & ) throw();  //!< assign
    };

}

#endif


//! \file
#ifndef Y_HR_INTS_INCLUDED
#define Y_HR_INTS_INCLUDED 1

#include "y/os/platform.hpp"
#include <iosfwd>
namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! human readable integer with multiplier
    //
    //__________________________________________________________________________
    class human_readable
    {
    public:
        const double value; //!< +/-[0:1024]
        const char   radix; //!< b,k,M,G,P,T,E

        human_readable( int64_t i ) throw();                                       //!< setup
        human_readable( const human_readable & ) throw();                          //!< copy
        ~human_readable() throw();                                                 //!< cleanup
        human_readable & operator=( const human_readable & ) throw();              //!< assign
        friend std::ostream & operator<<( std::ostream &, const human_readable &); //!< display
    };

}

#endif


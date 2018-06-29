//! \file
#ifndef Y_PRIMALITY_INCLUDED
#define Y_PRIMALITY_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    //! simple primality tests
    struct primality
    {
        static bool   _check( const size_t n ) throw(); //!< Eratosthene
        static bool   check( const size_t n ) throw();  //!< optimized

        static size_t prev(const size_t n); //!< next prime. \todo check overflow
        static size_t next(const size_t n); //!< prev prime
    };

}

#endif

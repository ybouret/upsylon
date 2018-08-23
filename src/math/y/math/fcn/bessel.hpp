//! \file
#ifndef Y_MATH_FCN_BESSEL_INCLUDED
#define Y_MATH_FCN_BESSEL_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    
    namespace math
    {
        //! Bessel functions
        template <typename T>
        struct bessel
        {
            static T j0(T x); //!< any x
            static T y0(T x); //!< x > 0
            static T i0(T x); //!< any x
            static T k0(T x); //!< x>0
        };
        
    }
    
}

#endif

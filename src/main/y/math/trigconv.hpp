//! \file
#ifndef Y_MATH_TRIGCONV_INCLUDED
#define Y_MATH_TRIGCONV_INCLUDED

#include "y/math/types.hpp"

namespace upsylon
{

    namespace math
    {

        template <typename T>
        inline T rad2deg( const T angle_rad ) throw()
        {
            static const T full_rad = numeric<T>::pi;
            static const T full_deg = 180;
            return (full_deg*angle_rad)/full_rad;
        }

        template <typename T>
        inline T deg2rad( const T angle_deg ) throw()
        {
            static const T full_rad = numeric<T>::pi;
            static const T full_deg = 180;
            return (full_rad*angle_deg)/full_deg;
        }

    }
}

#endif


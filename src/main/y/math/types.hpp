//! \file
#ifndef Y_MATH_TYPES_INCLUDED
#define Y_MATH_TYPES_INCLUDED 1

#include "y/type/complex.hpp"
#include <cmath>

namespace upsylon
{
    namespace math
    {
        template <typename T>
        struct real_for
        {
            typedef T type;
        };

        template <>
        struct real_for< complex<float> >
        {
            typedef float type;
        };

        template <>
        struct real_for< complex<double> >
        {
            typedef double type;
        };


        template <typename T>
        struct numeric
        {
            static const T      minimum;  //!< minimum representable value
            static const T      maximum;  //!< maximum representable value
            static const T      epsilon;  //!< $1+\epsilon\simeq1$
            static const T      pi;       //!< $\pi$
            static const T      two_pi;   //!< $2\pi$
            static const T      half_pi;  //!< $\pi/2$
            static const T      min_exp;  //!< minimal exponent
            static const T      max_exp;  //!< maximal exponent
            static const size_t dig;      //!< significant digits
            static const T      min_10_exp; //!< minimal 10 exponent
            static const T      max_10_exp; //!< maximal 10 exponent
            
        };
    }
}

#endif


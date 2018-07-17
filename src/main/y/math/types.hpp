//! \file
#ifndef Y_MATH_TYPES_INCLUDED
#define Y_MATH_TYPES_INCLUDED 1

#include "y/type/complex.hpp"
#include "y/type/utils.hpp"
#include <cmath>

namespace upsylon
{
    namespace math
    {
        //! corresponding real type
        template <typename T>
        struct real_for
        {
            typedef T type;
        };

        //! specialized for complex<float>
        template <>
        struct real_for< complex<float> >
        {
            typedef float type;
        };

        //! specialized for complex<double>
        template <>
        struct real_for< complex<double> >
        {
            typedef double type;
        };

        //! numeric definitions
        template <typename T>
        struct numeric
        {
            static const T      minimum;    //!< minimum representable value
            static const T      maximum;    //!< maximum representable value
            static const T      epsilon;    //!< $1+\epsilon\simeq1$
            static const T      pi;         //!< $\pi$
            static const T      two_pi;     //!< $2\pi$
            static const T      half_pi;    //!< $\pi/2$
            static const T      min_exp;    //!< minimal exponent
            static const T      max_exp;    //!< maximal exponent
            static const size_t dig;        //!< significant digits
            static const T      ftol;       //!< $10^{-dig}$
            static const T      sqrt_ftol;  //!< $10^{-dig/2-1}$
            static const T      min_10_exp; //!< minimal 10 exponent
            static const T      max_10_exp; //!< maximal 10 exponent
            static const T      tiny;       //!< $10^{min_10_exp}$
            static const T      huge;       //!< $10^{max_10_exp}$
            static T __sqrt(const T) throw();
        };

        inline float  __sqrt( const float  f ) throw() { return sqrtf(f); }
        inline double __sqrt( const double f ) throw() { return sqrt(f);  }

        inline float  __fabs( const float  f ) throw() { return fabsf(f); }
        inline double __fabs( const double f ) throw() { return fabs(f);  }
        inline unit_t __fabs( const unit_t f ) throw() { return (f<0) ? -f : f; }
        inline float  __fabs( const complex<float>  c) throw() { return max_of(fabsf(c.re),fabsf(c.im)); }
        inline double __fabs( const complex<double> c) throw() { return max_of(fabs(c.re),fabs(c.im)); }

        inline float  __pow( const float  x, const float  p) throw() { return powf(x,p); }
        inline double __pow( const double x, const double p) throw() { return pow(x,p); }

    }
}

#if !defined(Y_MATH_IN_TYPES_CXX)

#define Y_MATH_DECL_EXTERN 1

#if defined(Y_MATH_DECL_EXTERN) && (1==Y_MATH_DECL_EXTERN)

#define Y_MATH_EXTERN__(TYPE,NAME) \
extern template const TYPE upsylon::math::numeric<TYPE>::NAME

#define Y_MATH_EXTERN(NAME)   \
Y_MATH_EXTERN__(float,NAME);  \
Y_MATH_EXTERN__(double,NAME)

Y_MATH_EXTERN(minimum);
Y_MATH_EXTERN(maximum);
Y_MATH_EXTERN(epsilon);
Y_MATH_EXTERN(pi);
Y_MATH_EXTERN(two_pi);
Y_MATH_EXTERN(half_pi);
Y_MATH_EXTERN(min_exp);
Y_MATH_EXTERN(max_exp);
Y_MATH_EXTERN(dig);
Y_MATH_EXTERN(min_10_exp);
Y_MATH_EXTERN(max_10_exp);
Y_MATH_EXTERN(ftol);
Y_MATH_EXTERN(sqrt_ftol);
Y_MATH_EXTERN(tiny);
Y_MATH_EXTERN(huge);

#endif

#endif


#endif


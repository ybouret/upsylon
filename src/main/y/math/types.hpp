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
            typedef T type; //!< type itself
        };

        //! specialized for complex<float>
        template <>
        struct real_for< complex<float> >
        {
            typedef float type; //!< float
        };

        //! specialized for complex<double>
        template <>
        struct real_for< complex<double> >
        {
            typedef double type; //!< double
        };

        //! numeric definitions
        template <typename T>
        struct numeric
        {
            static const T      minimum;    //!< minimum representable value
            static const T      maximum;    //!< maximum representable value
            static const T      epsilon;    //!< \f$1+\epsilon\simeq1\f$
            static const T      pi;         //!< \f$\pi\f$
            static const T      two_pi;     //!< \f$2\pi\f$
            static const T      half_pi;    //!< \f$\pi/2\f$
            static const T      min_exp;    //!< minimal exponent
            static const T      max_exp;    //!< maximal exponent
            static const size_t dig;        //!< significant digits
            static const T      ftol;       //!< \f$10^{-dig}\f$
            static const T      sqrt_ftol;  //!< \f$10^{-dig/2-1}\f$
            static const T      min_10_exp; //!< minimal 10 exponent
            static const T      max_10_exp; //!< maximal 10 exponent
            static const T      tiny;       //!< \f$10^{min_{10}exp}\f$
            static const T      huge;       //!< \f$10^{max_{10}exp}\f$
        };

        inline float  __sqrt( const float  f ) throw() { return sqrtf(f); } //!< sqrt
        inline double __sqrt( const double f ) throw() { return sqrt(f);  } //!< sqrt

        inline float  __fabs( const float  f ) throw() { return fabsf(f); }       //!< fabs
        inline double __fabs( const double f ) throw() { return fabs(f);  }       //!< fabs
        inline unit_t __fabs( const unit_t f ) throw() { return (f<0) ? -f : f; } //!< fabs
        inline float  __fabs( const complex<float>  c) throw() { return max_of(fabsf(c.re),fabsf(c.im)); } //!< fabs
        inline double __fabs( const complex<double> c) throw() { return max_of(fabs(c.re),fabs(c.im));   } //!< fabs

        inline float  __pow( const float  x, const float  p) throw() { return powf(x,p); } //!< pow
        inline double __pow( const double x, const double p) throw() { return pow(x,p); }  //!< pow

    }
}

#if !defined(Y_MATH_IN_TYPES_CXX)

//! default use of extern template
#define Y_MATH_DECL_EXTERN 1

#if defined(Y_MATH_DECL_EXTERN) && (1==Y_MATH_DECL_EXTERN)

//! prototype for a class
#define Y_MATH_EXTERN__(TYPE,NAME) \
extern template const TYPE upsylon::math::numeric<TYPE>::NAME

//! prototypes for float and double
#define Y_MATH_EXTERN(NAME)   \
Y_MATH_EXTERN__(float,NAME);  \
Y_MATH_EXTERN__(double,NAME)

Y_MATH_EXTERN(minimum);    //!< extern
Y_MATH_EXTERN(maximum);    //!< extern
Y_MATH_EXTERN(epsilon);    //!< extern
Y_MATH_EXTERN(pi);         //!< extern
Y_MATH_EXTERN(two_pi);     //!< extern
Y_MATH_EXTERN(half_pi);    //!< extern
Y_MATH_EXTERN(min_exp);    //!< extern
Y_MATH_EXTERN(max_exp);    //!< extern
Y_MATH_EXTERN(dig);        //!< extern
Y_MATH_EXTERN(min_10_exp); //!< extern
Y_MATH_EXTERN(max_10_exp); //!< extern
Y_MATH_EXTERN(ftol);       //!< extern
Y_MATH_EXTERN(sqrt_ftol);  //!< extern
Y_MATH_EXTERN(tiny);       //!< extern
Y_MATH_EXTERN(huge);       //!< extern

#endif

#endif


#endif


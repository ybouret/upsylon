//! \file
#ifndef Y_MATH_TYPES_INCLUDED
#define Y_MATH_TYPES_INCLUDED 1

#include "y/type/complex.hpp"
#include "y/type/utils.hpp"
#include "y/functor.hpp"
#include "y/sequence/array.hpp"
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
            static const int    min_10_exp; //!< minimal 10 exponent
            static const int    max_10_exp; //!< maximal 10 exponent
            static const T      tiny;       //!< \f$10^{min_{10}exp}\f$
            static const T      huge;       //!< \f$10^{max_{10}exp}\f$
            static const T      gold;       //!< golden ratio
            typedef functor<T,TL1(T)>                     function;            //!< 1-argument function wrapper
            typedef functor<T,TL1(const array<T>&)>       scalar_field;        //!< scalar field (for gradient)
            typedef functor<T,TL2(T,const array<T>&)>     parametric_function; //!< parametric function
            typedef functor<array<T>,TL1(const array<T>)> vector_field;        //!< vector field
            
            
        };
        
        
        inline float  sqrt_of( const float  f ) throw() { return sqrtf(f); } //!< sqrt
        inline double sqrt_of( const double f ) throw() { return sqrt(f);  } //!< sqrt
        
        inline float  __fabs( const float  f ) throw() { return fabsf(f); }       //!< fabs
        inline double __fabs( const double f ) throw() { return fabs(f);  }       //!< fabs
        inline unit_t __fabs( const unit_t f ) throw() { return (f<0) ? -f : f; } //!< fabs
        inline float  __fabs( const complex<float>  c) throw() { return max_of(fabsf(c.re),fabsf(c.im)); } //!< fabs
        inline double __fabs( const complex<double> c) throw() { return max_of(fabs(c.re),fabs(c.im));   } //!< fabs
        
        //! return the signed value of a w.r.t the sign of b
        template <typename T, typename U>
        inline T __sgn(T a, U b) throw()
        {
            return (b >= 0) ? __fabs(a) : -__fabs(a);
        }
        
        template <typename T>
        inline T      __id(const T &f) { return f; } //!< identity operator
        
        template <typename T>
        inline typename real_for<T>::type __abs( const T &f ) { return __fabs(f); } //!< absolute value operator
        
        inline float  __pow( const float  x, const float  p) throw() { return powf(x,p); } //!< pow
        inline double __pow( const double x, const double p) throw() { return pow(x,p); }  //!< pow
        
        inline float   __mod2(const float  x) throw() { return x*x; }               //!< |x|^2
        inline double  __mod2(const double x) throw() { return x*x; }               //!< |x|^2
        inline unit_t  __mod2(const unit_t x) throw() { return x*x; }               //!< |x|^2
        inline float   __mod2(const complex<float>  x) throw() { return x.mod2(); } //!< |x|^2
        inline double  __mod2(const complex<double> x) throw() { return x.mod2(); } //!< |x|^2
        
        inline float  __floor( const float  x ) throw() { return floorf(x); } //!< floor
        inline double __floor( const double x ) throw() { return floor(x);  } //!< floor
        inline unit_t __floor( const unit_t x ) throw() { return x; }         //!< floor
        
        inline float  __ceil( const float  x ) throw() { return ceilf(x); } //!< floor
        inline double __ceil( const double x ) throw() { return ceil(x);  } //!< floor
        inline unit_t __ceil( const unit_t x ) throw() { return x; }        //!< ceil
        
        inline float  __log10( const float  x ) throw() { return log10f(x); } //!< log10
        inline double __log10( const double x ) throw() { return log10(x); }  //!< log10
        
        inline float  __anint( const float  x ) throw() { return floorf(x+0.5f); } //!< nearest integer
        inline double __anint( const double x ) throw() { return floor(x+0.5); }   //!< nearest integer

        inline float  __log(const float x) throw() { return logf(x); } //!< natural log
        inline double __log(const double x) throw() { return log(x); } //!< natural log

        inline float  __exp(const float  x) throw() { return expf(x); } //!< exp
        inline double __exp(const double x) throw() { return exp(x);  } //!< exp

        inline float  cos_of( const float  x ) throw() { return cosf(x); } //!< cosine
        inline double cos_of( const double x ) throw() { return cos(x);  } //!< cosine

        inline float  sin_of( const float  x ) throw() { return sinf(x); } //!< sine
        inline double sin_of( const double x ) throw() { return sin(x);  } //!< sine
        inline float  __atan(const float  x) { return atanf(x); } //!< arc tangent
        inline double __atan(const double x) { return atan(x); }  //!< arc tangent

        float  __atan2(float y,float x);     //!< in \f$]-\pi,\pi]\f$
        double __atan2(double y,double x);   //!< in \f$]-\pi,\pi]\f$

        template <typename T>
        static inline bool almost_equal( const T X, const T Y) throw()
        {
            static T fac = T(0.5) * numeric<T>::epsilon;
            return ( __fabs(X-Y) <= fac * ( __fabs(X) + __fabs(Y) ) );
        }
        
        //! hypothenuse withouth underflow or overflowe
        inline float __hypotenuse( float a, float b) throw()
        {
            const float absa=__fabs(a);
            const float absb=__fabs(b);
            if (absa > absb)
            {
                return absa*sqrtf(1.0f+square_of(absb/absa));
            }
            else
            {
                return (absb <= 0.0f ? 0.0f : absb*sqrtf(1.0f+square_of(absa/absb)));
            }
        }
        
        //! hypothenuse withouth underflow or overflowe
        inline double __hypotenuse( double a, double b) throw()
        {
            const double absa=__fabs(a);
            const double absb=__fabs(b);
            if (absa > absb)
            {
                return absa*sqrt(1.0+square_of(absb/absa));
            }
            else
            {
                return (absb <= 0.0 ? 0.0 : absb*sqrt(1.0+square_of(absa/absb)));
            }
        }

        float  __angle_of(float  c,float  s) throw(); //!< c=cos(phi), s=sin(phi)
        double __angle_of(double c,double s) throw(); //!< c=cos(phi), s=sin(phi)


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
//Y_MATH_EXTERN(dig);        //!< extern
//Y_MATH_EXTERN(min_10_exp); //!< extern
//Y_MATH_EXTERN(max_10_exp); //!< extern
Y_MATH_EXTERN(ftol);       //!< extern
Y_MATH_EXTERN(sqrt_ftol);  //!< extern
Y_MATH_EXTERN(tiny);       //!< extern
Y_MATH_EXTERN(huge);       //!< extern
Y_MATH_EXTERN(gold);       //!< extern
#endif

#endif



#endif


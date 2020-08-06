//! \file
#ifndef Y_TYPE_UTILS_INCLUDED
#define Y_TYPE_UTILS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    //! default minimum
    template <typename T> inline T min_of( const T a, const T b ) { return (a<b) ? a:b; }

    //! default maximum
    template <typename T> inline T max_of( const T a, const T b ) { return (b<a) ? a:b; }

    //! nested maxium of three items
    template <typename T> inline T max_of( const T a, const T b, const T c )
    { return max_of(a,max_of(b,c)); }


    //! default absolute value
    template <typename T> inline T abs_of(const T a) { return (a<T(0)) ? -a : a; }

    //! max_of(0,a)
    template <typename T> inline T geqz(const T a)
    {
        static const T zero(0);
        return (a<=zero) ? zero : a;
    }

    //! default sign value
    template <typename T> inline int sign_of( const T a ) { return (a<0) ? -1 : (  (0<a) ? 1 : 0 );     }

    //! default clamping of value
    template <typename T> inline T clamp( const T amin, const T a, const T amax ) { return (a<amin) ? amin : ( (amax<a) ? amax : a );     }

    //! default squared value
    template <typename T> inline T square_of( const T a ) { return a*a; }

   
   
   

}

#endif


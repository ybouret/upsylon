//! \file
#ifndef Y_ARITH_INCLUDED
#define Y_ARITH_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    struct arithmetic
    {
        //! greated commond divider, assuming positive values
        template <typename T> static inline
        T gcd( T x , T y )
        {
            if( x > 0 && y > 0 )
            {
                T a = x;
                T b = y;
                if( b > a )
                {
                    a = y;
                    b = x;
                }
                while( b > 0 )
                {
                    const T t = b;
                    b = a % b;
                    a = t;
                }
                return a;
            }
            else
                return T(1);
        }

        template <typename T> static inline
        void simplify( T &num, T &den )
        {
            const T __gcd = gcd(num,den);
            num /=  __gcd;
            den /=  __gcd;
        }

        //! least common multiplier
        template <typename T> static inline
        T lcm( T x , T y )
        {
            return ( x * y ) / gcd( x, y );
        }
    };
}

#endif


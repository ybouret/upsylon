//! \file
#ifndef Y_ARITH_INCLUDED
#define Y_ARITH_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    //! basic arithmetic algorithm
    struct arithmetic
    {
        //! greated commond divider, assuming positive values
        template <typename T> static inline
        T gcd( T x , T y )
        {
            if( x > 0 && y > 0 )
            {
                return gcd_positive(x,y);
            }
            else
                return T(1);
        }

        template <typename T> static inline
        T gcd_positive(const T &x, const T &y)
        {
            assert(x>0);
            assert(y>0);
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

        //! fraction simplification
        template <typename T> static inline
        void simplify( T &num, T &den )
        {
            if(num==0)
            {
                den = 1;
            }
            else
            {
                assert(den>0);
                const T __gcd = gcd_positive(num,den);
                num /=  __gcd;
                den /=  __gcd;
            }
        }

        //! least common multiplier
        template <typename T> static inline
        T lcm( T x , T y )
        {
            return ( x * y ) / gcd( x, y );
        }

    private:
        
    };
}

#endif


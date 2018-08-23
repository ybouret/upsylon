//! \file
#ifndef Y_ARITH_INCLUDED
#define Y_ARITH_INCLUDED 1

#include "y/type/xnumeric.hpp"

namespace upsylon
{
    //! basic arithmetic algorithm
    struct arithmetic
    {
        //! greated commond divider, assuming positive values
        template <typename T> static inline
        T gcd( T x , T y )
        {
            if( xnumeric<T>::is_positive(x) && xnumeric<T>::is_positive(y) )
            {
                return gcd_positive(x,y);
            }
            else
                return T(1);
        }

        //! gcd for positive types
        template <typename T> static inline
        T gcd_positive(const T &x, const T &y)
        {
            assert(xnumeric<T>::is_positive(x));
            assert(xnumeric<T>::is_positive(y));
            T a = x;
            T b = y;
            if( b > a )
            {
                a = y;
                b = x;
            }
            while( xnumeric<T>::is_positive(b) )
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
            if(xnumeric<T>::is_zero(num))
            {
                den = 1;
            }
            else
            {
                assert(xnumeric<T>::is_positive(den)>0);
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


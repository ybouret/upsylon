//! \file
#ifndef Y_MATH_POLYNOMIAL_INCLUDED
#define Y_MATH_POLYNOMIAL_INCLUDED 1

#include "y/sequence/array.hpp"

namespace upsylon {

    namespace mkl {

        //! polynomial ops
        struct polynomial
        {
            //! fast evaluation
            template <typename T> static inline
            T eval( const T X, const array<T> &a ) throw()
            {
                assert(a.size()>0);
                size_t n=a.size();
                T ans = a[n--];
                while(n>0)
                {
                    ans *= X;
                    ans += a[n--];
                }
                return ans;
            }
        };
    }

}

#endif


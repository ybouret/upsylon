//! \file
#ifndef Y_MATH_KERNEL_ERRORS_INCLUDED
#define Y_MATH_KERNEL_ERRORS_INCLUDED 1

#include "y/math/types.hpp"

namespace upsylon {

    namespace math {

        //! computing some erros
        struct errors
        {

            //! 100*aerr/aorg
            template <typename T> static inline
            T percent( const T aerr, const T aorg ) throw()
            {
                static const T half      = T(0.5);
                static const T hundred   = T(100);
                static const T precision = T(100);
                static const T multiply  = precision * hundred;
                static const T tiny      = numeric<T>::tiny;
                const T num = fabs_of(aerr);
                const T den = max_of( num, fabs_of(aorg) );
                if(num<=0&&den<=0)
                {
                    return 0;
                }
                else
                {
                    const T rho = (num+tiny)/(den+tiny);
                    const T fac = clamp<T>(0, floor_of( multiply * rho + half), multiply );
                    return  fac/precision;
                }
            }
        };

    }

}
#endif


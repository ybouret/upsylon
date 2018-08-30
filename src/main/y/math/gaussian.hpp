//! \file
#ifndef Y_MATH_GAUSSIAN_INCLUDED
#define Y_MATH_GAUSSIAN_INCLUDED 1

#include "y/math/types.hpp"

namespace upsylon
{
    namespace math
    {
        template <typename T>
        class gaussian
        {
        public:
            const T mu;
            const T sigma;
            const T scale;
            const T factor;

            inline explicit gaussian(const T __mu,
                                     const T __sigma ) :
            mu(__mu),
            sigma(__sigma),
            scale( 2*sigma*sigma ),
            factor( sqrt_of( numeric<T>::pi * scale ) )
            {
            }

            inline  gaussian( const gaussian &other ) throw() :
            mu(other.mu),
            sigma(other.sigma),
            scale(other.scale),
            factor(other.factor)
            {
            }

            inline virtual ~gaussian() throw()
            {
            }

            inline T operator()( const T x ) const
            {
                return __exp( -square_of(x-mu) / scale )/factor;
            }


        private:
            Y_DISABLE_ASSIGN(gaussian);
        };
    }
}

#endif


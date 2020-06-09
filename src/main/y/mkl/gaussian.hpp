//! \file
#ifndef Y_MKL_GAUSSIAN_INCLUDED
#define Y_MKL_GAUSSIAN_INCLUDED 1

#include "y/mkl/types.hpp"

namespace upsylon
{
    namespace math
    {
        //! gaussian computaiton
        template <typename T>
        class gaussian
        {
        public:
            const T mu;     //!< center
            const T sigma;  //!< deviation
            const T scale;  //!< 2*sigma*sigma
            const T factor; //!< normalisation factor

            //! initialize
            inline explicit gaussian(const T __mu,
                                     const T __sigma ) :
            mu(__mu),
            sigma(__sigma),
            scale( 2*sigma*sigma ),
            factor( sqrt_of( numeric<T>::pi * scale ) )
            {
            }

            //! copy
            inline  gaussian( const gaussian &other ) throw() :
            mu(other.mu),
            sigma(other.sigma),
            scale(other.scale),
            factor(other.factor)
            {
            }

            //! destructor
            inline virtual ~gaussian() throw()
            {
            }

            //! return the value
            inline T operator()( const T x ) const
            {
                return exp_of( -square_of(x-mu) / scale )/factor;
            }


        private:
            Y_DISABLE_ASSIGN(gaussian);
        };
    }
}

#endif


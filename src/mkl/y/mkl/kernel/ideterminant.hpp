//! \file
#ifndef Y_MKL_IDETERMINANT_INCLUDED
#define Y_MKL_IDETERMINANT_INCLUDED 1

#include "y/yap/rational.hpp"
#include "y/mkl/kernel/determinant.hpp"

namespace upsylon
{
    
    namespace mkl
    {
        //! assuming integer type
        /**
         internal apq computation
         */
        template <typename T>
        apz ideterminant( const matrix<T> &a )
        {
            assert(a.rows>0);
            assert(a.is_square);
            const size_t   n=a.rows;
            matrix<apq>    b(n,n);
            for(size_t i=n;i>0;--i)
            {
                addressable<apq>     &bi = b[i];
                const accessible<T>  &ai = a[i];
                for(size_t j=n;j>0;--j)
                {
                    bi[j] = ai[j];
                }
            }
            const apq d = __determinant(b);
            return d.to_integer();
        }
        
    }
    
}
#endif

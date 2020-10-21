//! \file
#ifndef Y_MKL_APZ_DET_INCLUDED
#define Y_MKL_APZ_DET_INCLUDED 1

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
        apz apz_det( const matrix<T> &a )
        {
            assert(a.rows>0);
            assert(a.is_square);
            const size_t   n=a.rows;
            matrix<apq>    b(n,n);
            b.assign(a);
            const apq d = __determinant(b);
            return d.to_integer();
        }
        
    }
    
}
#endif

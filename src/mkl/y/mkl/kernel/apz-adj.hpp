
//! \file
#ifndef Y_MKL_APZ_ADJ_INCLUDED
#define Y_MKL_APZ_ADJ_INCLUDED 1

#include "y/yap/rational.hpp"
#include "y/mkl/kernel/determinant.hpp"

namespace upsylon
{
    namespace mkl
    {
        
        
        //! compute adjoint for integer types
        template <typename U>
        inline void apz_adj( matrix<apz> &target, const matrix<U> &source )
        {
            assert(target.rows==source.rows);
            assert(target.cols==source.cols);
            assert(source.is_square);
            const size_t n = source.rows;
            if(n<=1)
            {
                target[1][1] = 1;
            }
            else
            {
                const size_t   nm1 = n-1;
                matrix<apq>    M(nm1,nm1);
                for(size_t c=n;c>0;--c)
                {
                    addressable<apz> &tc = target[c];
                    for(size_t r=n;r>0;--r)
                    {
                        M.minor_of(source,r,c);
                        const apz d = __determinant(M).to_integer();
                        tc[r] = ( (r+c) & 1) ? -d:d;
                    }
                }
            }
        }
        
        //! simplify::on(a,d)
        apz apz_rescale( matrix<apz> &a, apz &d );
        
    }
    
}

#endif

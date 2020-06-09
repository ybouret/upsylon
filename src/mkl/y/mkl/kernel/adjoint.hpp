
//! \file
#ifndef Y_MKL_ADJOINT_INCLUDED
#define Y_MKL_ADJOINT_INCLUDED 1

#include "y/mkl/kernel/determinant.hpp"

namespace upsylon
{
    namespace math
    {
        //! compute adjoint matrix, transpose of comatrix
        /**
         possible to use and integer type source matrix and a field target matrix
         */
        template <typename T, typename U>
        inline void adjoint( matrix<T> &target, const matrix<U> &source )
        {
            assert(target.rows==source.rows);
            assert(target.cols==source.cols);
            assert(source.is_square);
            const size_t n = source.rows;
            if(n<=1)
            {
                target[1][1] = T(1);
            }
            else
            {
                const size_t nm1 = n-1;
                matrix<T> M(nm1,nm1);
                for(size_t c=n;c>0;--c)
                {
                    addressable<T> &tc = target[c];
                    for(size_t r=n;r>0;--r)
                    {
                        M.minor_of(source,r,c);
                        const T d = __determinant(M);
                        tc[r] = ( (r+c) & 1) ? -d:d;
                    }
                }
            }
        }

        //! compute adjoint for integer types, using internal double precision computation
        /**
         beware of overflow!
         */
        template <typename T, typename U>
        inline void iadjoint( matrix<T> &target, const matrix<U> &source )
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
                matrix<double> M(nm1,nm1);
                for(size_t c=n;c>0;--c)
                {
                    addressable<T> &tc = target[c];
                    for(size_t r=n;r>0;--r)
                    {
                        M.minor_of(source,r,c);
                        const double dd =__determinant(M);
                        const T      d  = T( floor(dd+0.5));
                        tc[r] = ( (r+c) & 1) ? -d:d;
                    }
                }
            }
        }


    }
}

#endif



//! \file
#ifndef Y_MATH_ADJOINT_INCLUDED
#define Y_MATH_ADJOINT_INCLUDED 1

#include "y/math/kernel/determinant.hpp"

namespace upsylon
{
    namespace math
    {
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
                    array<T> &tc = target[c];
                    for(size_t r=n;r>0;--r)
                    {
                        M.minor_of(source,r,c);
                        const T d = __determinant(M);
                        tc[r] = ( (r+c) & 1) ? -d:d;
                    }
                }
            }
        }


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
                const size_t nm1 = n-1;
                matrix<T> M(nm1,nm1);
                for(size_t c=n;c>0;--c)
                {
                    array<T> &tc = target[c];
                    for(size_t r=n;r>0;--r)
                    {
                        M.minor_of(source,r,c);
                        const T d = ideterminant(M);
                        tc[r] = ( (r+c) & 1) ? -d:d;
                    }
                }
            }
        }


    }
}

#endif


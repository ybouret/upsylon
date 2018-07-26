//! \file
#ifndef Y_MATH_DETERMINANT_INCLUDED
#define Y_MATH_DETERMINANT_INCLUDED 1

#include "y/math/kernel/lu.hpp"

namespace upsylon
{
    namespace math
    {

        //! compute determinant, with LU decomposition of a
        /**
         so that T must be a filed (float,double,complex,rational)
         */
        template <typename T>
        T __determinant( matrix<T> &a )
        {
            assert(a.rows>0);
            assert(a.is_square);
            bool dneg=false;
            if(!LU::build(a,&dneg))
            {
                return 0;
            }
            const size_t n = a.rows;
            T det = a[1][1];
            for(size_t i=2;i<=n;++i)
            {
                det *= a[i][i];
            }
            return dneg ? -det : det;
        }

        //! determinant using a copy of a
        template <typename T>
        T determinant( const matrix<T> &a )
        {
            matrix<T> tmp(a);
            return __determinant(tmp);
        }

        //! assuming integer type
        /**
         internal double computation
         */
        template <typename T>
        T ideterminant( const matrix<T> &a )
        {
            assert(a.rows>0);
            assert(a.is_square);
            const size_t   n=a.rows;
            matrix<double> b(n,n);
            for(size_t i=n;i>0;--i)
            {
                array<double>  &bi = b[i];
                const array<T> &ai = a[i];
                for(size_t j=n;j>0;--i)
                {
                    bi[j] = double(ai[j]);
                }
            }
            return T( floor( __determinant(b) + 0.5 ) );
        }


    }
}

#endif


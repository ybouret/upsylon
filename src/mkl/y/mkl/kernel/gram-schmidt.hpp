//! \file

#ifndef Y_MKL_GRAM_SCHMIDT_INCLUDED
#define Y_MKL_GRAM_SCHMIDT_INCLUDED 1

#include "y/container/matrix.hpp"
#include "y/yap/rational.hpp"
#include "y/mkl/tao.hpp"
#include "y/counting/comb.hpp"

namespace upsylon {
    
    namespace mkl {

        //______________________________________________________________________
        //
        //
        //! Gram-Schmidt Ortho(normalisation)
        //
        //______________________________________________________________________
        struct GramSchmidt
        {
            //__________________________________________________________________
            //
            //! build orthonormal family, no normalisation
            //__________________________________________________________________
            template <typename T> static inline
            bool Ortho( matrix<T> &a )
            {
                const size_t     rows = a.rows;
                if(rows>0)
                {
                    //__________________________________________________________
                    //
                    // loop over rows
                    //__________________________________________________________
                    const size_t cols = a.cols;
                    for(size_t i=2;i<=rows;++i)
                    {
                        addressable<T> &v = a[i];
                        for(size_t k=1;k<i;++k)
                        {
                            const accessible<T> &u = a[k];
                            const T num = tao::dot<T>:: of(v,u);
                            const T den = tao::dot<T>:: of(u,u);
                            if(fabs_of(den)<=0)
                            {
                                return false;
                            }
                            const T fac = num/den;
                            for(size_t j=cols;j>0;--j)
                            {
                                v[j] -= fac * u[j];
                            }
                        }
                    }
                    
                    //__________________________________________________________
                    //
                    // check last one
                    //__________________________________________________________
                    const accessible<T> &last = a[rows];
                    return  tao::mod2<T>:: of(last)> 0;
                }
                else
                    return true;
            }
            
            //! build ortho and convert to integer
            /**
             - orthonormalize
             - convert to integer
             - minimum of minus signs
             - first not zero coordinate is positive
             */
            static bool OrthoSimple( matrix<apq> &a );


            //! build ortho from integer matrix
            template <typename T>
            static bool iOrtho( matrix<T> &a )
            {
                const size_t rows = a.rows;
                const size_t cols = a.cols;
                matrix<apq> q(rows,cols);
                for(size_t i=rows;i>0;--i)
                {
                    for(size_t j=cols;j>0;--j)
                    {
                        q[i][j] = static_cast<yap::number::itype>(a[i][j]);
                    }
                }
                if(!OrthoSimple(q)) return false;
                
                for(size_t i=rows;i>0;--i)
                {
                    for(size_t j=cols;j>0;--j)
                    {
                        a[i][j] = q[i][j].num.cast_to<T>();
                    }
                }
                return true;
            }
            
        };
    }
}
#endif


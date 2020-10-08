//! \file

#ifndef Y_MKL_GRAM_SCHMIDT_INCLUDED
#define Y_MKL_GRAM_SCHMIDT_INCLUDED 1

#include "y/container/matrix.hpp"
#include "y/yap/rational.hpp"
#include "y/mkl/kernel/quark.hpp"
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
                            const T num = quark::dot<T>:: of(v,u);
                            const T den = quark::dot<T>:: of(u,u);
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
                    return  quark::mod2<T>:: of(last)> 0;
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

            //! complete a set of independent vectors
            template <typename T>
            static bool iOrthoRows( matrix<T> &q, const matrix<T> &a  )
            {
                static const yap::number::itype zero = 0;
                static const yap::number::itype one  = 1;

                const size_t rows = a.rows;
                const size_t cols = a.cols; assert(rows<=cols);
                const size_t perp = q.rows; assert(perp+rows==cols);
                assert(perp>0);
                assert(q.cols==a.cols);
                matrix<apq> f(cols,cols);
                combination comb(cols,perp);
                for(comb.boot();comb.good(); comb.next())
                {
                    //----------------------------------------------------------
                    // fill img rows
                    //----------------------------------------------------------
                    for(size_t i=rows;i>0;--i)
                    {
                        for(size_t j=cols;j>0;--j)
                        {
                            f[i][j] = static_cast<yap::number::itype>(a[i][j]);
                        }
                    }

                    //----------------------------------------------------------
                    // fill sup rows
                    //----------------------------------------------------------
                    for(size_t i=1;i<=perp;++i)
                    {
                        array<apq> &f_i = f[rows+perp];
                        quark::ld(f_i,zero);
                        f_i[ comb[i] ] = one;
                    }

                    if( OrthoSimple(f) )
                    {
                        for(size_t i=perp;i>0;--i)
                        {
                            array<T>         &q_i = q[i];
                            const array<apq> &f_i = f[i+rows];
                            for(size_t j=cols;j>0;--j)
                            {
                               q_i[j] = f_i[j].num.cast_to<T>();
                            }
                        }
                        return true;
                    }

                }

                return false;

            }

        };
    }
}
#endif


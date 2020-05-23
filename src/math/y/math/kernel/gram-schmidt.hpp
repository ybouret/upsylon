//! \file

#ifndef Y_MKL_GRAM_SCHMIDT_INCLUDED
#define Y_MKL_GRAM_SCHMIDT_INCLUDED 1

#include "y/container/matrix.hpp"
#include "y/math/kernel/quark.hpp"

namespace upsylon {
    
    namespace math {
     
        struct GramSchmidt
        {
            template <typename T> static inline
            bool Ortho( matrix<T> &a )
            {
                const size_t    cols = a.cols;
                const size_t    rows = a.rows;
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
                return true;
            }
            
        };
    }
}
#endif


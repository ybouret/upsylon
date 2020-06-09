#include "y/math/kernel/gram-schmidt.hpp"

namespace upsylon {

    namespace math {

        bool GramSchmidt:: OrthoSimple( matrix<mpl::rational> &a )
        {
            if(! Ortho(a) ) return false;
            
            // rescale
            const size_t rows = a.rows;
            const size_t cols = a.cols;
            for(size_t i=1;i<=rows;++i)
            {
                addressable<mpq> &r   = a[i];
                mpn              big = r[1].den;
                for(size_t j=2;j<=a.cols;++j)
                {
                    const mpn  &tmp = r[j].den;
                    if(tmp>big) big = tmp;
                }
                const mpq fac = big;
                quark::rescale(r,fac);
                size_t np = 0;
                size_t nm = 0;
                size_t nz = 0;
                for(size_t j=1;j<=cols;++j)
                {
                    switch(r[j].num.s)
                    {
                        case mpl::__positive: ++np; break;
                        case mpl::__negative: ++nm; break;
                        case mpl::__zero:     ++nz; break;
                    }
                }
                assert(np+nm+nz==cols);
                if(nm>np)
                {
                    quark::neg(r);
                }
                else
                {
                    for(size_t j=1;j<=cols;++j)
                    {
                        const mpl::sign_type &s = r[j].num.s;
                        if(mpl::__zero==s) continue;
                        if(mpl::__negative==s)
                        {
                            quark::neg(r);
                            break;
                        }
                    }
                }

            }
            
            return true;
        }

    }
    
}


#include "y/mkl/kernel/gram-schmidt.hpp"

namespace upsylon {

    namespace mkl {

        bool GramSchmidt:: OrthoSimple( matrix<apq> &a )
        {
            if(! Ortho(a) ) return false;
            
            // rescale
            const size_t rows = a.rows;
            const size_t cols = a.cols;
            for(size_t i=1;i<=rows;++i)
            {
                addressable<apq> &r  = a[i];
                apn              big = r[1].den;
                for(size_t j=2;j<=a.cols;++j)
                {
                    const apn  &tmp = r[j].den;
                    if(tmp>big) big = tmp;
                }
                const apq fac = big;
                quark::rescale(r,fac);
                size_t np = 0;
                size_t nm = 0;
                size_t nz = 0;
                for(size_t j=1;j<=cols;++j)
                {
                    switch(r[j].num.s)
                    {
                        case yap::__positive: ++np; break;
                        case yap::__negative: ++nm; break;
                        case yap::__zero:     ++nz; break;
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
                        const yap::sign_type &s = r[j].num.s;
                        if(yap::__zero==s) continue;
                        if(yap::__negative==s)
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


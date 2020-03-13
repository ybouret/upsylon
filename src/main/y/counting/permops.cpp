#include "y/counting/permops.hpp"
#include "y/type/cswap.hpp"

namespace upsylon {

    void permops::init(size_t      *p,
                       const size_t n) throw()
    {
        assert(p!=NULL);
        assert(n>0);
        for(size_t i=n;i>0;--i) p[i] = i;
    }

    void permops::init(size_t      *p,
                       const size_t n,
                       size_t      *a) throw()
    {
        assert(p!=NULL);
        assert(n>0);
        assert(a!=NULL);
        for(size_t i=n;i>0;)
        {
            p[i] = i;
            --i;
            a[i] = i;
        }
    }

    void permops:: to_C(size_t *a, const size_t *p, const size_t n) throw()
    {
        for(size_t i=n;i>0;)
        {
            size_t q =p[i];
            a[--i] = --q;
        }
    }
    

    void permops::next(size_t      *p,
                       const size_t n) throw()
    {
        assert(p!=NULL);
        assert(n>0);
        //----------------------------------------------------------------------
        // find the largest perm[i]
        //----------------------------------------------------------------------
        size_t i=n-1;
        while( (i>0) && p[i]>p[i+1] ) --i;

        assert(i>0);
        //----------------------------------------------------------------------
        // find the largest element after perm[i] but not larger than perm[i]
        //----------------------------------------------------------------------
        {
            size_t k = n;
            while (p[i] > p[k])
            {
                --k;
            }
            cswap(p[i], p[k]);
        }

        //----------------------------------------------------------------------
        // swap the last n - i elements
        //----------------------------------------------------------------------
        const size_t jmax=(n+i)>>1;
        for(size_t k=0,j= ++i; j <= jmax; ++j, ++k)
        {
            cswap(p[j], p[n-k]);
        }
    }


    
}

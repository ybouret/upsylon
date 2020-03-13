#include "y/counting/ops.hpp"

namespace upsylon {

    namespace core {

        void counting:: init(size_t      *p,
                             const size_t n) throw()
        {
            assert(p!=NULL);
            assert(n>0);
            for(size_t i=n;i>0;--i) p[i] = i;
        }

        void counting:: init(size_t      *p,
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

        void counting:: to_C(size_t *a, const size_t *p, const size_t n) throw()
        {
            for(size_t i=n;i>0;)
            {
                size_t q =p[i];
                a[--i] = --q;
            }
        }

    }
}

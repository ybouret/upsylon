#include "y/memory/allocator/global.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/utest/run.hpp"
#include "y/type/aliasing.hpp"
#include <typeinfo>

using namespace upsylon;

namespace
{
    struct block
    {
        void  *addr;
        size_t size;
    };


    static inline void doTestWith( memory::allocator &a )
    {
        static const size_t N = 100;
        block blk[N];
        for(size_t iter=0;iter<128;++iter)
        {
            for(size_t i=0;i<N;++i)
            {
                blk[i].size = i>0 ? alea.leq(100) : 0;
                blk[i].addr = a.acquire(blk[i].size);
            }
            alea.shuffle(blk,N);
            for(size_t i=0;i<N;++i)
            {
                a.release( blk[i].addr, blk[i].size );
            }
        }

        {
            size_t  len = 5 * sizeof(double);
            double *d   = (double *)a.acquire(len);
            double *end = d+5;
            Y_CHECK( aliasing::delta(d,end) == 5*sizeof(double) );
            void *p = d;
            a.release(p,len);
        }
    }

}


Y_UTEST(alloc)
{
    concurrent::singleton::verbose = true;
    
    doTestWith( memory::global::instance() );
    doTestWith( memory::pooled::instance() );
    doTestWith( memory::dyadic::instance() );


    
    {
        char buff[20];
        double *a = (double *)&buff[0];
        double *b = (double *)&buff[1];
        Y_CHECK(aliasing::delta(a,b)==1);
    }

    

    
}
Y_UTEST_DONE()




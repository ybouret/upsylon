#include "y/memory/allocator/global.hpp"
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

   

}


Y_UTEST(alloc)
{
    memory::allocator &a = memory::global::instance();
    {
        static const size_t N = 100;
        block blk[N];
        for(size_t iter=0;iter<128;++iter)
        {
            for(size_t i=0;i<N;++i)
            {
                blk[i].size = alea.leq(100);
                blk[i].addr = a.acquire(blk[i].size);
            }
            alea.shuffle(blk,N);
            for(size_t i=0;i<N;++i)
            {
                a.release( blk[i].addr, blk[i].size );
            }
        }
    }
    
    {
        size_t len = 5 * sizeof(double);
        double *d = (double *)a.acquire(len);
        double *end = d+5;
        std::cerr << "delta=" << aliasing::delta(d,end) << std::endl;
        void *p = d;
        a.release(p,len);
    }
    
    {
        char buff[20];
        double *a = (double *)&buff[0];
        double *b = (double *)&buff[1];
        std::cerr << "delta=" << aliasing::delta(a,b) << std::endl;
    }

    

    
}
Y_UTEST_DONE()




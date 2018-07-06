#include "y/concurrent/scheme/simd.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    struct info
    {
        double *number;
        size_t  count;
    };

    static inline
    void simd_kernel(void     *data,
                     parallel &context,
                     lockable &)
    {
        info &I = *(info *)data;
        double *d = I.number;
        size_t  n = I.count;
        size_t  p = 0;
        context.split(n,p);

        for(size_t i=n;i>0;--i,++p)
        {
            d[i] = 0;
        }
    }
}

#include "y/memory/cblock.hpp"

Y_UTEST(simd)
{
    size_t n = 1000;
    memory::cblock_of<double> blk(n);
    info   I =
    {
        blk.data, n
    };


    concurrent::simd engine(true);

    for(size_t i=0;i<128;++i)
    {
        engine.ld(simd_kernel,&I);
        engine.wait();
    }
    


}
Y_UTEST_DONE()


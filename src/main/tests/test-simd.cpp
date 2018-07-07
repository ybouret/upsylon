#include "y/concurrent/scheme/simd.hpp"
#include "y/utest/run.hpp"
#include "y/randomized/marsaglia.hpp"

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
        randomized::Kiss32 ran;
        ran.reset( context.rank );
        info   &I = *(info *)data;
        double *d = I.number;
        size_t  n = I.count;
        size_t  p = 0;
        context.split(n,p);

        for(size_t i=n;i>0;--i,++p)
        {
            for(size_t k=1024;k>0;--k)
            {
		    assert(p<I.count);
                d[p] += ran.to<double>();
            }
        }
    }
}

#include "y/memory/cblock.hpp"
#include "y/os/wtime.hpp"

Y_UTEST(simd)
{
    double   duration = 5;
    size_t   n = 10000;
    memory::cblock_of<double> blk(n);
    info   I =
    {
        blk.data, n
    };



    concurrent::simd engine(true);
    wtime  chrono;
    size_t cycles=0;
    for(cycles=0;chrono.query()<=duration;)
    {
        ++cycles;
        engine.start(simd_kernel,&I);
        engine.finish();
    }
    


}
Y_UTEST_DONE()


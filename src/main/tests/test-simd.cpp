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
    //double   duration = 3;
    size_t   n        = 16384;
    memory::cblock_of<double> blk(n);
    info   I =
    {
        blk.data, n
    };

    concurrent::simd par(true);
    std::cerr << "-- in main --" << std::endl;
    par.start(simd_kernel,&I);
    //par.finish();

    rt_clock clk;
    //clk.sleep(2);

#if 0

    double par_speed = 0;
    concurrent::simd par(true);
    {
        wtime  chrono;
        size_t cycles=0;
        double ellapsed=0;
        for(cycles=0;(ellapsed=chrono.query())<=duration;)
        {
            ++cycles;
            par.start(simd_kernel,&I);
            par.finish();
        }
        par_speed = cycles/ellapsed;
    }
    std::cerr << "par_speed=" << par_speed << std::endl;

    double seq_speed = 0;
    {
        concurrent::sequential_for engine;
        wtime  chrono;
        size_t cycles=0;
        double ellapsed=0;
        for(cycles=0;(ellapsed=chrono.query())<=duration;)
        {
            ++cycles;
            engine.start(simd_kernel,&I);
            engine.finish();
        }
        seq_speed = cycles/ellapsed;
    }
    std::cerr << "seq_speed=" << seq_speed << std::endl;

    const double speed_up = par_speed/seq_speed;
    std::cerr << std::endl;
    std::cerr << "\tSpeed Up  : " << speed_up << std::endl;
    std::cerr << "\tEfficiency: " << par[0].efficiency(speed_up) << "%" << std::endl;
    std::cerr << std::endl;
#endif

}
Y_UTEST_DONE()


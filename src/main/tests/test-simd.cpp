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
    void simd_kernel0(void *,parallel &,lockable &)
    {

    }

    static inline
    void simd_kernel(void     *data,
                     parallel &context,
                     lockable &)
    {
        randomized::Kiss32 ran;
        ran.reset( uint32_t(context.rank) );
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

#include "y/ptr/cblock.hpp"
#include "y/os/wtime.hpp"
#include "y/string/env.hpp"

Y_UTEST(simd)
{
    std::cerr << "-- init program -- " << std::endl;
    rt_clock clk;
    double   duration = 3;
    size_t   n        = 16384;
    cblock<double>    blk(n);
    info   I =
    {
        &blk[0], n
    };

    environment::set(Y_THREADS_VERBOSITY,"1");

    {
        std::cerr << "-- testing no ops simd" << std::endl;
        concurrent::simd par0;
    }

    {
        std::cerr << "-- testing ops" << std::endl;
        concurrent::simd par1;
        for(size_t iter=0;iter<8;++iter)
        {
            std::cerr << "\t-- loop " << iter << std::endl;
            par1.run(simd_kernel0,0);
        }
    }

    
#if 1
    std::cerr << "-- testing parallelization" << std::endl;
    environment::set(Y_THREADS_VERBOSITY,"0");
    double           par_speed = 0;
    concurrent::simd par;
    //par.engine().acquire_all(12);
    {
        wtime  chrono;
        size_t cycles=0;
        double ellapsed=0;
        for(cycles=0;(ellapsed=chrono.query())<=duration; )
        {
            ++cycles;
            par.run(simd_kernel,&I);
        }
        par_speed = cycles/ellapsed;
        std::cerr << "par_speed=" << par_speed << " [#cycles=" << cycles << "]" << std::endl;
    }
    
    double seq_speed = 0;
    {
        concurrent::sequential_for engine;
        //engine.engine().acquire_all(10);
        wtime  chrono;
        size_t cycles=0;
        double ellapsed=0;
        for(cycles=0;(ellapsed=chrono.query())<=duration;)
        {
            ++cycles;
            engine.run(simd_kernel,&I);
        }
        seq_speed = cycles/ellapsed;
        std::cerr << "seq_speed=" << seq_speed << " [#cycles=" << cycles << "]" << std::endl;
    }

    const double speed_up = par_speed/seq_speed;
    std::cerr << std::endl;
    std::cerr << "\tSpeed Up  : " << speed_up << std::endl;
    std::cerr << "\tEfficiency: " << par.engine()[0].efficiency(speed_up) << "%" << std::endl;
    std::cerr << std::endl;

    std::cerr << "-- end program -- " << std::endl;
#endif
    
}
Y_UTEST_DONE()


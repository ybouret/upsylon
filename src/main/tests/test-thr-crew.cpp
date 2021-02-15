
#include "y/concurrent/device/crew.hpp"
#include "y/utest/run.hpp"
#include "y/os/real-time-clock.hpp"

using namespace upsylon;

namespace
{
    static size_t delta = 0;
    
    static inline
    void doNothing(void*,const concurrent::context &ctx, lockable &sync)
    {
        {
            Y_LOCK(sync);
            std::cerr << "-- doNothing in " << ctx.label << " -> " << delta << std::endl;
        }
        volatile double res = 0;
        for(size_t i=delta;i>0;--i)
        {
            res += 1.0/double(i);
        }
        
        
    }
}

Y_UTEST(thr_crew)
{
    if(argc>1)
    {
        delta = atol(argv[1]);
    }
    
    concurrent::crew Crew;
    Crew.run(doNothing,NULL);
    real_time_clock clk;
    if(argc>2)
    {
        clk.sleep(atof(argv[2]));
    }
    
    
    
}
Y_UTEST_DONE()



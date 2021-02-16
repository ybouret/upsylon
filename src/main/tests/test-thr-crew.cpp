
#include "y/concurrent/device/crew.hpp"
#include "y/concurrent/device/solo.hpp"

#include "y/os/real-time-clock.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;

namespace
{
    static size_t delta = 0;
    
    static inline
    void doNothing(void *,const concurrent::context &ctx, lockable &sync)
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
    concurrent::solo Solo;
    Solo.once(doNothing,NULL);
    
    if(argc>1)
    {
        delta = atol(argv[1]);
    }

    {
        std::cerr << "nope Crew" << std::endl;
        concurrent::crew Crew;
    }

    {
        std::cerr << "used Crew" << std::endl;
        concurrent::crew Crew;
        Crew.once(doNothing,NULL);
        real_time_clock clk;
        if(argc>2)
        {
            clk.sleep(atof(argv[2]));
        }
    }

    Y_UTEST_SIZEOF(concurrent::solo);
    Y_UTEST_SIZEOF(concurrent::crew);

    
}
Y_UTEST_DONE()



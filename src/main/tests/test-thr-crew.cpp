
#include "y/concurrent/device/crew.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    static inline
    void doNothing(void*,const concurrent::context &ctx, lockable &sync)
    {
        Y_LOCK(sync);
        std::cerr << "doNothing in " << ctx.label << std::endl;
    }
}

Y_UTEST(thr_crew)
{
    concurrent::crew Crew;
    std::cerr << "Will Run..." << std::endl;
    Crew.run(doNothing,NULL);
    
    
    
    
}
Y_UTEST_DONE()



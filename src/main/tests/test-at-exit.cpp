#include "y/concurrent/at-exit.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    static const at_exit::longevity life_time[3] =
    {
        1010,1000,1010
    };

    static inline void proc0(  )
    {
        std::cerr << "life_time0=" << life_time[0] << std::endl;
    }

    static inline void proc1(   )
    {
        std::cerr << "life_time1=" << life_time[1] << std::endl;
    }

    static inline void proc2(  )
    {
        std::cerr << "life_time2=" << life_time[2] << std::endl;
    }



}

Y_UTEST(at_exit)
{
    at_exit::perform(proc0,life_time[0]);
    at_exit::perform(proc1,life_time[1]);
    at_exit::perform(proc2,life_time[2]);

}
Y_UTEST_DONE()


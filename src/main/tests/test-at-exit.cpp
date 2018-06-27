#include "y/concurrent/at-exit.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    static const at_exit::longevity life_time[3] =
    {
        1010,1000,1010
    };

    static inline void proc0( void *args )
    {
        Y_CHECK(args||die("NULL args in proc0"));
        Y_CHECK(*(at_exit::longevity *)args == life_time[0]);
    }

    static inline void proc1( void *args )
    {
        Y_CHECK(args||die("NULL args in proc1"));
        Y_CHECK(*(at_exit::longevity *)args == life_time[1]);
    }

    static inline void proc2( void *args )
    {
        Y_CHECK(args||die("NULL args in proc2"));
        Y_CHECK(*(at_exit::longevity *)args == life_time[2]);

    }



}

Y_UTEST(at_exit)
{
    at_exit::perform(proc0,(void*)&life_time[0],life_time[0]);
    at_exit::perform(proc1,(void*)&life_time[1],life_time[1]);
    at_exit::perform(proc2,(void*)&life_time[2],life_time[2]);

}
Y_UTEST_DONE()


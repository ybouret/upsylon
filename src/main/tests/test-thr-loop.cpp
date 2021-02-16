#include "y/concurrent/device/sist.hpp"
#include "y/concurrent/device/simt.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;


namespace
{
    static inline
    void doSomething(void *,const concurrent::context &ctx, lockable &sync)
    {
        {
            Y_LOCK(sync);
            std::cerr << "doSomething@" << ctx.label << std::endl;
        }
    }
}

Y_UTEST(thr_loop)
{

    concurrent::sist seq_loop;
    concurrent::simt par_loop;

    par_loop.loop(doSomething,NULL);


}
Y_UTEST_DONE()



#include "y/concurrent/thread.hpp"
#include "y/concurrent/sync/mutex.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace {

    struct Working
    {
        concurrent::mutex &sync;
        int                task;

        static inline void Call(void *args)
        {
            Y_ASSERT(args);
            Working &w = *static_cast<Working *>(args);
            Y_LOCK(w.sync);
            std::cerr << "task#" << w.task << std::endl;
        }

    };

}

Y_UTEST(thr_thread)
{
    concurrent::mutex sync;

    Working w0 = { sync, 0 };
    Working w1 = { sync, 1 };
    Working w2 = { sync, 2 };

    concurrent::thread t0( Working::Call, &w0,3,0);
    concurrent::thread t1( Working::Call, &w1,3,1);
    concurrent::thread t2( Working::Call, &w2,3,2);



}
Y_UTEST_DONE()


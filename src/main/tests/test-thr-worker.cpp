#include "y/concurrent/worker.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace {

    struct Working
    {
        lockable &sync;
        int       item;

        static inline void Call(void *args)
        {
            assert(args);
            Working &self = *static_cast<Working *>(args);
            Y_LOCK(self.sync);
            std::cerr << "Called #" << self.item << std::endl;
        }

    };



}

Y_UTEST(thr_worker)
{
    concurrent::mutex sync;

    Working W0 = { sync, 0 };
    Working W1 = { sync, 1 };
    Working W2 = { sync, 2 };

    {
        concurrent::worker w0(Working::Call,&W0,3,0);
        concurrent::worker w1(Working::Call,&W1,3,1);
        concurrent::worker w2(Working::Call,&W2,3,2);
    }

}
Y_UTEST_DONE()


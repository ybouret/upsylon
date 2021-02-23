
#include "y/concurrent/task/worker.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;

namespace
{



}

Y_UTEST(thr_team)
{
    Y_UTEST_SIZEOF(concurrent::worker);
    Y_UTEST_SIZEOF(concurrent::condition);

}
Y_UTEST_DONE()


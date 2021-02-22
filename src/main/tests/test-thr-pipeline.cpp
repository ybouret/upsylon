#include "y/concurrent/task/pipeline.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;

Y_UTEST(thr_pipeline)
{

    concurrent::pipeline Q;

    Y_UTEST_SIZEOF(concurrent::pipeline::task);

}
Y_UTEST_DONE()



#include "y/concurrent/loop/env.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;

Y_UTEST(thr_loop_env)
{

    auto_ptr<concurrent::looper> engine = concurrent::looper_from_env::get();
    std::cerr << "engine.size=" << engine->size() << "/" << engine->category() << std::endl;
}
Y_UTEST_DONE()


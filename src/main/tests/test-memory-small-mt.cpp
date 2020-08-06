
#include "y/memory/small/hoard.hpp"
#include "y/concurrent/fake-lock.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace memory;

Y_UTEST(small_mt)
{
    concurrent::fake_lock access;
    
}
Y_UTEST_DONE()


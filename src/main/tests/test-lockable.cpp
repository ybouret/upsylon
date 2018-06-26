#include "y/concurrent/fake-lock.hpp"
#include "y/utest/run.hpp"
using namespace upsylon;

Y_UTEST(lockable)
{
    concurrent::fake_lock guard;
    {
        Y_LOCK(guard);
        Y_LOCK(guard);
        {
            Y_LOCK(guard);
        }
    }
}
Y_UTEST_DONE()


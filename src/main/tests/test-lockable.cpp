#include "y/concurrent/fake-lock.hpp"
#include "y/concurrent/mutex.hpp"
#include "y/utest/run.hpp"
using namespace upsylon;

static inline void test_lock( lockable &guard )
{
    Y_LOCK(guard);
    Y_LOCK(guard);
    {
        Y_LOCK(guard);
        if(guard.try_lock())
        {
            guard.unlock();
        }
    }
}

Y_UTEST(lockable)
{
    {
        concurrent::fake_lock guard;
        test_lock(guard);
    }
    
    std::cerr << "sizeof(mutex)=" << sizeof(concurrent::mutex) << std::endl;
    
    {
        concurrent::mutex guard;
        test_lock(guard);
    }
}
Y_UTEST_DONE()


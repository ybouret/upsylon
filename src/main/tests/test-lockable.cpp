#include "y/concurrent/fake-lock.hpp"
#include "y/concurrent/mutex.hpp"
#include "y/utest/run.hpp"
using namespace upsylon;

Y_UTEST(lockable)
{
    {
        concurrent::fake_lock guard;
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
    }
    
    std::cerr << "sizeof(mutex)=" << sizeof(concurrent::mutex) << std::endl;
}
Y_UTEST_DONE()


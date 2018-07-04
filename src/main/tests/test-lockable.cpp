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

    std::cerr << "sizeof(nucleus::mutex::type) = " << sizeof(concurrent::nucleus::mutex::type) << std::endl;
    std::cerr << "sizeof(mutex)                = " << sizeof(concurrent::mutex) << std::endl;
    std::cerr << "mutex.mutex_base_size        = " << concurrent::mutex::mutex_base_size << std::endl;
    std::cerr << "mutex.mutex_name_mini        = " << concurrent::mutex::mutex_name_mini << std::endl;
    std::cerr << "mutex.mutex_temp_size        = " << concurrent::mutex::mutex_temp_size << std::endl;
    std::cerr << "mutex.mutex_full_size        = " << concurrent::mutex::mutex_full_size << std::endl;
    std::cerr << "mutex.mutex_name_size        = " << concurrent::mutex::mutex_name_size << std::endl;


    {
        concurrent::mutex guard;
        test_lock(guard);
    }
}
Y_UTEST_DONE()


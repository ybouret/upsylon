#include "y/concurrent/threads.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    class some_threads : public concurrent:: threads
    {
    public:
        explicit some_threads() : concurrent::threads(true)
        {
        }

        virtual ~some_threads() throw()
        {
        }

        inline void call() throw()
        {
            synchronize.broadcast();
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(some_threads);
    };
}

Y_UTEST(threads)
{
    std::cerr << "sizeof(thread)=" << sizeof(concurrent::thread) << std::endl;
    some_threads threads;
    threads.call();
    threads.wait();

}
Y_UTEST_DONE()

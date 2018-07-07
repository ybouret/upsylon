#include "y/concurrent/threads.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    class some_threads : public concurrent:: threads
    {
    public:
        size_t                ready;
        concurrent::condition start;

        explicit some_threads() : concurrent::threads(true), ready(0), start()
        {
            
        }

        virtual ~some_threads() throw()
        {
        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(some_threads);
    };
}

Y_UTEST(threads)
{
    std::cerr << "sizeof(thread)=" << sizeof(concurrent::thread) << std::endl;

    some_threads threads;

}
Y_UTEST_DONE()

#include "y/concurrent/threads.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    class some_threads : public concurrent:: threads
    {
    public:
        concurrent::condition done;

        explicit some_threads() : concurrent::threads(true), done()
        {
        }

        virtual ~some_threads() throw()
        {
        }

        void call()
        {
            synchronize.broadcast();
        }

        virtual void run(parallel &ctx) throw()
        {

            access.lock();
            std::cerr << "some_threads run " << ctx.label << std::endl;
            std::cerr << "\trunning=" << running << "/" << size() << std::endl;
            if(running<size())
            {
                done.wait(access);
            }
            else
            {
                done.broadcast();
            }
            access.unlock();
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

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

        virtual void loop( parallel &context) throw()
        {
            Y_LOCK(access);
            std::cerr << "some_threads loop in " << context.label << std::endl;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(some_threads);
    };
}
            
#include <cstring>

Y_UTEST(threads)
{
    std::cerr << "sizeof(thread)=" << sizeof(concurrent::thread) << std::endl;

    bool run = true;
    if(argc>1 && !strcmp(argv[1],"false"))
    {
        run = false;
    }
    some_threads threads;

    if(run)
    {
        threads.run();
    }

}
Y_UTEST_DONE()

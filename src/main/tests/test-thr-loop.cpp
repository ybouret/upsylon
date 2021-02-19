

#include "y/concurrent/loop/simt.hpp"
#include "y/utest/run.hpp"
#include "y/os/real-time-clock.hpp"

using namespace upsylon;

namespace
{
    class doSome : public concurrent::runnable
    {
    public:
        explicit doSome() throw()
        {

        }

        virtual ~doSome() throw()
        {

        }

        virtual void run(const concurrent::context & ctx, lockable &sync)
        {
            Y_LOCK(sync);
            std::cerr << "\t...do something @" << ctx.label << std::endl;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(doSome);

    };
}

Y_UTEST(thr_loop)
{

    std::cerr << "Empty   SIMT" << std::endl;
    {
        concurrent::simt par;
    }
    std::cerr << std::endl;
    
    std::cerr << "Working SIMT" << std::endl;
    {
        concurrent::simt par;
        doSome           code;
        par.loop(code);
        {
            Y_LOCK(par.access);
            std::cerr << "launched!"<< std::endl;
        }
        par.join();
    }




    

}
Y_UTEST_DONE()


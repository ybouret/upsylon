

#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"
#include "y/utest/run.hpp"
#include "y/os/real-time-clock.hpp"

using namespace upsylon;

namespace
{
    static size_t work_loops = 0;

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
            {
                Y_LOCK(sync);
                std::cerr << "\t...do something @" << ctx.label << " / " << work_loops << std::endl;
            }

            volatile double sum = 0;
            for(size_t i=0;i<work_loops;++i)
            {
                sum += cos(double(i));
            }

        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(doSome);

    };

    class doNope : public concurrent::runnable
    {
    public:
        explicit doNope() throw() {}
        virtual ~doNope() throw() {}

        virtual void run(const concurrent::context &, lockable &)
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(doNope);
    };
}

#include "y/string/convert.hpp"

Y_UTEST(thr_loop)
{
    real_time_clock clk;

    if(argc>1)
    {
        work_loops = string_convert::to<size_t>(argv[1],"work_loops");
    }

    
    std::cerr << "Empty   SIMT" << std::endl;
    for(size_t i=0;i<2;++i)
    {
        concurrent::simt par;
    }
    std::cerr << std::endl;



    std::cerr << "Working SIMT" << std::endl;
    {
        concurrent::simt par;
        {
            doSome           code;

            std::cerr << "one loop" << std::endl;
            {
                par.for_each(code);
            }
        }

        
        {
            std::cerr << "many loops" << std::endl;
            doNope nope;
            for(size_t i=0;i<10000;++i)
            {
                par.for_each(nope);
            }
        }
    }

    std::cerr << "Working SOLO" << std::endl;
    {
        concurrent::solo seq;
        doSome code;
        seq.for_each(code);
    }
    std::cerr << std::endl;


    

}
Y_UTEST_DONE()


#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/type/utils.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/utest/timings.hpp"

using namespace upsylon;

namespace
{
    class Engine : public concurrent:: runnable
    {
    public:
        vector<double> sums;
        size_t         count;
        bool           verbose;

        explicit Engine() : sums(), count(0), verbose(true) {}

        void prepare(const concurrent::looper &l, const size_t n)
        {
            sums.make(l.size(),0);
            count=n;
        }

        virtual ~Engine() throw()
        {
        }

        virtual void run(const concurrent::context &ctx, lockable &sync) throw()
        {
            double sum    = 0;
            size_t offset = 1;
            size_t length = count;
            ctx.split(length,offset);
            if(verbose)
            {
                Y_LOCK(sync);
                std::cerr << "ctx @" << ctx.label << " => " << offset << "+" << length << std::endl;
            }
            while(length-- > 0)
            {
                sum += 1.0/square_of( double(offset++) );
            }
            sums[ctx.indx] = sum;
        }

        inline double result()   throw()
        {
            return sqrt(6.0*sorted_sum(sums));
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Engine);
    };

}

#include "y/string/convert.hpp"
#include "y/string/env.hpp"

Y_UTEST(thr_pi)
{
 
    size_t n = 10000000;

    if(argc>1)
    {
        n = string_convert::to<size_t>(argv[1],"");
    }

    Engine           engine;
    concurrent::solo seq;
    concurrent::simt par;

    engine.prepare(seq,n);
    seq.for_each(engine);

    std::cerr << "seq: " << engine.result() << std::endl;



    engine.prepare(par,n);
    par.for_each(engine);

    std::cerr << "par: " << engine.result() << std::endl;

    engine.prepare(seq,n);
    engine.verbose = false;

    double seq_speed = 0;

    Y_TIMINGS(seq_speed,1,seq.for_each(engine));
    std::cerr << "seq_speed=" << seq_speed << std::endl;

    engine.prepare(par,n);
    double par_speed = 0;
    Y_TIMINGS(par_speed,1,par.for_each(engine));
    std::cerr << "par_speed=" << par_speed << std::endl;

    std::cerr << "efficiency: " << par.efficiency(par_speed/seq_speed) << "%" << std::endl;

}
Y_UTEST_DONE()

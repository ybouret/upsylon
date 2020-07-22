
#include "y/concurrent/scheme/simd.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"

#include "y/string.hpp"
#include "y/container/matrix.hpp"
#include "y/os/wtime.hpp"
#include "y/counting/permutations.hpp"

#include <iomanip>

using namespace upsylon;

namespace {


    double doPerm(concurrent::for_each &loop,
                  const string         &source,
                  matrix<char>         &target)
    {
        typedef permutations<char> perm_type;

        const size_t          cols = source.size();
        {
            perm_type         perm(*source,cols);
            const size_t      rows = perm.count;
            target.make(rows,cols);
        }
        concurrent::executor &cpu = loop.engine();


        for(size_t i=0;i<cpu.num_threads();++i)
        {

        }

        struct task
        {
            static inline void run( void *args, parallel &ctx, lockable &access ) throw()
            {
                (void)access;
                task            &self    = *(task *)args;
            }
        };

        task todo = {   };
        double speed = 0;
        Y_TIMINGS(speed,1,loop.run( task::run, &todo));
        return speed;
    }


}


Y_UTEST(perm_par)
{
    concurrent::sequential_for seq;
    concurrent::simd           par;

    string source = "hello";
    if(argc>1)
    {
        source = argv[1];
    }
    std::cerr << "sequential:" << std::endl;
    matrix<char> target_seq;
    const double seq_speed = doPerm(seq,source,target_seq);
    std::cerr << "seq_speed=" << seq_speed << std::endl;

    matrix<char> target_par;
    const double par_speed = doPerm(par,source,target_par);
    std::cerr << "par_speed=" << par_speed << std::endl;

    std::cerr << "efficiency=" << par.engine()[0].efficiency(par_speed/seq_speed) << '%' << std::endl;


}
Y_UTEST_DONE()



#include "y/concurrent/scheme/simd.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"

#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/os/wtime.hpp"
#include "y/counting/permutations.hpp"

#include <iomanip>

using namespace upsylon;

namespace {


    double doPerm(concurrent::for_each &loop,
                  const string         &source,
                  vector<char>         &target)
    {
        typedef permutations<char> perm_type;

        const size_t          dims = source.size();
        const perm_type       perm(*source,dims);
        concurrent::executor &cpu = loop.engine();

        target.make( perm.count * dims,0 );
        cpu.vcopy<perm_type>( memory::storage::global, 2, perm );

        for(size_t i=0;i<cpu.num_threads();++i)
        {
            parallel  & ctx = cpu[i];
            perm_type & primary = ctx.get<perm_type>(0);
            perm_type & replica = ctx.get<perm_type>(1);
            Y_ASSERT(primary.has_same_state_than(perm));
            Y_ASSERT(replica.has_same_state_than(perm));
            ctx.mark = primary.boot_mark(ctx.size,ctx.rank);
         }

        struct task
        {
            vector<char> *frames;
            static inline void run( void *args, parallel &ctx, lockable &access ) throw()
            {
                (void)access;
                task            &self    = *(task *)args;
                const perm_type &primary = ctx.get<perm_type>(0);
                perm_type       &replica = ctx.get<perm_type>(1);
                vector<char>    &frames  = *self.frames;

                replica.reload(primary);

                size_t length = ctx.mark.length;
                size_t offset = ctx.mark.offset;

                const size_t  width  = primary.size();
                size_t        j      = 1+(offset-1)*width;

                while( length-- > 0 )
                {
                    assert(replica.index==offset);
                    assert(replica.good());
                    for(size_t i=1;i<=width;++i)
                    {
                        frames[j++] = replica[i];
                    }
                    replica.next();
                    ++offset;
                }
            }
        };

        task todo = { &target };
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
    vector<char> target_seq;
    const double seq_speed = doPerm(seq,source,target_seq);
    std::cerr << "seq_speed=" << seq_speed << std::endl;

    vector<char> target_par;
    const double par_speed = doPerm(par,source,target_par);
    std::cerr << "par_speed=" << par_speed << std::endl;

    std::cerr << "efficiency=" << par.engine()[0].efficiency(par_speed/seq_speed) << '%' << std::endl;

    Y_CHECK(target_seq.size()==target_par.size());
    bool identical = true;
    for(size_t i=target_seq.size();i>0;--i)
    {
        Y_ASSERT(target_par[i]==target_seq[i]);
    }
    Y_CHECK(identical);

}
Y_UTEST_DONE()



#include "y/counting/permuter.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"

#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/os/wtime.hpp"

#include <iomanip>

using namespace upsylon;

namespace {


    double doPerm(concurrent::for_each &loop,
                  const string         &source,
                  vector<char>         &target)
    {
        typedef permuter<char> perm_type;
        concurrent::executor  &ex = loop.engine();
        const size_t           nt = ex.num_threads();
        ex.make<perm_type, const char *, size_t >(memory::storage::global,*source,source.length());

        size_t count = 0;
        size_t nodes = 0;
        size_t created = 0;
        size_t width = source.size();

        {
            perm_type P(*source,width);
            P.unwind();
            count   = P.count;
            nodes   = P.store.required();
            created = P.store.created;
        }

        std::cerr << "#permutations=" << count << ", nodes=" << nodes << ", created=" << created << std::endl;
        
        for(size_t i=0;i<nt;++i)
        {
            parallel::cache_type &cache = ex(i); Y_ASSERT(cache.is_built_from<perm_type>());
            perm_type &p = ex(i).get<perm_type>();
            p.store.grow_cache(nodes);
            Y_ASSERT(p.store.required() == nodes);
            ex[i].mark = 0;
        }

        target.make(count*width,0);


        struct task
        {
            const string        *s;
            addressable<char>   *a;

            static inline void run(void *args, parallel &ctx, lockable &access)
            {
                (void) access;

                task                &t = *static_cast<task *>(args);
                perm_type           &p = ctx.get<perm_type>();
                const string        &s = *t.s;
                addressable<char>   &a = *t.a;
                

                assert( s.size() == p.size() );
                const uint64_t t0  = ctx.ticks(access);
                size_t         n   = p.boot(ctx.size,ctx.rank);
                const size_t   w   = s.size();
                char          *q   = &a[1+ (p.index-1) * w];
                p.store.created = 0;
                while(n-- > 0)
                {
                    assert(p.good());
                    p.apply(q);
                    q += w;
                    p.next();
                }
                assert(p.store.created<=0);
                ctx.mark += ctx.ticks(access) - t0;
            }

        };

        task todo = { &source, &target };
        double speed  = 0;
        size_t cycles = 0;
        Y_TIMINGS_(speed,2, loop.run( task::run, &todo ),cycles);
        std::cerr << "#cycles=" << cycles << std::endl;

        wtime chrono;

        for(size_t i=0;i<nt;++i)
        {
            const double workTime = chrono(ex[i].mark);
            const double workRate = double(cycles)/workTime;
            std::cerr << "\t\t#thread" << i << " : " << workRate << " cycles/s" << std::endl;
        }
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
    vector<char> target_par;
    vector<char> target_seq;
    std::cerr << "sequential:" << std::endl;
    const double seq_speed = doPerm(seq,source,target_seq);
    std::cerr << seq_speed << std::endl;

    std::cerr << "parallel:" << std::endl;
    const double par_speed = doPerm(par,source,target_par);
    std::cerr << par_speed << std::endl;


}
Y_UTEST_DONE()


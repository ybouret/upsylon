
#include "y/counting/permuter.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"

#include "y/string.hpp"
#include "y/sequence/vector.hpp"

#include <iomanip>

using namespace upsylon;

namespace {


    void doPerm(concurrent::for_each &loop,
                const string         &source)
    {
        typedef permuter<char> perm_type;
        concurrent::executor  &ex = loop.engine();
        const size_t           nt = ex.num_threads();
        ex.make<perm_type, const char *, size_t >( memory::storage::global,*source,source.length());
        size_t count = 0;
        size_t nodes = 0;
        {
            perm_type P(*source,source.size());
            P.unwind();
            count = P.count;
            nodes = P.required_nodes();
        }
        std::cerr << "count=" << count << ", nodes=" << nodes << std::endl;
        for(size_t i=0;i<nt;++i)
        {
            ex(i).get<perm_type>().extra(nodes);
        }

    }


}


Y_UTEST(perm_par)
{
    concurrent::sequential_for seq;
    concurrent::simd           par;

    string source = "hello";

    doPerm(par,source);
    doPerm(seq,source);


}
Y_UTEST_DONE()


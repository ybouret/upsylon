#include "y/concurrent/scheme/skimp.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

namespace
{
    struct ops
    {
        template <typename T, typename U> static inline
        void add( T &tgt, const U &src, parallel &, lockable & )
        {
            tgt += src;
        }

    };


    template <typename T,typename U>
    void test_with( concurrent::for_each &loop )
    {
        for(size_t n=1;n<=128; n <<= 1)
        {
            vector<T> target(n,0);
            vector<U> source(n,1);

            std::cerr << "n=" << n << std::endl;
            concurrent::skimp<size_t>::call( ops::add<T,U>, target, source, n, 1, loop);
            std::cerr << std::endl;
        }
    }
}

Y_UTEST(skimp)
{
    concurrent::sequential_for seq;
    concurrent::simd           par;

    test_with<int,int>(seq);
    test_with<int,int>(par);

}
Y_UTEST_DONE()
